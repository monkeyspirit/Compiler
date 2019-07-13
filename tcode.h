
#ifndef COMPILER_TCODE_H
#define COMPILER_TCODE_H
#include "symbolTable.h"

void genNewTCode(int),
    genStatListTCode(Pnode, PLine),
    genTCode(PLine, Pnode),
    genOperationTCode(Pnode, int, PLine, char*),
    mathOperation(Pnode, int, PLine, char*),
    relOperation(Pnode, int, PLine,char*),
    logicOperation(Pnode, int, PLine),
    relationOp(Pnode, int, PLine, char*),
    genModuleTCode(Pnode , PLine),
    genModuleParamsTCode(Pnode, PLine),
    genElseIfExprTCode(Pnode , PLine),
    genCondExprTCode(Pnode , PLine),
    genCastTCode(Pnode, PLine, int),
    genReadStatTCode(Pnode, PLine),
    genWriteStatTCode(Pnode, PLine),
    genIfStatTCode(Pnode ,PLine),
    genElseIfStatTCode(Pnode, PLine),
    genExprTCode(Pnode x_term, PLine moduleLine);

int countModuleBucketLines(PLine module);

#endif //COMPILER_TCODE_H
