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
     constantDeclaration(int, char*, Pnode);
PLine findModuleById(char*, PLine*);
#endif