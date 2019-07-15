#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIM_STACK 1000

#define INT_T 'i'
#define REAL_T 'r'
#define STR_T 's'
#define CHAR_T 'c'

#define ERRORE "!ERRORE! data.h"

/** Struttura di contenimento di costanti e variabili dei record di attivazione.
* *start: puntatore alla PRIMA area di memoria dello stack.
* *pointer: puntatore da usare per modificare gli elementi salvati.
* Nei record di attivazione verrà salvato il puntatore all'area di memoria
* corrispondente alla prima variabile che viene inizializzata nel modulo
*/
typedef struct
{
    int dimension;
    void *start;
    void *pointer;
    void *freeSpace;
}Data;

/** Costruttore di Data.
* Allocati DIM blocchi di memoria con *p = malloc(n) (funzione che ritorna un
* puntatore *p ad n caratteri consecutivi).
* Salva il primo indirizzo di memoria e inizializza il puntatore.
* Ritorna un oggetto Data.
*/
Data newData() {
    Data data;
    data.start = malloc(DIM_STACK);
    data.pointer = data.freeSpace = data.start;
    return data;
}

/** Funzione di Data
* Ritorna la dimensione attuale della memoria utilizzata, facendo
* la differenza tra i puntatori "pointer" e "start"
*/
int getDataMemoryUsage(Data* data) {
    return data->freeSpace - data->start;
}

/** Funzione di Data
* Ritorna il puntatore alla prima posizione libera nella memoria
*/
void* getDataFreeSpace(Data* data) {
    return data->freeSpace;
}

/**
 * La chiameremo quando dovremo togliere un activation record (le var relative 
 * devono essere cancellate) e quindi imposteremo free space a objectPointer
*/
void setDataFreeSpace(Data* data, void* freeSpace) {
    data->freeSpace = freeSpace;
}

void setDataPointer(Data* data, void* pointer) {
    data->pointer = pointer;
}

/**
 * Ritorna il puntatore alla prima posizione della memoria
*/
void* getDataStart(Data* data) {
    return data->start;
}


void* getDataPointer(Data* data) {
    return data->pointer;
}
/**
 * Funzione che crea uno spazio di memoria dedito ad un carattere
 * e perciò comincerà con un 'c'
 */
void pushDataChar(Data* data) {
    char* type;
    type = data->freeSpace;
    *type = CHAR_T;
    data->freeSpace+=sizeof(char);
    data->freeSpace+=sizeof(char);
}

/** Funzione di Data.
* Modifica una variabile di tipo carattere di Data
*/
void setDataChar(Data *data, char value) {
    char* type;
    type = data->pointer;
    if(*type != CHAR_T) printf("%s:setDataChar sta modificando un tipo diverso da carattere", ERRORE);
    data->pointer++;
    
    char* charac;
    charac = data->pointer;
    *charac = value;
    // Operazione equivalente: *data.pointer++;
    data->pointer += sizeof(char);
}

/** Funzione di Data
* 
*/
char getDataChar(Data *data) {
    char* type;
    type = data->pointer;
    if(*type != CHAR_T) printf("%s:getDataChar sta prendendo un tipo diverso da carattere", ERRORE);
    data->pointer++;
    char* value = data->pointer;
    return *value;
}

/**
 * Funzione che crea uno spazio di memoria dedito ad un intero
 * e perciò comincerà con un 'i'
 */
void pushDataInt(Data* data) {
    char* type;
    type = data->freeSpace;
    *type = INT_T;
    data->freeSpace+=sizeof(char);
    data->freeSpace+=sizeof(int);
}

/** Funzione di Data.
* Aggiunge una variabile di tipo intero a Data
*/
void setDataInt(Data *data, int value) {
    char* type;
    type = data->pointer;
    if(*type != INT_T) printf("%s:setDataInt sta modificando un tipo diverso da intero", ERRORE);
    data->pointer++;
    
    int* integer;
    integer = data->pointer;
    *integer = value;
    // Dopo aver salvato il tipo e il valore incremento il puntatore alla prossima
    // area libera di 4Byte
    data->pointer += sizeof(int);
}

