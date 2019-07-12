//
// Created by maria on 10/07/19.
//

#include "tcode.h"
#include "semantic.h"
#include "counter(deprecato).h"
#include "dynamicArray.h"

#include <stdarg.h>


char **buffer=NULL;
int bufferSize=0;
FILE *out;


void tCodeGenerator(PLine pLine, Pnode pNode);

PLine wholeSymbolTable;

// metodi per gestire il buffer

// aggiunge una strina al buffer e aumenta il bufferSize
void bufferWrite(char *lineToWrite){
    buffer = realloc(buffer, sizeof(char*) * (bufferSize+1)); // rialloco la memoria
    buffer[bufferSize++] = lineToWrite; // aggiungo la rica
}

// fa come la printf ma sul buffer
void bprintf(char *stringToCompile, ...){
    char *compiledString = malloc(strlen(stringToCompile)); // devo allocare la memoria per la nuova stringa
    va_list args;
    va_start(args, stringToCompile);
    vsprintf(compiledString, stringToCompile, args); // elaboriamo la stringa
    bufferWrite(compiledString); // la aggiungiamo al buffer
    va_end(args);
}

// fa come la bprintf ma non aggiunge una riga in fondo
// bensì sovrascrive una già esistente (all'indirizzo specificato)
void bprintfAtIndex(int index, char *stringToCompile, ...){
    char *compiledString = malloc(strlen(stringToCompile)); // devo allocare la memoria per la nuova stringa
    va_list args;
    va_start(args, stringToCompile);
    vsprintf(compiledString, stringToCompile, args); // elaboriamo la stringa
    buffer[index] = compiledString; // la aggiungiamo al buffer
    va_end(args);
}

// scrive il buffer su file
void flush(){
    out = fopen("../tCode.out", "w");
    for (int i = 0; i < bufferSize; ++i)
        fprintf( out, "%s", buffer[i]);
    fclose(out);
}

// metodi generazione tcode

void tCode(PLine rootLine, Pnode root){
    wholeSymbolTable = rootLine;
    tCodeGenerator(rootLine, root);
    flush();
}

void tCodeGenerator(PLine rootLine, Pnode root){

    bprintf( "MOD %d\n", rootLine->oid);

//    if(rootLine!=wholeSymbolTable){
//        printf("Id: %s Father: %s\n", rootLine->id, getFather(rootLine, wholeSymbolTable)->id);
//    }
//
//    printf("Livello di %s: %d\n", rootLine->id, getLevelModule(rootLine, wholeSymbolTable));
//

    Pnode iterNode, paramNode, declNode, constDeclNode, modNode;
    iterNode = root->child; // punta a ID
    char* idMod = iterNode->value.sval;

    iterNode = iterNode->brother;



    if(iterNode -> value.ival==NOPT_PARAM_LIST){

        paramNode = iterNode->child->child;

        while (paramNode != NULL) {
            paramNode = paramNode->brother;
        }

        iterNode = iterNode->brother;
    }

    iterNode = iterNode->brother;


    if(iterNode->value.ival==NOPT_VAR_SECT){

        declNode = iterNode->child->child;

        while (declNode!=NULL) {

            int decl_type = declNode->child->brother->child->type;
            Pnode idNode = declNode->child->child;

            while(idNode != NULL){
                newTcode(decl_type);
                idNode = idNode->brother;
            }

            declNode = declNode->brother;
        }

        iterNode = iterNode->brother;
    }


    if(iterNode->value.ival==NOPT_CONST_SECT){

        Pnode constIst;
        constIst = constDeclNode = iterNode->child->child;

        //Qui creo il NEW con tipo
        while(constDeclNode != NULL) {

            int decl_type = constDeclNode->child->child->brother->child->type;
            Pnode idNode = constDeclNode->child->child->child;

            while(idNode != NULL){
                newTcode(decl_type);
                idNode = idNode->brother;
            }

            constDeclNode = constDeclNode->brother;
        }

        //Qui istanzio il codice relativo

        while(constIst != NULL) {

            Pnode idNode = constIst->child->child->child;
            Pnode expr = constIst->child->brother;

            while(idNode != NULL){
                instTypeOfExpr(expr, rootLine);
                idNode = idNode->brother;
            }

            constIst = constIst->brother;
        }


        iterNode = iterNode->brother;                                        //PUNTO A OPT-MODULE/MODULE BODY
    }


    if(iterNode->value.ival==NOPT_MODULE_LIST){

        modNode = iterNode->child;

        while (modNode != NULL) {

            /* .... */

//            PLine nextModule = findLineByIdAndClass(modNode->child->value.sval, rootLine->class, rootLine->bucket);
            PLine nextModule = findLineById(modNode->child->value.sval, rootLine);
            tCodeGenerator(nextModule, modNode);
            modNode = modNode->brother;
        }

        iterNode = iterNode->brother;
    }



    codeStatment(iterNode->child->brother->child, rootLine);
}

