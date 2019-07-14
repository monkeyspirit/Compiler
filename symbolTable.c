#include <string.h>
#include "symbolTable.h"
#include "semantic.h"

// Definizioni
char *tableTypes[] = {
        "CHAR",
        "INT",
        "REAL",
        "STRING",
        "BOOL",
        "VOID"
}, *tableClasses[] = {
        "VAR",
        "MOD",
        "CON",
        "PAR"
};


// Fine Definizioni

int hash(char *id) {
    int h=0;

    do h = ((h << 4) + *id) % BUCKET_SIZE;
    while(*++id != '\0');

    // for(int i=0; id[i] != '\0'; i++)
    //    h = ((h << 4) + id[i]) % BUCKET_SIZE;

    return h;
}

// Alloca la memoria per una nuova riga e la inizializza
PLine newLine() {
    PLine p = (PLine) calloc(1, sizeof(Line));
    //    for(int b=0; b<BUCKET_SIZE; b++)
    //        p->bucket[b] = 0; // devo metterli tutti a zero se no non li vede NULL
    return p;
}

void isDefined(PLine tableLine, char *id) {
    while (tableLine != NULL) {
        // printf("%s, %s\n", id, tableLine->id);
        if (strcmp(tableLine->id, id)==0) {
            printf("Errore: inserite due variabili omonime (%s)", tableLine->id);
            exit(-30);
        }
        tableLine = tableLine->next;
    }
}

// inserisce una riga in una tabella (se la riga è vuota tutto ok,
// altrimenti la inserisce in coda a quella già presente)
int insertLine(PLine table[], PLine line) {
    char *id = line->id;
    int index = hash(id);
    if (table[index] == NULL)
        table[index] = line;
    else {
        isDefined(table[index], line->id);
        PLine nextB = table[index];
        while(nextB->next != NULL)
            nextB = nextB->next;
        nextB->next = line;
    }
}

// crea una riga per un parametro a partire dal nodo
PLine createParamLine(Pnode paramNode) {
    PLine paramLine = newLine();
    paramLine->id = paramNode->child->value.sval;
    paramLine->class = tableClasses[3];
    paramLine->type = tableTypes[paramNode->child->brother->child->type];
    return paramLine;
}

// crea una riga per una variabile a partire dal nodo
PLine createVarLine(Pnode idNode, int type) {
    PLine varLine = newLine();
    varLine->id = idNode->value.sval;
    varLine->type = tableTypes[type];
    varLine->class = tableClasses[0];
    return varLine;
}

// crea una riga per una costante a partire dal nodo
PLine createConstLine(Pnode idNode, int type) {
    PLine varLine = newLine();
    varLine->id = idNode->value.sval;
    varLine->type = tableTypes[type];
    varLine->class = tableClasses[2];
    return varLine;
}


int global_oid = 0; // oid dei modulia

