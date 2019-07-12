//
// Created by maria on 10/07/19.
//

#ifndef COMPILER_COUNTER_DEPRECATO_H
#define COMPILER_COUNTER_DEPRECATO_H

#include "def.h"

int  opertationNumLines(Pnode , int),
     numberLinesCodeOFConditionalExprElseif(Pnode),
        numberLinesCodeOFConditionalExpr(Pnode),
        numberOfLinesExprOnly(Pnode);


int numberOfLinesExpr(Pnode node);
#endif //COMPILER_COUNTER_DEPRECATO_H
