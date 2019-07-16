#include <stdio.h>
#include <stdlib.h>


typedef struct record{
    int returnIndex;
    int parameters;
    int locals;
    int objectsPointer;
    int accessLink;
    char returnType;
} ActivationRecord;

/**
Funzione che costruisce il record di attivazione, passandogli l'indirizzo del suo spazio delle
variabili
*/
ActivationRecord newAR( int returnIndex, int localsVariables, int params, int dataPointer ) {
    ActivationRecord record;
    record.locals = localsVariables;
    record.returnIndex = returnIndex;
    record.parameters = params;
    record.objectsPointer = dataPointer;
    record.parameters = params;
    return record;
}

int getObjectPointer(ActivationRecord* ar) {
    return ar->objectsPointer;
}

void setAccessLink( ActivationRecord* ar, int father ) {
    ar->accessLink = father;
}

int getAccessLink( ActivationRecord* ar ) {
    return ar->accessLink; 
}


/**
 * Funzione che instanzia una variabile :D
 * 
*/








//ci passano il nome della variabile e la distanza dall'attuale record di attivazione