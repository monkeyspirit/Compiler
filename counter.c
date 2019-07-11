//
// Created by maria on 10/07/19.
//

#include "counter.h"
#include "def.h"



int numberOfLinesExpr(Pnode x_term) { //nodo di cui voglio determinare il numero di linee
    int num = 0;

    if (x_term->child != NULL) {
        if (x_term->child->value.ival == NFACTOR) {
            if (x_term->child->child->type == 11) {
                numberOfLinesExpr(x_term->child);
            } else {
                switch (x_term->child->child->value.ival) {
                    case NUNARYOP: {
                        num = num + numberOfLinesExpr(x_term->child->child->brother);
                        num++;
                        break;
                    }
                    case NMODULE_CALL: {
//                        generateCodeFormalParams(x_term->child->child->child->brother->child->child, moduleLine);
//                        PLine moduleCalled = findLineByIdAndClass(x_term->child->child->child->value.sval, "MOD",
//                                                                  moduleLine->bucket);
//                        fprintf(out, "PUSH %d -oggetti- -chain\n\t GOTO %d\nPOP\n", moduleCalled->nFormalParams,
//                                moduleCalled->oid);
//formali
                        num = num + 3;
                        break;
                        break;
                    }
                    case NEXPR: {
                        num = num + numberOfLinesExpr(x_term->child->child);
                        break;
                    }
                    case NCONSTANT: {
                        num = num + numberOfLinesExpr(x_term->child);
                        break;
                    }
                    case NCOND_EXPR: {
//                        generateCodeOFConditionalExpr(x_term->child->child->child);
                        break;
                    }
                    case NTYPE: {

                        break;
                    }
                }
            }
        }
        else {
            num = num + numberOfLinesExpr(x_term->child);
        }
    }
    else {
            num++;
    }



    int typeOp;
    Pnode prox_term;
    if(x_term->brother != NULL) {

        if (x_term->value.ival == 32) {//rel term è diverso per costruzione
            prox_term = x_term->brother->brother;
            typeOp = x_term->brother->child->type;
        } else {
            prox_term = x_term->brother->child->brother;
            typeOp = x_term->brother->child->child->type;
        }


        num = num + opertationNumLines(prox_term, typeOp);
    }

    return num;
}

int opertationNumLines(Pnode x_term, int typeOp){
    int num = 0;

    switch (typeOp){
        //logic op:
        case 24 : case 25:{
            num = num + numberOfLinesExpr(x_term);
            num = num +3;
            break;
        }

            //rel op:
        case 18 : case 19: case 20: case 21: case 22: case 23:{
            num = num + numberOfLinesExpr(x_term);
            num++;
            break;
        }

            //math op:
        case 13 : case 14: case 15: case 16:{
            num = num + numberOfLinesExpr(x_term);
            num++;
            break;
        }
    }

    return num;
}