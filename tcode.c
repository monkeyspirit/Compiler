//
// Created by maria on 10/07/19.
//

#include "tcode.h"
#include "semantic.h"
#include "dynamicArray.h"

#include <stdarg.h>


char **buffer=NULL;
int bufferSize=0;
FILE *out;

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
ListNode *modListEntry=NULL;

void genTCode(PLine rootLine, Pnode root){
    wholeSymbolTable = rootLine;
    genModuleTCode(root, rootLine);
//    subsEntryPModuleCall();
    flush();
}

void subsEntryPModuleCall(){

    for (; modListEntry!=NULL; modListEntry=modListEntry->next) {
        int entryPoint = getEntryPMod(modListEntry->value.modOid);
        if(entryPoint==-1){
            printf("ERROR");
        }
        //bprintfAtIndex(modListEntry->value.ival, "GOTO %d\n", entryPoint);
    }
}

int getEntryPMod(int oid){

//
//
//    for(int i=0; i<bufferSize; i++){
//        if(strcmp(buffer[i], stringa)==0){
//            return i;
//        }
//    }

    return -1;
}


void genModuleTCode(Pnode moduleNode, PLine fatherModuleLine){

    bprintf( "MOD %d\n", fatherModuleLine->oid);

//    if(rootLine!=wholeSymbolTable){
//        printf("Id: %s Father: %s\n", rootLine->id, getFather(rootLine, wholeSymbolTable)->id);
//    }
//
//    printf("Livello di %s: %d\n", rootLine->id, getLevelModule(rootLine, wholeSymbolTable));
//

    Pnode iterNode, paramNode, declNode, constDeclNode, modNode;
    iterNode = moduleNode->child; // punta a ID
    char* idMod = iterNode->value.sval;

    iterNode = iterNode->brother;



    if(iterNode -> value.ival==NOPT_PARAM_LIST){
        /*
        paramNode = iterNode->child->child;

        while (paramNode != NULL) {
            paramNode = paramNode->brother;
        }
        */
//        Non so seci va ----- > genModuleParamsTCode(iterNode->child, fatherModuleLine);

        iterNode = iterNode->brother;
    }

    iterNode = iterNode->brother;


    if(iterNode->value.ival==NOPT_VAR_SECT){

        declNode = iterNode->child->child;

        while (declNode!=NULL) {

            int decl_type = declNode->child->brother->child->type;
            Pnode idNode = declNode->child->child;

            while(idNode != NULL){
                genNewTCode(decl_type);
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
                genNewTCode(decl_type);
                idNode = idNode->brother;
            }

            constDeclNode = constDeclNode->brother;
        }

        //Qui istanzio il codice relativo

        while(constIst != NULL) {

            Pnode idNode = constIst->child->child->child;
            Pnode expr = constIst->child->brother;

            while(idNode != NULL){
                genExprTCode(expr, fatherModuleLine);
                PLine idLine = findLineById(idNode->value.sval, fatherModuleLine);
                bprintf( "STO %d %d\n",getLevelModule(idLine, fatherModuleLine)==-1?0:getLevelModule(idLine, fatherModuleLine), idLine->oid);
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
            PLine nextModule = findLineById(modNode->child->value.sval, fatherModuleLine);
            genModuleTCode(modNode, nextModule);
            modNode = modNode->brother;
        }

        iterNode = iterNode->brother;
    }

    genStatListTCode(iterNode->child->brother, fatherModuleLine);
}

void genStatListTCode(Pnode statListNode, PLine fatherModuleLine){

    Pnode statNode = statListNode->child;
    while(statNode!=NULL){ //punta a stat

        int statType = statNode->child->value.ival;

        switch (statType){
            case NASSIGN_STAT: {
                Pnode assignStat = statNode->child;
//                PLine id = findLineByIdFromPV(assignStat->child->value.sval, moduleLine->bucket);
                PLine id = findLineById(assignStat->child->value.sval, fatherModuleLine);
                // ↑↑↑↑↑↑ forse bisogna controllare che siano esclusivamente PAR o VAR ↑↑↑↑↑↑
                genExprTCode(assignStat->child->brother->child, fatherModuleLine);
                bprintf( "STO %d %d\n",getLevelModule(id, fatherModuleLine)==-1?0:getLevelModule(id, fatherModuleLine), id->oid);
                break;
            }
            case NIF_STAT:{
                genIfStatTCode(statNode->child, fatherModuleLine);
                break;
            }
            case NWHILE_STAT: {
                genWhileStatTCode(statNode->child, fatherModuleLine);
                break;
            }
            case NRETURN_STAT: {
                genReturnStatTCode(statNode->child, fatherModuleLine);
//                Pnode returnStat = stat->child;
//                char* moduleType = moduleLine->type;
//                char* typeExpr;

                break;
            }
            case NREAD_STAT:{
                genReadStatTCode(statNode->child, fatherModuleLine);
                break;
            }
            case NWRITE_STAT:{
                genWriteStatTCode(statNode->child, fatherModuleLine);
                break;
            }
            case NMODULE_CALL: {
                PLine moduleCalled = findLineById(statNode->child->child->value.sval, fatherModuleLine);
                // ↑↑↑↑↑↑ forse bisogna controllare che sia esclusivamente MOD ↑↑↑↑↑↑
                genModuleExprTCode(statNode->child, fatherModuleLine);
                bprintf("PUSH %d %d %d\n", moduleCalled->nFormalParams, countModuleBucketLines(moduleCalled),
                        getGapModuleAmbient(fatherModuleLine, moduleCalled));
                modListEntry = addModEntryNode(modListEntry, bufferSize, moduleCalled->oid);
                bprintf("\t GOTO temp\n");
                bprintf("POP\n");
                break;
            }
        }

        statNode= statNode->brother;
    }

}

void genNewTCode(int type){
    switch (type){
        case T_INT:
            bprintf("NEW %d\n", sizeof(int));
            break;
        case T_REAL:
            bprintf("NEW %d\n", sizeof(float));
            break;
        case T_BOOL:
            bprintf("NEW %d\n", sizeof(int));
            break;
        case T_CHAR:
            bprintf("NEW %d\n", sizeof(char));
            break;
        case T_STRING:
            bprintf("NEW %d\n", sizeof(char*));
            break;
    }
}

int countModuleBucketLines(PLine module){
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

int unaryOP;

void genExprTCode(Pnode x_term, PLine moduleLine) { //expr punta x_term

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
                genExprTCode(x_term->child, moduleLine);
            else {
                if (x_term->child->child->type == 11)  // per gli id scendiamo di livello
                    genExprTCode(x_term->child, moduleLine);
                else {
                    switch (x_term->child->child->value.ival) {
                        case NUNARYOP: {
                            genExprTCode(x_term->child->child->brother, moduleLine);
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
                             PLine moduleCalled = findLineById(x_term->child->child->child->value.sval, moduleLine);
                            // ↑↑↑↑↑↑ forse bisogna controllare che sia esclusivamente MOD ↑↑↑↑↑↑

                            genModuleExprTCode(x_term->child->child, moduleLine);

                            bprintf("PUSH %d %d %d\n", moduleCalled->nFormalParams, countModuleBucketLines(moduleCalled), getGapModuleAmbient(moduleLine, moduleCalled));
                            modListEntry = addModEntryNode(modListEntry, bufferSize, moduleCalled->oid);
                            bprintf("\t GOTO temp\n");
                            bprintf("POP\n");
//
                            break;
                        }
                        case NEXPR:
                            genExprTCode(x_term->child->child, moduleLine);
                            break;

                        case NCONSTANT: {
                            genExprTCode(x_term->child, moduleLine);
                            break;
                        }
                        case NCOND_EXPR: {
                            genCondExprTCode(x_term->child->child->child, moduleLine);
                            break;
                        }
                        case NTYPE: {
                            int type = x_term->child->child->child->type;
                            genCastTCode(x_term->child->child->brother, moduleLine, type);
                            break;
                        }
                    }
                }
            }

        }


        int typeOp;
        Pnode prox_term;

        if (x_term->brother != NULL && x_term->value.ival != NEXPR){ // se c'è una parte destra bisogna controllare

            if (x_term->value.ival == 32) { // rel term è diverso per costruzione
                prox_term = x_term->brother->brother;
                typeOp = x_term->brother->child->type;
            } else {
                prox_term = x_term->brother->child->brother;
                typeOp = x_term->brother->child->child->type;
            }

            genOperationTCode(prox_term, typeOp, moduleLine, getExprType(prox_term, moduleLine));
        }

    }

}

