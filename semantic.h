#ifndef UNTITLED_SEMANTIC_H
#define UNTITLED_SEMANTIC_H
#include "def.h"
#include "symbolTable.h"
#include <ctype.h>
#include <stdbool.h>

PLine findLineById(char*, PLine);
void semanticControl(Pnode, PLine);
void  statListControl(Pnode, PLine);
char *getExprType(Pnode, PLine);
int isChildDirectly(PLine, PLine);
int getLevelModule(PLine, PLine);
int isBrother(PLine, PLine);
int getGapModuleAmbient(PLine, PLine);
PLine getFather(PLine, PLine);
#endif