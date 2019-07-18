#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExeStack.h"

void RADDf(ExeStack* exe) {
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    float result = first + second;
    //////printf("-- DEBUG: RADD -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void IADDf(ExeStack* exe) {
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    int result = first + second;
    pushExeInt(exe, result);
    //////printf("-- DEBUG: IADD -> first: %d | second:%d result: %d\n", first, second, result);
}

void RSUBf(ExeStack* exe) {
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    float result = first - second;
    //////printf("-- DEBUG: RSUB -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void ISUBf(ExeStack* exe) {
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    int result = first - second;
    //////printf("-- DEBUG: ISUB -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}

void RMULf(ExeStack* exe) {
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    float result = first * second;
    //////printf("-- DEBUG: RMUL -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void IMULf(ExeStack* exe) {
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    int result = first * second;
    //////printf("-- DEBUG: IMUL -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}

void RDIVf(ExeStack* exe) {
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    if (second==0) {
        printf("XXX: EXE ERROR IN RDIV: DIVISION BY ZERO\n");
        exit(1);
    }
    float result = first / second;
    //////printf("-- DEBUG: RDIV -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void IDIVf(ExeStack* exe) {
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    if (second==0) {
        printf("XXX: EXE ERROR IN IDIV: DIVISION BY ZERO\n");
        exit(1);
    }
    int result = first / second;
    //////printf("-- DEBUG: IDIV -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}


void EQUf(ExeStack* exe) {
    Union second = pullExeUnion(exe);
    Union first = pullExeUnion(exe);
    
    int dimension = first.dimension;
    char* firstPointer = (char*) &first;
    char* secondPointer = (char*) &second;
    for ( int i = 0 ; i < dimension ; i++ ) {
        if (*firstPointer != *secondPointer) {
            pushExeInt(exe, 0);
            return;
        }
        firstPointer++;
        secondPointer++;
    }
    pushExeInt(exe, 1);
}
/**
 * Metodo che nega il risultato della EQU //adesso lo facciamo così poi vedremo come ottimizzarlo
 */
 
void NEQf(ExeStack* exe) {
    EQUf(exe);
    int result = pullExeInt(exe);
    result = result == 0 ? 1 : 0;
    pushExeInt(exe, result);
}


// ---------------------------------OPERATORI DI CONFRONTO--------------------------------


/**
 * Operatori di confronto riguardanti i caratteri, trattati come interi
 */
void CGTf(ExeStack* exe) {
    
    char second = pullExeChar(exe);
    char first = pullExeChar(exe);
    if (first > second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void CGEf(ExeStack* exe) {
    
    char second = pullExeChar(exe);
    char first = pullExeChar(exe);
    if (first >= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void CLTf(ExeStack* exe) {
    
    char second = pullExeChar(exe);
    char first = pullExeChar(exe);
    if (first < second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void CLEf(ExeStack* exe) {
    
    char second = pullExeChar(exe);
    char first = pullExeChar(exe);
    if (first <= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

/**
 * Operatori di confronto riguardanti interi
 */
 
 
void IGTf(ExeStack* exe) {
    
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    if (first > second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void IGEf(ExeStack* exe) {
    
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    if (first >= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void ILTf(ExeStack* exe) {
    
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    if (first < second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void ILEf(ExeStack* exe) {
    
    int second = pullExeInt(exe);
    int first = pullExeInt(exe);
    if (first <= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

/**
 * Metodi di confronto applicati ai reali
 */
 
void RGTf(ExeStack* exe) {
    
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    if (first > second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void RGEf(ExeStack* exe) {
    
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    if (first >= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void RLTf(ExeStack* exe) {
    
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    if (first < second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void RLEf(ExeStack* exe) {
    
    float second = pullExeReal(exe);
    float first = pullExeReal(exe);
    if (first <= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

/**
 * Metodi di confronto applicati alle stringhe utilizzanto strcmp
 */
 
void SGTf(ExeStack* exe) {
    
    char* second = pullExeString(exe);
    char* first = pullExeString(exe);
    int result = strcmp(first, second) > 0 ? 1 : 0;
    pushExeInt(exe, result);
    return;
}

void SGEf(ExeStack* exe) {
    
    char* second = pullExeString(exe);
    char* first = pullExeString(exe);
    int result = strcmp(first, second) >= 0 ? 1 : 0;
    pushExeInt(exe, result);
    return;
}

void SLTf(ExeStack* exe) {
    
    char* second = pullExeString(exe);
    char* first = pullExeString(exe);
    int result = strcmp(first, second) < 0 ? 1 : 0;
    pushExeInt(exe, result);
    return;
}

void SLEf(ExeStack* exe) {
    
    char* second = pullExeString(exe);
    char* first = pullExeString(exe);
    int result = strcmp(first, second) <= 0 ? 1 : 0;
    pushExeInt(exe, result);
    return;
}

//---------------------------------------------------METODI DI CASTING-----------------------

/** 
 * Cast tra intero e reale
 */
 
void TOINTf(ExeStack* exe){
    float number = pullExeReal(exe);
    int casted = number;
    pushExeInt(exe, casted);
}

/** 
 * Cast da reale a intero
 */

void TOREALf(ExeStack* exe){
    int number = pullExeInt(exe);
    float casted = number;
    pushExeReal(exe, casted);
}

/**
 * Negazione tipi
 */

void IUMIf(ExeStack* exe){
    int number = pullExeInt(exe);
    number = number * (-1);
    pushExeInt(exe, number);
}

void RUMIf(ExeStack* exe){
    float number = pullExeReal(exe);
    number = number * (-1);
    pushExeInt(exe, number);
}

void LNEGf(ExeStack* exe){
    int boolean = pullExeInt(exe);
    boolean = boolean ? 0 : 1;
    pushExeInt(exe, boolean);
}