void codeStatment(Pnode stat, PLine moduleLine){

    while(stat!=NULL){ //punta a stat

        int statType = stat->child->value.ival;


        switch (statType){
            case NASSIGN_STAT: {

                Pnode assignStat = stat->child;
//                PLine id = findLineByIdFromPV(assignStat->child->value.sval, moduleLine->bucket);
                PLine id = findLineById(assignStat->child->value.sval, moduleLine);
                // ↑↑↑↑↑↑ forse bisogna controllare che siano esclusivamente PAR o VAR ↑↑↑↑↑↑
                instTypeOfExpr(assignStat->child->brother->child, moduleLine);
                bprintf( "STO %d %d\n",getLevelModule(id, moduleLine)==-1?0:getLevelModule(id, moduleLine), id->oid);
                break;
            }
            case NIF_STAT:{
                generateCodeIfStat(stat->child, moduleLine);
                break;
            }
            case NWHILE_STAT: {

//                controlOfStatment(stat->child->child->brother->child, moduleLine);
                break;


            }
            case NRETURN_STAT: {

//                Pnode returnStat = stat->child;
//                char* moduleType = moduleLine->type;
//                char* typeExpr;

                break;
            }
            case NREAD_STAT:{
                generateCodeRead(stat->child->child->child, moduleLine);
                break;
            }
            case NWRITE_STAT:{
                generateCodeWrite(stat->child->child->child, moduleLine);
                break;
            }
            case NMODULE_CALL: {
                PLine moduleCalled = findLineById(stat->child->child->child->value.sval, moduleLine);
                // ↑↑↑↑↑↑ forse bisogna controllare che sia esclusivamente MOD ↑↑↑↑↑↑
                bprintf("PUSH %d %d %d\n", moduleCalled->nFormalParams, getLocalObjectInModule(moduleCalled),
                        getGapModuleAmbient(moduleLine, moduleCalled));
                bprintf("\t GOTO %d\n", moduleCalled->oid);
                bprintf("POP\n");
//
                break;
            }
        }

        stat= stat->brother;
    }

}

void newTcode(int type){
    switch (type){
        case T_INT:
            bprintf("NEW |int|\n");
            break;
        case T_REAL:
            bprintf("NEW |float|\n");
            break;
        case T_BOOL:
            bprintf("NEW |int|\n");
            break;
        case T_CHAR:
            bprintf("NEW |char|\n");
            break;
        case T_STRING:
            bprintf("NEW |string|\n");
            break;
    }
}

