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

ListNode *modListEntry=NULL;
ListNode *lineExitCondExpr = NULL;

// ---- Metodi di Utilità ----

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

int getEntryPMod(int oid){

    char stringa[20];

    sprintf(stringa, "MOD %d\n", oid);
    for(int i=0; i<bufferSize; i++){
        if(strcmp(buffer[i], stringa)==0){
            return i;
        }
    }

    return -1;
}

void subsEntryPModuleCall(){

    for (; modListEntry!=NULL; modListEntry=modListEntry->next) {
        int entryPoint = getEntryPMod(modListEntry->modOid);
        if(entryPoint==-1){
            printf("ERROR");
        }
        bprintfAtIndex(modListEntry->value.ival, "\t GOTO %d\n", entryPoint);
    }
}



int countModuleBucketLines(PLine moduleLine){
    int n = 0;

    for (int i = 0; i < BUCKET_SIZE; ++i) {
        PLine line = moduleLine->bucket[i];

        while (line!=NULL){
            n++;
            line = line->next;
        }

    }

    return n;
}

// ---- Metodi per la generazione di exprs ----

void mathOperation(Pnode term, int type,  PLine fatherModuleLine, char* typeExpr){
    char *typeC;
    if (strcmp(typeExpr, "INT")==0)
        typeC = "I";
    else if (strcmp(typeExpr, "REAL")==0)
        typeC = "R";
    else {
        printf("Errore: mathoperation non riconosciuta");
        exit(-18);
    }

    genExprTCode(term, fatherModuleLine);

    char *opC=NULL;
    switch (type) {
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

void relationOp(Pnode term, int type, PLine fatherModuleLine, char* typeExpr){
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

    genExprTCode(term, fatherModuleLine);

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

void relOperation(Pnode term, int type, PLine fatherModuleLine, char* typeExpr){

    switch (type){
        case 22: // !=
            genExprTCode(term, fatherModuleLine);
            bprintf("NEQ\n");
            break;
        case 23: // ==
            genExprTCode(term, fatherModuleLine);
            bprintf("EQU\n");
            break;
        case 18 : case 19: case 20: case 21:{
            relationOp(term, type, fatherModuleLine, typeExpr);
            break;
        }
    }

}

void logicOperation(Pnode term, int type, PLine fatherModuleLine){

    switch (type){
        case 24:{ //AND

            int jmfBufferAddress = bufferSize;
            bprintf( "JMF temp\n");
            genExprTCode(term->child, fatherModuleLine);
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
            genExprTCode(term, fatherModuleLine);
            bprintfAtIndex(jmpAddress, "JMP %d\n", bufferSize-jmpAddress);

            break;
        }
    }


}

void genOperationTCode(Pnode expr2Node, int type, PLine fatherModuleLine){
    char *typeExpr = getExprType(expr2Node, fatherModuleLine);
    switch (type){
        //logic op:
        case 24 : case 25:
            logicOperation(expr2Node, type, fatherModuleLine);
            break;

            //rel op:
        case 18 : case 19: case 20: case 21: case 22: case 23:
            relOperation(expr2Node,type, fatherModuleLine, typeExpr);
            break;

            //math op:
        case 13 : case 14: case 15: case 16:
            mathOperation(expr2Node,type, fatherModuleLine, typeExpr);
            break;

    }

}

void genElseIfExprTCode(Pnode optExpr, PLine fatherModuleLine){


    Pnode expr = optExpr->child;
    genExprTCode(expr, fatherModuleLine);
    Pnode then = optExpr->brother->child;

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    genExprTCode(then, fatherModuleLine);


    lineExitCondExpr = addIntNode(lineExitCondExpr, bufferSize);
    bprintf("JMP exit\n");

    if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);
        genElseIfExprTCode(optExpr->brother->brother->child, fatherModuleLine);
    }
    else{
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    }

}

void genCondExprTCode(Pnode condition, PLine fatherModuleLine){

    Pnode ifNode = condition->child;
    Pnode thenNode = condition->brother->child ;


    genExprTCode(ifNode, fatherModuleLine);
    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");
    genExprTCode(thenNode, fatherModuleLine);

    Pnode elseNode;


    lineExitCondExpr = addIntNode(lineExitCondExpr, bufferSize);
    bprintf("JMP exit\n");

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    if(condition->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST) {

        genElseIfExprTCode(condition->brother->brother->child, fatherModuleLine);

        condition = condition->brother;
    }
    elseNode = condition->brother->brother;
    genExprTCode(elseNode, fatherModuleLine);

    for (; lineExitCondExpr!=NULL; lineExitCondExpr=lineExitCondExpr->next) {
        bprintfAtIndex(lineExitCondExpr->value.ival, "JMP %d\n", bufferSize-lineExitCondExpr->value.ival);
    }


}

void genModuleExprTCode(Pnode moduleCallStat, PLine fatherModuleLine){

    if(moduleCallStat->child->brother!=NULL){

        Pnode expr = moduleCallStat->child->brother->child->child;

        while(expr!=NULL){
            genExprTCode(expr, fatherModuleLine);
            expr=expr->brother;
        }

    }
}

void genExprTCode(Pnode exprNode, PLine fatherModuleLine) { //expr punta x_term

    if(exprNode->child == NULL) { // siamo a un nodo foglia
        switch (exprNode->type) {
            case T_CHARCONST:
                bprintf("LDC %s\n", exprNode->value.sval);
                break;
            case T_INTCONST:
                bprintf("LDI %d\n", exprNode->value.ival);
                break;
            case T_REALCONST:
                bprintf("LDR %f\n", exprNode->value.rval);
                break;
            case T_STRCONST:
                bprintf("LDS %s\n", exprNode->value.sval);
                break;
            case T_BOOLCONST:
                bprintf("LDI %d\n", exprNode->value.ival);
                break;
            case T_ID:{
                PLine id = findLineById(exprNode->value.sval, fatherModuleLine);
                bprintf( "LOD %d %d\n", getLevelModule(id, fatherModuleLine)==-1?0:getLevelModule(id, fatherModuleLine),id->oid);
            }
                break;


        }
    } else { // non siamo a un nodo foglia
        switch (exprNode->value.ival) {
            case NUNARYOP:
                genExprTCode(exprNode->brother, fatherModuleLine);
                char* type = getExprType(exprNode->brother, fatherModuleLine);
                if (strcmp(type, "INT"))
                    bprintf( "IUMI\n");
                else if (strcmp(type, "REAL"))
                    bprintf( "RUMI\n");
                else if (strcmp(type, "BOOL"))
                    bprintf( "LNEG\n");
                else {
                    printf("Errore grave generazione operatore unario...");
                    exit(-1234);
                }
                return;
            case NTYPE:
                genExprTCode(exprNode->brother, fatherModuleLine);

                switch (exprNode->child->type){
                    case T_INT:
                        bprintf("TOINT\n");
                        break;
                    case T_REAL:
                        bprintf("TOREA\n");
                        break;
                    default:
                        printf("Errore generazione cast");
                        exit(-1234);
                }

                return;
            case NCOND_EXPR:
                genCondExprTCode(exprNode->child, fatherModuleLine);
                break;
            case NMODULE_CALL: {
//                  generateCodeFormalParams(x_term->child->child->child->brother->child->child, moduleLine);

                PLine moduleCalled = findLineById(exprNode->child->value.sval, fatherModuleLine);

                bprintf("PUSH %d %d %d\n", moduleCalled->nFormalParams, countModuleBucketLines(moduleCalled), getGapModuleAmbient(fatherModuleLine, moduleCalled));
                modListEntry = addModEntryNode(modListEntry, bufferSize, moduleCalled->oid);
                bprintf("\t GOTO %d\n", moduleCalled->oid );
                bprintf("POP\n");
            }
                break;
                printf("MODULE_CALL ANCORA DA IMPLEMENTARE");
                exit(-1);
                break;
            default:
                genExprTCode(exprNode->child, fatherModuleLine);
        }

    }

    if(exprNode->brother != NULL) { // se c'è una parte destra bisogna controllare

        Pnode prox_term;
        int typeOp;
        switch (exprNode->brother->value.ival) {
            case NSTAT_LIST:
            case NEXPR:
                return;
            case NREL_TERM1:
            case NLOW_TERM1:
            case NEXPR1:
                typeOp = exprNode->brother->child->child->type;
                prox_term = exprNode->brother->child->brother;
                break;
            case NRELOP:
                typeOp = exprNode->brother->child->type;
                prox_term = exprNode->brother->brother;
                break;
            default:
                printf("Errore nella generazione della parte destra dell'operazione %d", exprNode->brother->value.ival);
                exit(-188);
        }
        genOperationTCode(prox_term, typeOp, fatherModuleLine);

    }

}

// ---- Metodi per la generazione di Stat ----

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

ListNode *lineExitIfStat = NULL;
void genElseIfStatTCode(Pnode optElseStatNode, PLine fatherModuleLine){

    Pnode expr = optElseStatNode->child;
    genExprTCode(expr, fatherModuleLine);
    Pnode then = optElseStatNode->child->brother;

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    genStatListTCode(then, fatherModuleLine);

    lineExitIfStat = addIntNode(lineExitIfStat, bufferSize);
    bprintf("JMP exit\n");

    if(then->brother!=NULL && then->brother->value.ival==NOPT_ELSEIF_STAT_LIST){
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);
        genElseIfStatTCode(then->brother, fatherModuleLine);
    }
    else
        bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

}

