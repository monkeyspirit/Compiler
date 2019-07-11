//
// Created by maria on 10/07/19.
//

#ifndef COMPILER_COUNTER_H
#define COMPILER_COUNTER_H

#include "def.h"

int  opertationNumLines(Pnode , int),
     logicOperationCounter(Pnode),
     relOperationCounter(Pnode);


int numberOfLinesExpr(Pnode node);
#endif //COMPILER_COUNTER_H