void instTypeOfExpr2(Pnode x_term, PLine fatherModuleLine) {
    if(x_term->child == NULL){
        switch (x_term->type){
            case T_CHARCONST:
                bprintf("LDC %s\n", x_term->value.sval);
                break;
            case T_INTCONST:
                bprintf("LDI %s\n", x_term->value.sval);
                break;
            case T_REALCONST:
                bprintf("LDR %s\n", x_term->value.sval);
                break;
            case T_STRCONST:
                bprintf("LDS %s\n", x_term->value.sval);
                break;
            case T_BOOLCONST:
                bprintf("LDI %s\n", x_term->value.sval);
                break;
            case T_ID: {
                PLine id = findLineById(x_term->value.sval, fatherModuleLine);
                bprintf( "LOD %d %d\n", getLevelModule(id, fatherModuleLine)==-1?0:getLevelModule(id, fatherModuleLine),id->oid);
            }
                break;
        }
    } else {
        switch (x_term->value.ival) {
            case NUNARYOP:
                genExprTCode(x_term->child->brother, fatherModuleLine);
                switch (unaryOP) {
                    case 1:  //int
                        bprintf( "IUMI\n");
                        break;
                    case 2:  //real
                        bprintf( "RUMI\n");
                        break;
                    case 3:  //bool
                        bprintf( "LNEG\n");
                        break;
                    default:
                        exit(-523);
                }
                break;
            case NCOND_EXPR:
                genExprTCode(x_term->child, fatherModuleLine);
                break;
            case NTYPE: {
                int type = x_term->child->type;
                genCastTCode(x_term->brother, fatherModuleLine, type);
                break;
            }
            case NMODULE_CALL:

                break;
            default:
                genExprTCode(x_term->child, fatherModuleLine);
        }


        int typeOp;
        Pnode prox_term;

        if (x_term->brother != NULL){ // se c'è una parte destra bisogna controllare
            switch (x_term->brother->value.ival) {
                case NREL_TERM1:
                case NLOW_TERM1:
                case NEXPR1:
                    typeOp = x_term->brother->child->child->type;
                    prox_term = x_term->brother->child->brother;
                    break;
                case NRELOP:
                    typeOp = x_term->brother->child->type;
                    prox_term = x_term->brother->brother;
                    break;
                default:
                    printf("Errore nella parte destra dell'operazione %d", x_term->brother->value.ival);
                    exit(-189);
            }

            genOperationTCode(prox_term, typeOp, fatherModuleLine, getExprType(prox_term, fatherModuleLine));
        }
    }
}