int unaryOP;
void instTypeOfExpr(Pnode x_term, PLine moduleLine) { //expr punta x_term

    if(x_term!=NULL) {

        if (x_term->child == NULL) { // se siamo ai minimi termini
            switch (x_term->type) {
                case 6: {
                    //char const
                    bprintf( "LDC %s\n", x_term->value.sval);
                    break;
                }
                case 7: {
                    //int const
                    bprintf( "LDI %d\n", x_term->value.ival);
                    break;
                }
                case 8: {
                    //real const
                    bprintf( "LDR %f\n", x_term->value.rval);
                    break;
                }
                case 9: {
                    //string const
                    bprintf( "LDS %s\n", x_term->value.sval);
                    break;
                }
                case 10: {
                    //bool const
                    bprintf( "LDI %d\n", x_term->value.ival);
                    break;
                }
                case 11: {
                    PLine id = findLineById(x_term->value.sval, moduleLine);
                    bprintf( "LOD %d %d\n", getLevelModule(id, moduleLine)==-1?0:getLevelModule(id, moduleLine),id->oid);
                    break;
                }
            }
        } else { // passo ricorsivo

            if (x_term->child->value.ival != NFACTOR)
                instTypeOfExpr(x_term->child, moduleLine);
            else {
                if (x_term->child->child->type == 11)  // per gli id scendiamo di livello
                    instTypeOfExpr(x_term->child, moduleLine);
                else {
                    switch (x_term->child->child->value.ival) {
                        case NUNARYOP: {
                            instTypeOfExpr(x_term->child->child->brother, moduleLine);
                            switch (unaryOP) {
                                case 1: { //int
                                    bprintf( "IUMI\n");
                                    break;
                                }
                                case 2: { //real
                                    bprintf( "RUMI\n");

                                    break;
                                }
                                case 3: { //bool
                                    bprintf( "LNEG\n");

                                    break;
                                }
                            }
                            break;
                        }
                        case NMODULE_CALL: {
                            //                            generateCodeFormalParams(x_term->child->child->child->brother->child->child, moduleLine);
//                            PLine moduleCalled = findLineByIdAndClass(x_term->child->child->child->value.sval, "MOD",
//                                                                      moduleLine->bucket);
                            PLine moduleCalled = findLineById(x_term->child->child->child->value.sval, moduleLine);
                            // ↑↑↑↑↑↑ forse bisogna controllare che sia esclusivamente MOD ↑↑↑↑↑↑

//                            if(isChildOfCaller(moduleCalled, moduleLine)==1){
//                                printf("Figlio\n");
//                            }
//                            if(isFatherOfCaller(moduleCalled, moduleLine)==1){
//                                printf("Genitore\n");
//                            }
//                            printf("Gap: %d tra chiamato: %s e chiamante: %s\n", getGapModuleAmbient(moduleLine, moduleCalled), moduleCalled->id, moduleLine->id);
                            bprintf("PUSH %d %d %d\n", moduleCalled->nFormalParams,  getLocalObjectInModule(moduleCalled), getGapModuleAmbient(moduleLine, moduleCalled));
                            bprintf("\t GOTO %d\n", moduleCalled->oid );
                            bprintf("POP\n");
//
                            break;
                        }
                        case NEXPR:
                            instTypeOfExpr(x_term->child->child, moduleLine);
                            break;

                        case NCONSTANT: {
                            instTypeOfExpr(x_term->child, moduleLine);
                            break;
                        }
                        case NCOND_EXPR: {
                            generateCodeOFConditionalExpr(x_term->child->child->child, moduleLine);
                            break;
                        }
                        case NTYPE: {
                            int type = x_term->child->child->child->type;
                            generateCodeCast(x_term->child->child->brother, moduleLine, type);
                            break;
                        }
                    }
                }
            }

        }


        int typeOp;
        Pnode prox_term;

        if (x_term->brother != NULL){ // se c'è una parte destra bisogna controllare

            if (x_term->value.ival == 32) { // rel term è diverso per costruzione
                prox_term = x_term->brother->brother;
                typeOp = x_term->brother->child->type;
            } else {
                prox_term = x_term->brother->child->brother;
                typeOp = x_term->brother->child->child->type;
            }

            operationCode(prox_term, typeOp, moduleLine, getExprType(prox_term, moduleLine));
        }

    }

}

void generateCodeFormalParams(Pnode param, PLine moduleLine){

    while (param!=NULL){
        instTypeOfExpr(param, moduleLine);
        param = param->brother;
    }
}

void operationCode(Pnode x_term, int type, PLine moduleLine, char* typeExpr){

    switch (type){
        //logic op:
        case 24 : case 25:
            logicOperation(x_term, type, moduleLine);
            break;

        //rel op:
        case 18 : case 19: case 20: case 21: case 22: case 23:
            relOperation(x_term,type, moduleLine, typeExpr);
            break;

        //math op:
        case 13 : case 14: case 15: case 16:
            mathOperation(x_term,type, moduleLine, typeExpr);
            break;

    }

}

