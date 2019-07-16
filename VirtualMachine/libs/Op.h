#include <stdlib.h>
#include <string.h>
#include "DataArray.h"

typedef struct {
    int command;
    Union arg1;
    int arg2;
    int arg3;
} Op;

Op newOp(int command, Union firstArg, int secondArg, int thirdArg) {
    
    Op operation;
    operation.command = command;
    operation.arg1 = firstArg;
    operation.arg2 = secondArg;
    operation.arg3 = thirdArg;
    
    return operation;
}