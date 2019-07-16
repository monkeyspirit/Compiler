#include <stdio.h>
#include <stdlib.h>


typedef struct record{
    int returnIndex;
    int parameters;
    int objects;
    void* objectsPointer;
    struct record* caller;
    struct record* accessLink;
    char returnType;
} ActivationRecord;

/**
Funzione che costruisce il record di attivazione, passandogli l'indirizzo del suo spazio delle
variabili
*/
ActivationRecord newAR( int returnIndex, int localsVariables, int params, void* dataPointer ) {
    ActivationRecord record;
    record.objects = localsVariables;
    record.returnIndex = returnIndex;
    record.parameters = params;
    record.objectsPointer = dataPointer;
    record.parameters = params;
    return record;
}



void* getARObjectPointer(ActivationRecord* ar) {
    return ar->objectsPointer;
}

void setAccessLink( ActivationRecord* ar, ActivationRecord* father ) {
    ar->accessLink = father;
}

ActivationRecord* getAccessLink( ActivationRecord* ar ) {
    return ar->accessLink; 
}

void setCaller( ActivationRecord* ar, ActivationRecord* caller) {
    ar->caller = caller;
}

/**
 * Funzione che instanzia una variabile :D
 * 
*/








//ci passano il nome della variabile e la distanza dall'attuale record di attivazione