void genIfStatTCode(Pnode if_stat, PLine fatherModuleLine){

    Pnode expr = if_stat->child;
    genExprTCode(expr, fatherModuleLine);
    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    Pnode statList = expr->brother;
    genStatListTCode(statList, fatherModuleLine);

    lineExitIfStat = addIntNode(lineExitIfStat, bufferSize);

    bprintf("JMP exit\n");

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);

    if( statList->brother!=NULL) {
        if (statList->brother->value.ival == NOPT_ELSEIF_STAT_LIST) {

            genElseIfStatTCode(statList->brother, fatherModuleLine);

            statList = statList->brother;
        }
        if (statList->brother != NULL && statList->brother->value.ival == NOPT_ELSE_STAT) {

            genStatListTCode(statList->brother, fatherModuleLine);

        }
    }
    for (; lineExitIfStat!=NULL; lineExitIfStat=lineExitIfStat->next) {
        bprintfAtIndex(lineExitIfStat->value.ival, "JMP %d\n", bufferSize-lineExitIfStat->value.ival);
    }
}

void genWhileStatTCode(Pnode whileStat, PLine fatherModuleLine){

    Pnode expr = whileStat->child;

    int jmpUp = bufferSize;

    genExprTCode(expr, fatherModuleLine);

    int jmpAddress = bufferSize;
    bprintf( "JMF temp\n");

    Pnode statList = expr->brother;
    genStatListTCode(statList, fatherModuleLine);

    bprintf("JMP %d\n", -(bufferSize-jmpUp));

    bprintfAtIndex(jmpAddress, "JMF %d\n", bufferSize-jmpAddress);


}