void genModuleExprTCode(Pnode moduleCallStat, PLine moduleLine){

    if(moduleCallStat->child->brother!=NULL){

        Pnode expr = moduleCallStat->child->brother->child->child;

        while(expr!=NULL){
            genExprTCode(expr, moduleLine);
            expr=expr->brother;
        }

    }
}

void genOperationTCode(Pnode x_term, int type, PLine moduleLine, char *typeExpr){

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

    genExprTCode(term, moduleLine);

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
            genExprTCode(term, moduleLine);
            bprintf("NEQ\n");
            break;
        case 23: // ==
            genExprTCode(term, moduleLine);
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

    genExprTCode(term, moduleLine);

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
            genExprTCode(term->child, moduleLine);
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
            genExprTCode(term, moduleLine);
            bprintfAtIndex(jmpAddress, "JMP %d\n", bufferSize-jmpAddress);

            break;
        }
    }


}

ListNode *lineExitCondExpr = NULL;

void genCondExprTCode(Pnode condition, PLine moduleLine){

    Pnode ifNode = condition->child;
    Pnode thenNode = condition->brother->child ;


    genExprTCode(ifNode, moduleLine);
    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");
    genExprTCode(thenNode, moduleLine);

    Pnode elseNode;


    lineExitCondExpr = addIntNode(lineExitCondExpr, bufferSize);
    bprintf("JMP exit\n");

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    if(condition->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST) {

        genElseIfExprTCode(condition->brother->brother->child, moduleLine);

        condition = condition->brother;
    }
        elseNode = condition->brother->brother;
    genExprTCode(elseNode, moduleLine);

    for (; lineExitCondExpr!=NULL; lineExitCondExpr=lineExitCondExpr->next) {
            bprintfAtIndex(lineExitCondExpr->value.ival, "JMP %d\n", bufferSize-lineExitCondExpr->value.ival);
    }


}

