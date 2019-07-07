//#ifndef UNTITLED_SYMBOLTABLE_H
//#define UNTITLED_SYMBOLTABLE_H

#include "def.h"

// Definizioni

#define BUCKET_SIZE 20

typedef struct structLine{
    char *id, *class, *type;
    int oid;

    struct structLine *bucket[BUCKET_SIZE]; // AMBIENTE

    struct structLine *next;

    //Formali
    int numParam;
    struct structLine *params[];

} Line;

typedef struct structLine *PLine;

// Metodi

PLine symbolTable(Pnode root);
void displayTable();

//#endif