/** Funzione di Data
* 
*/
int getDataInt(Data *data) {
    char* type;
    type = data->pointer;
    if(*type != INT_T) printf("%s:getDataInt sta prendendo un tipo diverso da intero", ERRORE);
    data->pointer++;
    int* value = data->pointer;
    return *value;
}

/**
 * Funzione che crea uno spazio di memoria dedito ad una stringa
 * e perciò comincerà con un 's'
 */
void pushDataString(Data* data) {
    char* type;
    type = data->freeSpace;
    *type = STR_T;
    data->freeSpace+=sizeof(char);
    data->freeSpace+=sizeof(void*);
}


/** Funzione di Data
* Aggiunge una variabile puntatore alla stringa puntata da value alla pila
*/
void setDataString(Data *data, char* value) {
    char* type;
    type = data->pointer;
    if(*type != STR_T) printf("%s:setDataString sta modificando un tipo diverso da stringa", ERRORE);
    data->pointer++;
    
    char** charp;
    charp = data->pointer;
    *charp = malloc(strlen(value));
    data->pointer += sizeof(char*);
    strcpy(*charp, value);
}

/**
* Rimuove e ritorna il puntatore alla stringa presente in cima alla pila
*/
char* getDataString(Data *data) {
    char* type;
    type = data->pointer;
    if(*type != STR_T) printf("%s:getDataString sta prendendo un tipo diverso da stringa", ERRORE);
    data->pointer++;
    
    char** c;
    c = data->pointer;
    return *c;
}

/**
 * Funzione che crea uno spazio di memoria dedito ad un reale
 * e perciò comincerà con un 'r'
 */
void pushDataReal(Data* data) {
    char* type;
    type = data->freeSpace;
    *type = REAL_T;
    data->freeSpace+=sizeof(char);
    data->freeSpace+=sizeof(float);
}


/** Funzione di Data
* Aggiunge una variabile di tipo float alla pila
*/
void setDataReal(Data *data, float value) {
    char* type;
    type = data->pointer;
    if(*type != REAL_T) 
        printf("%s:setDataReal sta modificando un tipo diverso da real", ERRORE);
    data->pointer++;
    
    float *real;
    real = data->pointer;
    *real = value;
    data->pointer += sizeof(float);
}

/**
* Rimuove e ritorna il valore di un float presente in cima alla pila
*/
float getDataReal(Data *data) {
    char* type;
    type = data->pointer;
    if(*type != REAL_T) printf("%s:getDataReal sta prendendo un tipo diverso da real", ERRORE);
    data->pointer++;
    
    float *real = data->pointer;
    return *real;
}

void debugData(Data* data) {
    printf("\n-- DEBUG DATA ->\n");
    void* p;
    int i, n = 20, memUsed = getDataMemoryUsage(data);
    for(i = 0; i < n; i++) {
        p = data->start;
        p += i;
        char *c = p;
        printf("%d:\t%c\tchar->ashii\t%d", i, *c, *c);
        if(i == memUsed) printf("\tprima pos libera");
        if(i == (data->pointer - data->start)) printf("\tposizione pointer");
        printf("\n");
    }
    printf("Dimensione memoria utilizzata: %d\n", memUsed);
}

int n = 0;
void goOneAhead(Data* data) {
    n++;
    char* type;
    type = data->pointer;
    data->pointer++;
    switch (*type){
        case 'i' : data->pointer += sizeof(int); break;
        case 'c' : data->pointer += sizeof(char); break;
        case 's' : data->pointer += sizeof(char*); break;
        case 'r' : data->pointer += sizeof(float);
    }
}

char getDataType(Data* data){
    char* type = data->pointer;
    return *type;
}