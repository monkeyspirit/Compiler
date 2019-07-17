//
// Created by maria on 10/07/19.
//

#include "counter(deprecato).h"
#include "def.h"


//
//
//int numberOfLinesExpr(Pnode x_term) { //nodo di cui voglio determinare il numero di linee
//    int num = 0;
//
//    if(x_term!=NULL) {
//
//        if (x_term->child == NULL) { // se siamo ai minimi termini
//            num++;
//        } else { // passo ricorsivo
//
//            if (x_term->child->value.ival != NFACTOR) {
//                num = num + numberOfLinesExpr(x_term->child);
//            }
//            else {
//                if (x_term->child->child->type == 11)  // per gli id scendiamo di livello
//                    num = num + numberOfLinesExpr(x_term->child);
//                else {
//                    switch (x_term->child->child->value.ival) {
//                        case NUNARYOP: {
//                            num = num + numberOfLinesExpr(x_term->child->child->brother);
//                            num++;
//                        }
//                        case NMODULE_CALL: {
//                            //                            generateCodeFormalParams(x_term->child->child->child->brother->child->child, moduleLine);
////                            PLine moduleCalled = findLineByIdAndClass(x_term->child->child->child->value.sval, "MOD",
////                                                                      moduleLine->bucket);
////                            PLine moduleCalled = findLineById(x_term->child->child->child->value.sval, moduleLine);
//                            // ↑↑↑↑↑↑ forse bisogna controllare che sia esclusivamente MOD ↑↑↑↑↑↑
//
////                            if(isChildOfCaller(moduleCalled, moduleLine)==1){
////                                printf("Figlio\n");
////                            }
////                            if(isFatherOfCaller(moduleCalled, moduleLine)==1){
////                                printf("Genitore\n");
////                            }
//                           num = num +3;
//                           break;
//                        }
//                        case NEXPR:
//                            num = num + numberOfLinesExpr(x_term->child->child);
//                            break;
//
//                        case NCONSTANT: {
//                            num = num + numberOfLinesExpr(x_term->child);
//                            break;
//                        }
//                        case NCOND_EXPR: {
//                            num = num + numberLinesCodeOFConditionalExpr(x_term->child->child);
//                            break;
//                        }
//                        case NTYPE: {
//                            break;
//                        }
//                    }
//                }
//            }
//
//        }
//
//
//        int typeOp;
//        Pnode prox_term;
//
//        if (x_term->brother != NULL){ // se c'è una parte destra bisogna controllare
//
//
//            if (x_term->value.ival == 32) { // rel term è diverso per costruzione
//                prox_term = x_term->brother->brother;
//                typeOp = x_term->brother->child->type;
//            } else {
//                prox_term = x_term->brother->child->brother;
//                typeOp = x_term->brother->child->child->type;
//            }
//
//            num = num + opertationNumLines(prox_term, typeOp);
//        }
//
//    }
//    return num;
//
//}
//
//
//
//int opertationNumLines(Pnode x_term, int typeOp){
//    int num = 0;
//
//    switch (typeOp){
//        //logic op:
//        case 24 : case 25:{
//            num = num + numberOfLinesExpr(x_term);
//            num = num + 3;
//            break;
//        }
//
//            //rel op:
//        case 18 : case 19: case 20: case 21: case 22: case 23:{
//            num = num + numberOfLinesExpr(x_term);
//            num++;
//            break;
//        }
//
//            //math op:
//        case 13 : case 14: case 15: case 16:{
//            num = num + numberOfLinesExpr(x_term);
//            num++;
//            break;
//        }
//    }
//
//    return num;
//}
//
//int numberLinesCodeOFConditionalExpr(Pnode condition){
//
//    int num = 0;
//
//    Pnode ifNode = condition->child;
//    Pnode thenNode = condition->brother ;
//
//
//    num = num + numberOfLinesExpr(ifNode);
//
//    num++;
//
//
//    num = num + numberOfLinesExpr(thenNode);
//
//    Pnode elseNode;
//
//
//    if(condition->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
//
//        num++;
//        num = num + numberLinesCodeOFConditionalExprElseif(condition->brother->brother->child);
//        num = num + numberOfLinesExpr(elseNode);
//
//    }
//    else{
//        num++;
//        num = num + numberOfLinesExpr(elseNode);
//
//    }
//
//    return num;
//}
//
//int numberLinesCodeOFConditionalExprElseif(Pnode optExpr){
//    int num = 0;
//    Pnode expr = optExpr->child;
//
//    num = num + numberOfLinesExpr(expr);
//    Pnode then = optExpr->brother->child;
//
//    num++;
//
//    num = num + numberOfLinesExpr(then);
//
//    if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
//
//        num = num + numberLinesCodeOFConditionalExprElseif(optExpr->brother->brother);
//
//
//    }
//    num++;
//
//    return num;
//}