//
// Created by maria on 10/07/19.
//

#include "tcode.h"
#include "semantic.h"
#include "counter.h"

FILE *out;

void tCodeGenerator(PLine pLine, Pnode pNode);

PLine wholeSymbolTable;


void tCode(PLine rootLine, Pnode root){
    out = fopen("../tCode.out", "w");
    wholeSymbolTable = rootLine;
    tCodeGenerator(rootLine, root);
}

void tCodeGenerator(PLine rootLine, Pnode root){

    fprintf(out, "MOD %d\n", rootLine->oid);

    if(rootLine!=wholeSymbolTable){
        printf("Father: %d\n", getFather(rootLine, wholeSymbolTable)->oid);
    }


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
                fprintf(out, "STO ... %d\n",id->oid);
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

    fprintf( out, "NEW ");

    switch (type){
        case T_INT:{
            fprintf( out, "|int|\n");
            break;
        }
        case T_REAL:{
            fprintf( out, "|float|\n");
            break;
        }
        case T_BOOL:{
            fprintf(out , "|int|\n");
            break;
        }
        case T_CHAR:{
            fprintf( out, "|char|\n");
            break;
        }
        case T_STRING:{
            fprintf(out, "|string|\n");
            break;
        }
    }

}







int unaryOP;

void instTypeOfExpr(Pnode x_term, PLine moduleLine) { //expr punta x_term


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
                                    fprintf(out, "IUMI\n");
                                    break;
                                }
                                case 2: { //real
                                    fprintf(out, "RUMI\n");

                                    break;
                                }
                                case 3: { //bool
                                    fprintf(out, "LNEG\n");

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

                            fprintf(out, "PUSH %d -oggetti- -chain\n\t GOTO %d\nPOP\n", moduleCalled->nFormalParams,
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
                            generateCodeOFConditionalExpr(x_term->child->child->child, moduleLine);
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
                    fprintf(out, "LDC %s\n", x_term->value.sval);

                    break;
                }
                case 7: {
//int const
                    fprintf(out, "LDI %d\n", x_term->value.ival);

                    unaryOP = 1;
                    break;
                }
                case 8: {
//real const
                    fprintf(out, "LDR %f\n", x_term->value.rval);

                    unaryOP = 2;
                    break;
                }
                case 9: {
//string const
                    fprintf(out, "LDS %s\n", x_term->value.sval);
                    break;
                }
                case 10: {
//bool const
                    fprintf(out, "LDI %d\n", x_term->value.ival);

                    unaryOP = 3;
                    break;
                }
                case 11: {
                    PLine id = findLineById(x_term->value.sval, moduleLine);

//faccio riferimento a quello che ho già caricato
                    fprintf(out, "LOD .. %d\n", id->oid);
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
        case 24 : case 25:{
            logicOperation(x_term, type, moduleLine);
            break;
        }

        //rel op:
        case 18 : case 19: case 20: case 21: case 22: case 23:{
            relOperation(x_term,type, moduleLine, typeExpr);
            break;
        }

        //math op:
        case 13 : case 14: case 15: case 16:{
            mathOperation(x_term,type, moduleLine, typeExpr);
            break;
        }
    }

}

void mathOperation(Pnode term, int type,  PLine moduleLine, char* typeExpr){
    if(strcmp(typeExpr, "INT")==0 ){
        instTypeOfExpr(term, moduleLine);

        switch (type){
            case 13 :{ // *
                fprintf(out, "IMUL\n");
                break;
            }
            case 14:{  // /
                fprintf(out, "IDIV\n");
                break;
            }
            case 15: { // -
                fprintf(out, "ISUB\n");
                break;
            }
            case 16: { // +
                fprintf(out, "IADD\n");
                break;

            }
        }
    }
    else if(strcmp(typeExpr, "REAL")==0){
        instTypeOfExpr(term, moduleLine);
        switch (type){
            case 13 :{ // *
                fprintf(out, "RMUL\n");
                break;
            }
            case 14:{  // /
                fprintf(out, "RDIV\n");
                break;
            }
            case 15: { // -
                fprintf(out, "RSUB\n");
                break;
            }
            case 16: { // +
                fprintf(out, "RADD\n");
                break;

            }
        }
    }

}

void relOperation(Pnode term, int type, PLine moduleLine, char* typeExpr){

    switch (type){
        case 22: case 23:{
            equalNotEqual(term, type, moduleLine);
            break;
        }
        case 18 : case 19: case 20: case 21:{
            relationOp(term, type, moduleLine, typeExpr);
            break;
        }
    }

}

