//
// Created by maria on 10/07/19.
//

#ifndef COMPILER_TCODE_H
#define COMPILER_TCODE_H
#include "symbolTable.h"

void newTcode(int),
     codeStatment(Pnode, PLine),
     tCode(PLine, Pnode),
     operationCode(Pnode, int, PLine, char*),
     mathOperation(Pnode, int, PLine, char*),
     relOperation(Pnode, int, PLine,char*),
     logicOperation(Pnode, int, PLine),
     equalNotEqual(Pnode, int, PLine),
     relationOp(Pnode, int, PLine, char*),
     generateCodeFormalParams(Pnode, PLine),
     generateCodeElseIfExpr(Pnode , PLine , int),
     generateCodeOFConditionalExpr(Pnode , PLine )  ;

void instTypeOfExpr(Pnode , PLine );


#endif //COMPILER_TCODE_H