void mathOperation(Pnode term, int type,  PLine moduleLine, char* typeExpr){
    char *typeC;
    if (strcmp(typeExpr, "INT")==0)
        typeC = "I";
    else if (strcmp(typeExpr, "REAL")==0)
        typeC = "R";
    else {
        printf("Errore: mathoperation non riconosciuta");
        exit(-18);
    }

    instTypeOfExpr(term, moduleLine);

    char *opC=NULL;
    switch (type){
        case 13 : // *
            opC = "MUL";
            break;
        case 14:  // /
            opC = "DIV";
            break;
        case 15: // -
            opC = "SUB";
            break;
        case 16:  // +
            opC = "ADD";
            break;
    }

    bprintf("%s%s\n", typeC, opC);

}

void relOperation(Pnode term, int type, PLine moduleLine, char* typeExpr){

    switch (type){
        case 22: // !=
            instTypeOfExpr(term, moduleLine);
            bprintf("NEQ\n");
            break;
        case 23: // ==
            instTypeOfExpr(term, moduleLine);
            bprintf("EQU\n");
            break;
        case 18 : case 19: case 20: case 21:{
            relationOp(term, type, moduleLine, typeExpr);
            break;
        }
    }

}

void relationOp(Pnode term, int type, PLine moduleLine, char* typeExpr){
/**/
    char *topo=NULL;
    if(strcmp(typeExpr, "INT")==0  || strcmp(typeExpr, "BOOL")==0 )
        topo = "I";
    else if (strcmp(typeExpr, "CHAR")==0)
        topo = "C";
    else if(strcmp(typeExpr, "REAL")==0)
        topo = "R";
    else if(strcmp(typeExpr, "STRING")==0)
        topo = "S";
    else {
        printf("Errore relationOP: tipo non riconosciuto");
        exit(-17);
    }

    instTypeOfExpr(term, moduleLine);

    char *op=NULL;
    switch (type) {
        case 18: // <=
            op = "LE";
            break;
        case 19: // >=
            op = "GE";
            break;
        case 20: // <
            op = "LT";
            break;
        case 21: // >
            op = "GT";
            break;
        default:
            printf("Errore relationOP: operazione %d sconosciuta", type);
            exit(-17);
    }

    bprintf("%s%s\n", topo, op);
}

void logicOperation(Pnode term, int type, PLine moduleLine){

    switch (type){
        case 24:{ //AND

            int jmfBufferAddress = bufferSize;
            bprintf( "JMF temp\n");
            instTypeOfExpr(term->child, moduleLine);
            bprintf("JMP 2\n");
            bprintfAtIndex(jmfBufferAddress, "JMF %d\n", bufferSize-jmfBufferAddress);
            bprintf( "LDI 0\n");

            break;
        }
        case 25:{ //OR
            bprintf( "JMF 3\n");
            bprintf( "LDI 1\n");
            int jmpAddress = bufferSize;
            bprintf("JMP temp\n");
            instTypeOfExpr(term, moduleLine);
            bprintfAtIndex(jmpAddress, "JMP %d\n", bufferSize-jmpAddress);

            break;
        }
    }


}

ListNode *lineExitCondExpr = NULL;

void generateCodeOFConditionalExpr(Pnode condition, PLine moduleLine){



    Pnode ifNode = condition->child;
    Pnode thenNode = condition->brother->child ;


    instTypeOfExpr(ifNode, moduleLine);
    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");
    instTypeOfExpr(thenNode, moduleLine);

    Pnode elseNode;


    lineExitCondExpr = addIntNode(lineExitCondExpr, bufferSize);
    bprintf("JMP exit\n");

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    if(condition->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST) {

        generateCodeElseIfExpr(condition->brother->brother->child, moduleLine);

        condition = condition->brother;
    }
        elseNode = condition->brother->brother;
        instTypeOfExpr(elseNode, moduleLine);

    for (; lineExitCondExpr!=NULL; lineExitCondExpr=lineExitCondExpr->next) {
            bprintfAtIndex(lineExitCondExpr->value.ival, "JMP %d\n", bufferSize-lineExitCondExpr->value.ival);
    }


}

void generateCodeElseIfExpr(Pnode optExpr, PLine moduleLine){


    Pnode expr = optExpr->child;
    instTypeOfExpr(expr, moduleLine);
    Pnode then = optExpr->brother->child;

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    instTypeOfExpr(then, moduleLine);


    lineExitCondExpr = addIntNode(lineExitCondExpr, bufferSize);
    bprintf("JMP exit\n");

    if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);
        generateCodeElseIfExpr(optExpr->brother->brother->child, moduleLine);
    }
    else{
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    }

}

