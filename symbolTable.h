#ifndef UNTITLED_SYMBOLTABLE_H
#define UNTITLED_SYMBOLTABLE_H

#include "def.h"

// Definizioni

#define BUCKET_SIZE 20

typedef struct structLine{
    char *id, *class;
    char *type;
    int oid;

    struct structLine *bucket[BUCKET_SIZE]; // AMBIENTE

    struct structLine *next;

    //Formali
    int nFormalParams;
    struct structLine **formalParams;

} Line;

typedef struct structLine *PLine;

// Metodi

PLine symbolTable(Pnode root);
void displayTable();

#endif