void genReturnStatTCode(Pnode returnStat, PLine fatherModuleLine){

//    if(returnStat->child!=NULL){
    if(returnStat->child->child!=NULL){
        Pnode expr = returnStat->child->child;
        genExprTCode(expr, fatherModuleLine);
    }

    bprintf("RETURN\n");
}

void genReadStatTCode(Pnode readStatNode, PLine fatherModuleLine){

    Pnode id = readStatNode->child->child;
    while (id!=NULL){

        PLine idLine = findLineById(id->value.sval, fatherModuleLine);

        int offsetEnv= getLevelModule(idLine, fatherModuleLine)==-1?0:getLevelModule(idLine, fatherModuleLine);
        char type = idLine->type[0];
        if(type>='A' && type<='Z'){
            type = type +32;
        }

        bprintf("READ %c %d %d\n", type,offsetEnv,idLine->oid);

        id = id->brother;
    }

}

ListNode *charListWrite = NULL;
void genWriteStatTCode(Pnode writeStatNode, PLine fatherModuleLine){

    Pnode expr = writeStatNode->child->child;
    while(expr!=NULL){

        char *type = getExprType(expr->child, fatherModuleLine);

        charListWrite = addCharNode(charListWrite, type[0]);

        genExprTCode(expr, fatherModuleLine);
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

void genTCode(PLine rootLine, Pnode root){
    wholeSymbolTable = rootLine;
    genModuleTCode(root, rootLine);
    subsEntryPModuleCall();
    flush();
}