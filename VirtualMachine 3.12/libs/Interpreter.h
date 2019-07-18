#include <stdio.h>
#include <stdlib.h>



typedef struct interpreter {
    ExeStack *exe;
    DataArray *data;
    ActivationRecord *callStack;
    int dimension;
    int currentAR;
} Interpreter;

Interpreter newInterpreter() {
    Interpreter interpreter;
    interpreter.exe = malloc(sizeof(ExeStack));
    *(interpreter.exe) = newExeStack();
    
    interpreter.data = malloc(sizeof(DataArray));
    *(interpreter.data) = newDataArray(); 
    
    interpreter.callStack = calloc(STEP, sizeof(ActivationRecord));
    interpreter.dimension = STEP;
    interpreter.currentAR = -1;
    return interpreter;
}

void goToOID(Interpreter* gfl, int oID, int ar) {
    gfl->data->index = gfl->callStack[ar].objectsPointer;
    for(int i = 0; i < oID; i++)
        increaseDAIndex(gfl->data);
}

void NEWf( Interpreter* gfl, int dim) {
    allocDataArray( gfl->data, dim );
}

void LODLocal ( Interpreter* gfl, int ar, int oID ) {
    goToOID( gfl, oID , ar );
    Union temp = getDAUnion(gfl->data);
    pushExeUnion (gfl->exe, temp);
}

void LODf ( Interpreter* gfl, int jumps, int oID ) {
    int al = gfl->currentAR;
    for(int i = 0; i < jumps; i++)
        al = gfl->callStack[al].accessLink;
    LODLocal(gfl, al, oID);
}

void STOLocal ( Interpreter* gfl, int ar, int oID ) {
    goToOID(gfl, oID, ar);
    Union temp = pullExeUnion(gfl->exe);
    setDAUnion(gfl->data, temp);
}

void STOf ( Interpreter* gfl,  int jumps, int oID ) {
    int al = gfl->currentAR;
    for(int i = 0; i < jumps; i++)
        al = gfl->callStack[al].accessLink;
    STOLocal(gfl, al, oID);
}
    
int findAccessLink( Interpreter* gfl, int jumps ) {
    if ( jumps == -1 )
        return -1;
    int pointer = gfl->currentAR;
    int i;
    for ( i = 0 ; i < jumps ; i++ ) 
        pointer = gfl->callStack[pointer].accessLink;
    return pointer;
}

void recursiveParamSto(Interpreter* gfl, int parameters) {
    if(parameters > 0) {
        Union temp = pullExeUnion(gfl->exe);
        recursiveParamSto(gfl, parameters - 1);
        setDAUnion(gfl->data, temp);
    }
}

void PUSHf(Interpreter* gfl, int parameters, int locals, int jumps, int returnIndex ) {
    ActivationRecord ar = newAR(returnIndex, locals, parameters, gfl->data->size );
    int defEnvironment = findAccessLink(gfl, jumps);
    gfl->data->index = gfl->data->size;
    ar.accessLink = defEnvironment;
    
    for ( int i = 0; i < locals; i++) {
        increaseSize(gfl->data);
    }
    
    recursiveParamSto(gfl, parameters);
    
    gfl->currentAR++;
    gfl->callStack[gfl->currentAR] = ar;
}

int RETURNf( Interpreter* gfl ) {
    ActivationRecord almostDead = gfl->callStack[gfl->currentAR];
    setDataSize(gfl->data, almostDead.objectsPointer);
    return almostDead.returnIndex;
}



/*
int main() {

    Interpreter *gfl = malloc(sizeof(Interpreter));
    *gfl = newInterpreter();
    pushExeInt( gfl->exe, 5);
    pushExeReal( gfl->exe, 8.3651);
    PUSH( gfl , 2, 1, -1, 0);
    pushExeChar( gfl->exe, 'p');
    NEW ( gfl, 1);
    STO ( gfl, 2, 0);
    LOD ( gfl, 2, 0);
    LOD ( gfl, 0, 0);
    TOINT( gfl->exe);
    pushExeInt( gfl->exe, 6);
    IADD ( gfl->exe );
    PUSH (gfl, 1, 1, 0, 0);
    pushExeInt( gfl->exe, 36);
    NEW (gfl, 4);
    STO (gfl, 1, 0);
    LOD (gfl, 1, 0);
    LOD (gfl, 1, 1);
    ISUB (gfl->exe);
    printf("%d", pullExeInt(gfl->exe));
    return 0;
}*/