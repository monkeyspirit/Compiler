//
// Created by maria on 05/07/19.
//
#ifndef UNTITLED_SEMANTIC_H
#define UNTITLED_SEMANTIC_H
#include "def.h"
#include "symbolTable.h"
#include <ctype.h>
#include <stdbool.h>



void semanticControl(PLine, Pnode),
     moduleNameControl(char*, char*, char*),
     constantDeclaration(int, char*, Pnode),
     controlFormalPar(Pnode, PLine, PLine*);

char *typeOfExpr(Pnode , PLine );

char *operationChecking(int, char *, char *),
     *typeOfModuleCall(Pnode, PLine*),
     *typeOFConstant(int),
     *typeOFConditionalExpr(Pnode, PLine),
     *typeOfElseIfExprList(Pnode , PLine),
     *unaryOperationChecking(int , char *);

void  controlOfStatment(Pnode, PLine);

PLine findLineByIdAndClass(char*, char*, PLine*),
      findLineByIdFromPCV(char* , PLine*),
      findLineByIdFromPV(char* , PLine*);
#endif