void generateCodeCast(Pnode cast, PLine moduleLine, int type){

    instTypeOfExpr(cast, moduleLine);

    switch (type){
        case T_INT:{
            bprintf("TOINT\n");
            break;
        }
        case T_REAL:{
            bprintf("TOREA\n");
            break;
        }
    }

}

void generateCodeRead(Pnode id, PLine moduleLine){

    while (id!=NULL){

        PLine idLine = findLineById(id->value.sval, moduleLine);

        int offsetEnv= getLevelModule(idLine, moduleLine)==-1?0:getLevelModule(idLine, moduleLine);
        char type = idLine->type[0];
            if(type>='A' && type<='Z'){
                type = type +32;
            }

        bprintf("READ %c %d %d\n", type,offsetEnv,idLine->oid);

        id = id->brother;
    }

}

ListNode *charListWrite = NULL;

void generateCodeWrite(Pnode expr, PLine moduleLine){

    while(expr!=NULL){

        char *type = getExprType(expr->child, moduleLine);

        charListWrite = addCharNode(charListWrite, type[0]);

        instTypeOfExpr(expr, moduleLine);
        expr = expr->brother;
    }

    ListNode *count = charListWrite;
    int num = 0;

    for (; count!=NULL; count=count->next) {
//        printf("Char: %c\n", count->value.cval);
        ++num;
    }
//    printf("num: %d ", num);

    char inverti[num];
    int i = num-1;

    while (charListWrite!=NULL ){
//        printf("i:%d ", i);
        inverti[i]=charListWrite->value.cval;
        charListWrite=charListWrite->next;
        --i;
    }


    char stampa[num];

    int j;
    for ( j= 0; j < num ; j++) {
//        printf("Char: %c\n", inverti[j]);
        stampa[j] = inverti[j];
    }
    stampa[j]='\0';

    int c=0;

    while(stampa[c]!='\0'){

        if(stampa[c]>='A' && stampa[c]<='Z'){
            stampa[c] = stampa[c] +32;
        }
        c++;
    }

    bprintf("WRITE \"%s\"\n", stampa);


}

int getLocalObjectInModule(PLine module){
    int n = 0;

    for (int i = 0; i < BUCKET_SIZE; ++i) {
        PLine line = module->bucket[i];

        while (line!=NULL){
            n++;
            line = line->next;
        }

    }

    return n;
}

ListNode *lineExitIfStat = NULL;

void generateCodeIfStat(Pnode if_stat, PLine moduleLine){

    Pnode expr = if_stat->child;
    instTypeOfExpr(expr, moduleLine);
    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    Pnode statList = expr->brother;
    codeStatment(statList->child, moduleLine);

    lineExitIfStat = addIntNode(lineExitIfStat, bufferSize);
    bprintf("JMP exit\n");

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);


    if( statList->brother!=NULL && statList->brother->value.ival==NOPT_ELSEIF_STAT_LIST) {

        generateCodeElseIfStat(if_stat->child->brother->brother->child, moduleLine);

        statList = statList->brother;
    }


    if(statList->brother!=NULL &&statList->brother->value.ival==NOPT_ELSE_STAT) {

        codeStatment(statList->brother->child,moduleLine);

    }

    for (; lineExitIfStat!=NULL; lineExitIfStat=lineExitIfStat->next) {
        bprintfAtIndex(lineExitIfStat->value.ival, "JMP %d\n", bufferSize-lineExitIfStat->value.ival);
    }


}

void generateCodeElseIfStat(Pnode optExpr, PLine moduleLine){


    Pnode expr = optExpr->child;
    instTypeOfExpr(expr, moduleLine);
    Pnode then = optExpr->brother->child;

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    codeStatment(then, moduleLine);


    lineExitIfStat = addIntNode(lineExitIfStat, bufferSize);
    bprintf("JMP exit\n");

    if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_STAT_LIST){
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);
        generateCodeElseIfStat(optExpr->brother->brother->child, moduleLine);
    }
    else{
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    }

}




