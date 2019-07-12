//
// Created by maria on 10/07/19.
//

#include "tcode.h"
#include "semantic.h"
#include "counter.h"

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

//                Pnode ifStat = stat->child;
//
//
//                Pnode statList = ifStat->child->brother;
//                controlOfStatment(statList->child, moduleLine);
//
//                if(statList->brother!=NULL &&  statList->brother->value.ival==NOPT_ELSEIF_STAT_LIST){
//                    controlOptElseIfStatList(statList->brother, moduleLine);
//                    statList=statList->brother;
//                }
//
//
//                if(statList->brother!=NULL && statList->brother->value.ival==NOPT_ELSE_STAT){
//
//                    controlOptElseStat(statList->brother, moduleLine);
//                }
//

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
                break;
            }
            case NWRITE_STAT:{
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
                            printf("Gap: %d tra chiamato: %s e chiamante: %s\n", getGapModuleAmbient(moduleLine, moduleCalled), moduleCalled->id, moduleLine->id);
                            bprintf( "PUSH %d -oggetti- %d\n\t GOTO %d\nPOP\n", moduleCalled->nFormalParams, getGapModuleAmbient(moduleLine, moduleCalled),
                                    moduleCalled->oid);
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
/*
        if (x_term->child != NULL) {
            if (x_term->child->value.ival == NFACTOR) {
                if (x_term->child->child->type == 11) {
                    instTypeOfExpr(x_term->child, moduleLine);
                } else {
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
                            printf("Gap: %d tra chiamato: %s e chiamante: %s\n", getGapModuleAmbient(moduleLine, moduleCalled), moduleCalled->id, moduleLine->id);
                            bprintf( "PUSH %d -oggetti- %d\n\t GOTO %d\nPOP\n", moduleCalled->nFormalParams, getGapModuleAmbient(moduleLine, moduleCalled),
                                    moduleCalled->oid);
                            break;
                        }
                        case NEXPR: {
                            instTypeOfExpr(x_term->child->child, moduleLine);
                            break;
                        }
                        case NCONSTANT: {
                            instTypeOfExpr(x_term->child, moduleLine);
                            break;
                        }
                        case NCOND_EXPR: {
                            generateCodeOFConditionalExpr(x_term->child->child, moduleLine);
                            break;
                        }
                        case NTYPE: {

                            break;
                        }
                    }
                }
            } else {
                instTypeOfExpr(x_term->child, moduleLine);
            }
        } else {
            switch (x_term->type) {
                case 6: {
//char const
                    bprintf( "LDC %s\n", x_term->value.sval);

                    break;
                }
                case 7: {
//int const
                    bprintf( "LDI %d\n", x_term->value.ival);

                    unaryOP = 1;
                    break;
                }
                case 8: {
//real const
                    bprintf( "LDR %f\n", x_term->value.rval);

                    unaryOP = 2;
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

                    unaryOP = 3;
                    break;
                }
                case 11: {
                    PLine id = findLineById(x_term->value.sval, moduleLine);

//faccio riferimento a quello che ho già caricato
                    bprintf( "LOD %d %d\n", getLevelModule(id, moduleLine)==-1?0:getLevelModule(id, moduleLine),id->oid);
                    break;
                }
            }

        }

        int typeOp;
        Pnode prox_term;
        if (x_term->brother != NULL) {

            if (x_term->value.ival == 32) {//rel term è diverso per costruzione
                prox_term = x_term->brother->brother;
                typeOp = x_term->brother->child->type;
            } else {
                prox_term = x_term->brother->child->brother;
                typeOp = x_term->brother->child->child->type;
            }


            operationCode(prox_term, typeOp, moduleLine, getExprType(prox_term, moduleLine));

        }

*/
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
            bprintf("EQU\n");
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

void generateCodeOFConditionalExpr(Pnode condition, PLine moduleLine){

    Pnode ifNode = condition->child;
    Pnode thenNode = condition->brother ;


    instTypeOfExpr(ifNode, moduleLine);

    int next = 2 + numberOfLinesExpr(thenNode);

    bprintf( "JMF %d\n", next);
    instTypeOfExpr(thenNode, moduleLine);

    if(condition->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){

        generateCodeElseIfExpr(condition->brother->brother->child, moduleLine);

        condition = condition->brother;
    }

    Pnode elseNode = condition->brother->brother;

    int exit = 1 + numberOfLinesExpr(elseNode->child);
    bprintf("JMP %d\n", exit);
    instTypeOfExpr(elseNode, moduleLine);

}

void generateCodeElseIfExpr(Pnode optExpr, PLine moduleLine){
    bool opt = false;

   Pnode elseIf = optExpr->child;

  /*  if(strcmp(typeExprELSEIF, "BOOL") != 0 ){
        printf("Errore la condizione dell'Else If deve essere di tipo BOOL invece è %s\n", typeExprELSEIF);
        exit(-9);
    }
    else{

        char *typeExprThen = getExprType(optExpr->brother->child, fatherModuleLine);

        char* typeExprOpt=NULL;

        if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
            typeExprOpt = getOptElseIfExprType(optExpr->brother->brother->child, fatherModuleLine);
            opt =true;

        }

        if(opt){
            if( strcmp(typeExprThen,  typeExprOpt)==0 ){
                return typeExprThen;
            }
            else{
                printf("Errore nella condizione Else If: ... then ...[%s] elseif ...[%s]  end\n", typeExprThen,typeExprOpt );
                exit(-9);
            }
        }
        else{
            return typeExprThen;
        }
    }*/
}