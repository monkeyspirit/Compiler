//
// Created by maria on 05/07/19.


#include "semantic.h"
#include "symbolTable.h"



void semanticControl(PLine rootLine, Pnode root){


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

                idNode = idNode->brother;
            }

            declNode = declNode->brother;
        }

        iterNode = iterNode->brother;
    }


    if(iterNode->value.ival==NOPT_CONST_SECT){

        constDeclNode = iterNode->child->child;

        while(constDeclNode != NULL) {

            int decl_type = constDeclNode->child->child->brother->child->type;
            Pnode idNode = constDeclNode->child->child->child;
            Pnode expr = constDeclNode->child->brother;
            constantDeclaration(decl_type, idNode->value.sval, expr);

            while(idNode != NULL){

                idNode = idNode->brother;
            }

            constDeclNode = constDeclNode->brother;
        }

        iterNode = iterNode->brother;                                        //PUNTO A OPT-MODULE/MODULE BODY
    }


    if(iterNode->value.ival==NOPT_MODULE_LIST){

        modNode = iterNode->child;

        while (modNode != NULL) {

            PLine nextModule = findLineByIdAndClass(modNode->child->value.sval, rootLine->class, rootLine->bucket);
            semanticControl(nextModule,modNode);
            modNode = modNode->brother;
        }

        iterNode = iterNode->brother;
    }

    moduleNameControl(iterNode->child->value.sval, iterNode->child->brother->brother->value.sval, idMod);

    // controllo negli stament

    controlOfStatment(iterNode, paramNode, rootLine);

}

PLine findLineByIdAndClass(char* id, char* class, PLine *table){

    PLine next;
    int i = 0;
    while(table[i]!=NULL){

        if(table[i]->class==class && table[i]->id==id){
            return table[i];
        }
        next = table[i]->next;
        printf("id:%s\n", table[i]->id);
        while (next!=NULL){
            printf("id:%s\n", next->id);
            if(next->class==class && next->id==id){
                return next;
            }

            next = next->next;

        }


       i++;
    }
    printf("Errore la riga non è presente, ma dovrebbe");
    exit(-5);
}

void controlOfStatment(Pnode moduleBody, Pnode paramNode, PLine moduleLine){

    Pnode stat = moduleBody->child->brother->child;


    while(stat!=NULL){

        int statType = stat->child->value.ival;

        switch (statType){
            case NASSIGN_STAT:
                break;
            case NMODULE_CALL:

                //controlFormalPar(paramNode, findLineByIdAndClass(stat->child->child->value.sval, moduleLine->class, moduleLine->bucket));
                break;
            case NIF_STAT:
                break;
            case NWHILE_STAT:
                break;
            case NRETURN_STAT:
                break;
            case NREAD_STAT:
                break;
            case NWRITE_STAT:
                break;


        }

        stat= stat->brother;
    }

}

/*
 * Controlli:
    1. Uguaglianza del nome del modulo con i nomi che delimitano il corpo del modulo
    2. Definizione delle costanti nella sezione const: compatibilità del valore con il tipo dichiarato
    3. Visibilità degli identificatori referenziati (nella gerarchia degli ambienti)
    4. Compatibilità in numero e tipo dei parametri attuali con i parametri formali
    5. Compatibilità delle espressioni di ritorno con il tipo del valore di ritorno del modulo
    6. Istruzione di return espressa sempre nel modulo che restituisce un valore diverso da void
    7. Compatibilità degli operatori con gli operandi
    8. Compatibilità dell'identificatore con l'espressione di assegnamento
    9. Compatibilità delle espressioni con le istruzioni in cui sono coinvolte
 * Type inference: computazione del tipo risultante di ogni operazione
 */

//1.
void moduleNameControl(char* h, char* q, char* id){
    if(strcmp(h,q)){
        printf("Errore gli ID non corrispondono: \"begin %s ... end %s\"", h, q);
        exit(-2);
    }
    else{
        if(strcmp(h, id)){
            printf("Errore gli ID non corrispondono: \"module %s ... begin %s\"",id, h);
            exit(-2);
        }
        else if(strcmp(q,id)){
            printf("Errore gli ID non corrispondono: \"module %s ... end %s\"", id, q);
            exit(-2);
        }
    }
}

//2.
void constantDeclaration(int type, char*id, Pnode expr){

    while(expr->child!=NULL){
        expr=expr->child;
    }

    switch (type){
        case T_CHAR:
            if(expr->type!=T_CHARCONST  && expr->type!=T_ID ){
                printf("Errore: id \"%s\" ci si aspetta un CHAR\n", id);
                exit(-3);
            }
            break;
        case T_INT:
            if(expr->type!=T_INTCONST){
                printf("Errore: id \"%s\" ci si aspetta un INT\n", id);
                exit(-3);
            }
            break;
        case T_REAL:
            if(expr->type!=T_REALCONST){
                printf("Errore: id \"%s\" ci si aspetta un REAL\n", id);
                exit(-3);
            }
            break;
        case T_STRING:
            if(expr->type!=T_STRCONST){
                printf("Errore: id \"%s\" ci si aspetta un STRING\n", id);
                exit(-3);
            }
            break;
        case T_BOOL:
            if(expr->type!=T_BOOLCONST){
                printf("Errore: id \"%s\" ci si aspetta un BOOL\n",id);
                exit(-3);
            }
            break;
    }

}


//3. Visibilità degli identificatori referenziati (nella gerarchia degli ambienti)

//4. Compatibilità in numero e tipo dei parametri attuali con i parametri formali
void controlFormalPar(Pnode paramNode, PLine rootLine){
    int numPar = 0;
    Pnode paramNumberNode, typeParamNode;
    paramNumberNode = typeParamNode = paramNode;


    while(paramNumberNode!=NULL){
        ++numPar;
        paramNumberNode= paramNumberNode->brother;
    }

    int types[numPar];
    int i = 0;
    while(typeParamNode!=NULL){
        types[i]= typeParamNode->child->brother->child->type;
        typeParamNode=typeParamNode->brother;
        i++;
    }

    printf("%d %d",rootLine->numParam,numPar );

    if(rootLine->numParam!=numPar){
        printf("Errore: nel modulo %s il numero di parametri formali non corrisponde a quello dichiarato",rootLine->id);
        exit(-6);
    }

    // controllo sui tipi

}

//5. Compatibilità delle espressioni di ritorno con il tipo del valore di ritorno del modulo

//6. Istruzione di return espressa sempre nel modulo che restituisce un valore diverso da void

//7. Compatibilità degli operatori con gli operandi

//8. Compatibilità dell'identificatore con l'espressione di assegnamento

//9. Compatibilità delle espressioni con le istruzioni in cui sono coinvolte
