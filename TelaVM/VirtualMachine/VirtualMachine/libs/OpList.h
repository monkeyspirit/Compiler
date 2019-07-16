#include <stdlib.h>
#include <string.h>
#include "Op.h"
#include "tCodeDef.h"

const int singleParam[] = { JMP, JMF, LDI, LDR, LDC, LDS, WRITE, NEW, MOD, GOTO};
const int singleParamSize = sizeof(singleParam)/4;

const int noParam[] = {HALT, POP, IADD, ISUB, IMUL, IDIV, RADD, RSUB, RMUL, RDIV, RETURN, EQU, NEQ, CGT, CGE, CLT, CLE, IGT, IGE, ILT, ILE, RGT, RGE, RLT, RLE, SGT, SGE, SLT, SLE, TOINT, TOREA, IUMI, RUMI, LNEG };
const int noParamSize = sizeof(noParam)/4;

const int doubleParam[] = {STO, LOD};
const int doubleParamSize = sizeof(doubleParam)/4;

const int tripleParam[] = {PUSH, READ};
const int tripleParamSize = sizeof(tripleParam)/4;

typedef struct
{
    Op *opArray;
    int currentOperation;
    int rows;
} OpList;

OpList newOpList ()
{
    OpList list;
    list.currentOperation = 0;
    return list;
}

int numOfParams (int code) {
    for (int i = 0; i < noParamSize; i++)
        if (noParam[i] == code )
            return 0;
            
    for (int i = 0; i < singleParamSize; i++)
        if (singleParam[i] == code )
            return 1;
    
    for (int i = 0; i < doubleParamSize; i++)
        if (doubleParam[i] == code )
            return 2;
            
    for (int i = 0; i < tripleParamSize; i++)
        if (tripleParam[i] == code )
            return 3;
}

void initList(OpList* list, char* file) {
    FILE *fp;
    fp = fopen(file, "r");
    
    if (fp == NULL)
   {
      perror("Error while opening the file. Try another filename.\n");
      exit(1);
   }
    
    int rows, second, third, code, params;
    fscanf(fp, "%d", &rows);
    list->opArray = calloc(rows, sizeof(Op));
    list->rows = rows;
    for ( int i = 0; i < rows; i++ ) {
        fscanf(fp, "%d ", &code);
        params = numOfParams(code);
        Union first;
        second = third = -1;
        switch (params) {
            
            case 1 : {
                switch ( code ) {
                    case JMP:
                    case JMF:
                    case NEW:
                    case GOTO:
                    case LDI:
                    case MOD: {
                        int value;
                        fscanf(fp, "%d", &value);
                        first.data.integer = value;
                    }break;
                    
                    case LDC : {
                        char value;
                        fscanf(fp, "%c", &value);
                        first.data.charap = value;
                    }break;
                    
                    case LDS : {
                        char* value;
                        fgets(value, 100, fp);
                        value = value + 1;
                        value[strlen(value)-2] = 0;
                        char* string = malloc(strlen(value));
                        strcpy(string, value);
                        first.data.string = string;
                    }break;
                    
                    case LDR : {
                        float value;
                        fscanf(fp, "%f", &value);
                        first.data.real = value;
                    }break;
                    
                    case WRITE : {
                        char* value = malloc(100);
                        fscanf(fp, "%s", value);
                        first.data.string = value;
                    }
                    
                }
            }break;
            
            case 2 : {
                int value1, value2;
                fscanf(fp, "%d %d", &value1, &value2);
                first.data.integer = value1;
                second = value2;
                
            }break;
            
            case 3 : {
                switch ( code ) {
                    case PUSH : {
                        int value1, value2, value3;
                        fscanf(fp, "%d %d %d\n", &value1, &value2, &value3);
                        first.data.integer = value1;
                        second = value2;
                        third = value3;
                    }break;
                    
                    case READ : {
                        char value1;
                        int value2, value3;
                        fscanf(fp, "%c %d %d\n", &value1, &value2, &value3);
                        first.data.charap = value1;
                        second = value2;
                        third = value3;
                    }break;
                }
            }
        }
        addInPosition(i, list, code, first, second, third);
        
        
    }
    //printf("\n -- DEBUG: codice letto correttamente\n");
    
}


void addInPosition(int index, OpList* list, int command, Union firstArg, int secondArg , int thirdArg) {
    Op operation = newOp(command, firstArg, secondArg, thirdArg);
    list->opArray[index] = operation;
}


Op getCurrent(OpList* list){
    return list->opArray[list->currentOperation];
}

Op goToInstruction(OpList* list, int index){
    list->currentOperation = index;
    return list->opArray[index];
}

Op getOperationByIndex(OpList* list, int index){
    return list->opArray[index];
}

void jumpByOffset (OpList* list, int jumps) {
    list->currentOperation += (jumps) - 1;
}

void jumpToIndex (OpList* list, int index) {
    list->currentOperation = index;
}

Op getNextOperation(OpList* list) {
    int index = list->currentOperation;
    list->currentOperation++;
    return list->opArray[index];
}

void debugOp(OpList* list){
    //////printf("-- DEBUG OPERATION LIST ->\n");
    int i;
    for (i = 0; i < list->rows; i++)
    {
        Op operation = getOperationByIndex(list, i);
        printf("%d Operation %d with params: %x, %d, %d\n",i, operation.command, operation.arg1.data, operation.arg2, operation.arg3);
    }
}