void genElseIfExprTCode(Pnode optExpr, PLine moduleLine){


    Pnode expr = optExpr->child;
    genExprTCode(expr, moduleLine);
    Pnode then = optExpr->brother->child;

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    genExprTCode(then, moduleLine);


    lineExitCondExpr = addIntNode(lineExitCondExpr, bufferSize);
    bprintf("JMP exit\n");

    if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);
        genElseIfExprTCode(optExpr->brother->brother->child, moduleLine);
    }
    else{
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    }

}

void genCastTCode(Pnode cast, PLine moduleLine, int type){

    genExprTCode(cast, moduleLine);

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

void genReadStatTCode(Pnode readStatNode, PLine moduleLine){

    Pnode id = readStatNode->child->child;
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
void genWriteStatTCode(Pnode writeStatNode, PLine moduleLine){

    Pnode expr = writeStatNode->child->child;
    while(expr!=NULL){

        char *type = getExprType(expr->child, moduleLine);

        charListWrite = addCharNode(charListWrite, type[0]);

        genExprTCode(expr, moduleLine);
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

ListNode *lineExitIfStat = NULL;

void genIfStatTCode(Pnode if_stat, PLine moduleLine){

    Pnode expr = if_stat->child;
    genExprTCode(expr, moduleLine);
    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    Pnode statList = expr->brother;
    genStatListTCode(statList, moduleLine);

    lineExitIfStat = addIntNode(lineExitIfStat, bufferSize);

    bprintf("JMP exit\n");

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    if( statList->brother!=NULL) {
        if (statList->brother->value.ival == NOPT_ELSEIF_STAT_LIST) {

            genElseIfStatTCode(statList->brother, moduleLine);

            statList = statList->brother;
        }
        if (statList->brother != NULL && statList->brother->value.ival == NOPT_ELSE_STAT) {

            genStatListTCode(statList->brother, moduleLine);

        }
    }
    for (; lineExitIfStat!=NULL; lineExitIfStat=lineExitIfStat->next) {
        bprintfAtIndex(lineExitIfStat->value.ival, "JMP %d\n", bufferSize-lineExitIfStat->value.ival);
    }
}

void genElseIfStatTCode(Pnode optElseStatNode, PLine moduleLine){

    Pnode expr = optElseStatNode->child;
    genExprTCode(expr, moduleLine);
    Pnode then = optElseStatNode->child->brother;

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    genStatListTCode(then, moduleLine);

    lineExitIfStat = addIntNode(lineExitIfStat, bufferSize);
    bprintf("JMP exit\n");

    if(then->brother!=NULL && then->brother->value.ival==NOPT_ELSEIF_STAT_LIST){
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);
        genElseIfStatTCode(then->brother, moduleLine);
    }
    else
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

}

void genWhileStatTCode(Pnode whileStat, PLine moduleLine){

    Pnode expr = whileStat->child;

    int jmpUp = bufferSize;

    genExprTCode(expr, moduleLine);

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    Pnode statList = expr->brother;
    genStatListTCode(statList, moduleLine);

    bprintf("JMP %d\n", bufferSize-jmpUp);

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);


}

void genReturnStatTCode(Pnode returnStat, PLine moduleLine){

    if(returnStat->child!=NULL){
        Pnode expr = returnStat->child->child;
        genExprTCode(expr, moduleLine);

    }

    bprintf("RETURN\n");
}