void equalNotEqual(Pnode term,int type,PLine moduleLine){
    switch (type){
        case 22:{ //==
            instTypeOfExpr(term, moduleLine);
            fprintf(out, "EQU\n");
            break;
        }
        case 23:{ //!=
            instTypeOfExpr(term, moduleLine);
            fprintf(out, "NEQ\n");
            break;
        }
    }
}

void relationOp(Pnode term, int type, PLine moduleLine, char* typeExpr){

    if(strcmp(typeExpr, "INT")==0  || strcmp(typeExpr, "BOOL")==0 ){
        instTypeOfExpr(term, moduleLine);

        switch (type){
            case 18 :{ //<=
                fprintf(out, "ILE\n");
                break;
            }
            case 19:{  //>=
                fprintf(out, "IGE\n");
                break;
            }
            case 20: { //<
                fprintf(out, "ILT\n");
                break;
            }
            case 21: { //>
                fprintf(out, "IGT\n");
                break;

            }
        }
    }
    else if(strcmp(typeExpr, "CHAR")==0){
        instTypeOfExpr(term, moduleLine);
        switch (type){
            case 18 :{ //<=
                fprintf(out, "CLE\n");
                break;
            }
            case 19:{  //>=
                fprintf(out, "CGE\n");
                break;
            }
            case 20: { //<
                fprintf(out, "CLT\n");
                break;
            }
            case 21: { //>
                fprintf(out, "CGT\n");
                break;

            }
        }
    }
    else if(strcmp(typeExpr, "REAL")==0){
        instTypeOfExpr(term, moduleLine);
        switch (type){
            case 18 :{ //<=
                fprintf(out, "RLE\n");

                break;
            }
            case 19:{  //>=
                fprintf(out, "RGE\n");
                break;
            }
            case 20: { //<
                fprintf(out, "RLT\n");
                break;
            }
            case 21: { //>
                fprintf(out, "RGT\n");
                break;

            }
        }
    }
    else if(strcmp(typeExpr, "STRING")==0){
        instTypeOfExpr(term, moduleLine);
        switch (type){
            case 18 :{ //<=
                fprintf(out, "SLE\n");
                break;
            }
            case 19:{  //>=
                fprintf(out, "SGE\n");
                break;
            }
            case 20: { //<
                fprintf(out, "SLT\n");
                break;
            }
            case 21: { //>
                fprintf(out, "SGT\n");
                break;

            }
        }
    }

}

void logicOperation(Pnode term, int type, PLine moduleLine){

    switch (type){
        case 24:{ //AND
            fprintf(out, "----AND----\n");
            int offset = numberOfLinesExpr(term) + 2;
            fprintf(out, "1-JMF %d\n", offset);
            instTypeOfExpr(term, moduleLine);
            fprintf(out,"2-JMP 2\n");
            fprintf(out, "3-LDI 0\n");
            fprintf(out, "------\n");
            break;
        }
        case 25:{ //OR
            fprintf(out, "----OR----\n");
            int exit = numberOfLinesExpr(term)+1;
            fprintf(out, "JMF 3\n");
            fprintf(out, "LDI 1\n");
            fprintf(out,"JMP %d\n", exit);
            instTypeOfExpr(term, moduleLine);


            break;
        }
    }


}

void generateCodeOFConditionalExpr(Pnode condition, PLine moduleLine){

    Pnode ifNode = condition->child;
    instTypeOfExpr(ifNode, moduleLine);
    int next = numberOfLinesExpr(ifNode->brother->brother);
    fprintf(out, "JMF %d\n", next);

    Pnode thenNode = ifNode->brother;
    instTypeOfExpr(thenNode, moduleLine);


//    printf("Tot istruction: %d\n", numberOfLinesExpr(condition));
//
//    printf("Count else: %d\n", numberOfLinesExpr(condition->child->brother->brother));

//    fprintf(out, "JMP %d\n", exit);

//    if(cond_expr->child->brother->brother == NOPT_ELSEIF_EXPR_LIST){
//
//        Pnode elseifNode = cond_expr->child->brother->brother->child;
//        Pnode thenElseifNode = cond_expr->child->brother->brother->child->brother;
//
//        if(cond_expr->child->brother->brother->child->brother->brother==NOPT_ELSEIF_EXPR_LIST)
//
//        expr = cond_expr->child->brother;
//    }

    Pnode elseNode = ifNode->brother->brother;
    instTypeOfExpr(elseNode, moduleLine);


}
