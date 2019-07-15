
#ifndef COMPILER_TCODE_H
#define COMPILER_TCODE_H
#include "symbolTable.h"

void genTCode(PLine, Pnode, char**, int),
    genExprTCode(Pnode exprNode, PLine moduleLine),
    genStatListTCode(Pnode, PLine),
    genModuleTCode(Pnode, PLine);

#endif //COMPILER_TCODE_H
