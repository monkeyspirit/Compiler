
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
    genModuleExprTCode(Pnode, PLine),
    genElseIfExprTCode(Pnode , PLine),
    genCondExprTCode(Pnode , PLine),
    genCastTCode(Pnode, PLine, int),
    genReadStatTCode(Pnode, PLine),
    genWriteStatTCode(Pnode, PLine),
    genIfStatTCode(Pnode ,PLine),
    genElseIfStatTCode(Pnode, PLine),
    genExprTCode(Pnode x_term, PLine moduleLine),
    genWhileStatTCode(Pnode, PLine),
    genReturnStatTCode(Pnode, PLine);

int countModuleBucketLines(PLine module);
int getEntryPMod(int oid);

void subsEntryPModuleCall();

#endif //COMPILER_TCODE_H