// crea una riga per un modulo a partire dal nodo
PLine createModuleLine(Pnode moduleNode) {

    PLine moduleLine = newLine(); // alloca memoria
    Pnode iterNode = moduleNode->child; // nodo che itera nel modulo
    moduleLine->id = iterNode->value.sval; // setta l'id
    moduleLine->class = tableClasses[1]; // setta la classe
    moduleLine->oid = global_oid++;

    iterNode = iterNode->brother;

    int local_oid = 0; // oid per le righe locali

    moduleLine->nFormalParams = 0;
    // controlliamo se ci sono dei parametri formali
    if(iterNode -> value.ival==NOPT_PARAM_LIST){
        moduleLine->formalParams = malloc(sizeof(PLine)); // chissa come... funziona

        Pnode paramNode = iterNode->child->child;
        while (paramNode != NULL) { // cicliamo su tutti i parametri
            PLine paramLine = createParamLine(paramNode);
            paramLine->oid = local_oid++;
            insertLine(moduleLine->bucket, paramLine); // li inseriamo nel bucket
            moduleLine->formalParams[moduleLine->nFormalParams++] = paramLine; // li inserisco tra i riferimenti ai formali
            // moduleLine->numParam++;

            paramNode = paramNode->brother;
        }

        iterNode = iterNode->brother;
    }

    moduleLine->type = tableTypes[iterNode->child->type]; // setta il tipo

    iterNode = iterNode->brother;

    //controlliamo se ci sono variabili
    if(iterNode->value.ival==NOPT_VAR_SECT){

        Pnode declNode = iterNode->child->child;
        while (declNode!=NULL) { // cicliamo sulle dichiarazioni
            int decl_type = declNode->child->brother->child->type;
            Pnode idNode = declNode->child->child;
            while(idNode != NULL){ // cicliamo su tutte le variabili
                PLine varLine = createVarLine(idNode, decl_type);
                varLine->oid = local_oid++;
                insertLine(moduleLine->bucket, varLine);
                idNode = idNode->brother;
            }

            declNode = declNode->brother;
        }

        iterNode = iterNode->brother;
    }

    // controlliamo se ci sono costanti
    if(iterNode->value.ival==NOPT_CONST_SECT){

        Pnode constDeclNode = iterNode->child->child;
        while(constDeclNode != NULL) {
            int decl_type = constDeclNode->child->child->brother->child->type;
            Pnode idNode = constDeclNode->child->child->child;
            while(idNode != NULL){
                PLine constLine = createConstLine(idNode, decl_type);
                constLine->oid = local_oid++;
                insertLine(moduleLine->bucket, constLine);
                idNode = idNode->brother;
            }

            constDeclNode = constDeclNode->brother;
        }

        iterNode = iterNode->brother;                                        //PUNTO A OPT-MODULE/MODULE BODY
    }

    // controlliamo se ci sono moduli
    if(iterNode->value.ival==NOPT_MODULE_LIST){

        Pnode modNode = iterNode->child;
        while (modNode != NULL) {
            insertLine(moduleLine->bucket, createModuleLine(modNode));
            modNode = modNode->brother;
        }

        iterNode = iterNode->brother;
    }

    return moduleLine;
}

PLine rootLine;
PLine symbolTable(Pnode root){
    rootLine = createModuleLine(root->child);
    return  rootLine;
}

// METODI PER STAMPARE LA TABELLA

FILE *out;

// stampa una sola riga (usato nella ricorsione)
void printLine(PLine line, int indent) {
    // tabbiamo con la giusta iNdeNtazione
    for(int tab=0; tab<indent; tab++) fprintf(out, "\t");

    fprintf(out, "[%d] %s , cls:%s , tp:%s , oid=%d", hash(line->id), line->id, line->class, line->type, line->oid);
    if(line->class==tableClasses[1]) { // se è un metodo stampo anche i parametri formali
        fprintf(out, " , parms(#=%d):", line->nFormalParams);
        for (int i = 0; i < line->nFormalParams; ++i)
            fprintf(out, "%s ", line->formalParams[i]->id);
    }
    fprintf(out, "\n");
}

// stampa un intero bucket
void printTable(PLine table[], int indent){
//    for(int tab=0; tab<indent; tab++) printf("\t");
//    printf("[hash] Name, Type, # Params, Class\n");

    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toPrint = table[i];
        while(toPrint!=NULL){
            printLine(toPrint, indent);

            if(toPrint->class == tableClasses[1]) // se è un modulo stampo pure il suo bucket
                printTable(toPrint->bucket, indent+1);

            toPrint = toPrint->next;
        }
    }
}

// stampa tutto a partire dalla rootline
void displayTable(){
    char *outSymbolTable= malloc(sizeof(char*));
    sprintf(outSymbolTable, "%s.symboltable", telaFileName);
    out = fopen(outSymbolTable, "w");
    printLine(rootLine, 0);
    printTable(rootLine->bucket, 1);
}

