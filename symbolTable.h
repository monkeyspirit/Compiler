/*
 * Header file del symbolTable.c
 */
#ifndef UNTITLED_SYMBOLTABLE_H
#define UNTITLED_SYMBOLTABLE_H

#include "def.h"
#define TOT 50
/*
 * Definizione della symbolTable
 */
typedef struct structLine{
    char* id;
    //PUNTATORE
    char* class;
    char* type;
    struct structLine *bucket; // AMBIENTE
    //Formali
    int numParam;
    struct structLine *next;

} Line;


/*
 * Definizione metodi e tipi
 */
typedef Line *PLine;

PLine symbolTable[TOT], newLine();

void displayTable(),
        programLine(Pnode),
        module_declLine(Pnode),
        param_declLine(Pnode),
        vardecl_listLines(int, Pnode),
        vardecl_Line(int, Pnode),
        constdecl_listLines(int, Pnode),
        constdecl_Line(int, Pnode),
        addLine(PLine,char* );

int hash(char*), search(int);

#endif