#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIM_STACK 1000

#define INT_T 'i'
#define REAL_T 'r'
#define STR_T 's'
#define CHAR_T 'c'

#define ERRORE_PULL "!ERRORE! pullStack*(): carattere di tipo non corrispondente!\n"

/** Struttura di contenimento di costanti e variabili dei record di attivazione.
* *start: puntatore alla PRIMA area di memoria dello stack.
* *pointer: puntatore alla prima area di memoria non occupata.
* Nei record di attivazione verrà salvato il puntatore all'area di memoria
* corrispondente alla prima variabile che viene inizializzata nel modulo
*/
typedef struct
{
    int dimension;
    void *start;
    void *pointer;
}Stack;

/** Costruttore di Stack.
* Allocati DIM blocchi di memoria con *p = malloc(n) (funzione che ritorna un
* puntatore *p ad n caratteri consecutivi).
* Salva il primo indirizzo di memoria e inizializza il puntatore.
* Ritorna un oggetto Stack.
*/
Stack newStack() {
    Stack data;
    data.start = malloc(DIM_STACK);
    data.pointer = data.start;
    return data;

}

/** Funzione di Stack
* Ritorna la dimensione attuale della memoria utilizzata, facendo
* la differenza tra i puntatori "pointer" e "start"
*/
int getMemoryUsage(Stack* data) {
    return data->pointer - data->start;
}

/** Funzione di Stack
* Ritorna il puntatore alla prima posizione libera nella memoria
*/
void* getPointer(Stack* data) {
    return data->pointer;
}

void setPointer(Stack* data, void* pointer){
    data->pointer = pointer;
    return;
}

/**
 * Ritorna il puntatore alla prima posizione della memoria
*/
void* getStart(Stack* data) {
    return data->start;
}

/** Funzione di Stack.
* Aggiunge una variabile di tipo carattere a Stack
*/
void pushStackChar(Stack *data, char value) {
    
    char *charac;
    charac = data->pointer;
    *charac = value;
    // Operazione equivalente: *data.pointer++;
    data->pointer += sizeof(char);
    char *tipo;
    tipo = data->pointer;
    *tipo = CHAR_T;
    data->pointer+=sizeof(char);
}

/** Funzione di Stack
* Ritorna il carattere in cima alla lista e lo rimuove
*/
char pullStackChar(Stack *data) {
    data->pointer -= sizeof(char);
    char* tipo = data->pointer;
    if(*tipo != CHAR_T) { printf(ERRORE_PULL); exit(1);}
    data->pointer -= sizeof(char);
    char* value = data->pointer;
    return *value;
    
}

/** Funzione di Stack.
* Aggiunge una variabile di tipo intero a Stack
*/
void pushStackInt(Stack *data, int value) {
    int *integer;
    integer = data->pointer;
    *integer = value;
    // Dopo aver salvato il tipo e il valore incremento il puntatore alla prossima
    // area libera di 4Byte
    data->pointer += sizeof(int);
    char *tipo;
    tipo = data->pointer;
    *tipo = INT_T;
    data->pointer += sizeof(char);
}

/** Funzione di Stack
* Ritorna il carattere in cima alla lista e lo rimuove
*/
int pullStackInt(Stack *data) {
    data->pointer -= sizeof(char);
    char* tipo = data->pointer;
    if(*tipo != INT_T) printf(ERRORE_PULL);
    data->pointer -= sizeof(int);
    int *value = data->pointer;
    return *value;
    
}

/** Funzione di Stack
* Aggiunge una variabile puntatore alla stringa puntata da value alla pila
*/
void pushStackString(Stack *data, char* value) {
    char** charp;
    charp = data->pointer;
    *charp = malloc(strlen(value));
    data->pointer += sizeof(char*);
    strcpy(*charp, value);
    char *tipo;
    tipo = data->pointer;
    *tipo = STR_T;
    data->pointer += sizeof(char);
}

/**
* Rimuove e ritorna il puntatore alla stringa presente in cima alla pila
*/
char* pullStackString(Stack *data) {
    char** c;
    data->pointer -= sizeof(char);
    char* tipo = data->pointer;
    if(*tipo != STR_T) { printf(ERRORE_PULL); exit(1); }
    data->pointer -= sizeof(char*);
    c = data->pointer;
    return *c;
}

/** Funzione di Stack
* Aggiunge una variabile di tipo float alla pila
*/
void pushStackReal(Stack *data, float value) {
    float *real;
    real = data->pointer;
    *real = value;
    data->pointer += sizeof(float);
    char *tipo;
    tipo = data->pointer;
    *tipo = REAL_T;
    data->pointer += sizeof(char);
}

/**
* Rimuove e ritorna il valore di un float presente in cima alla pila
*/
float pullStackReal(Stack *data) {
    data->pointer -= sizeof(char);
    char* tipo = data->pointer;
    if(*tipo != REAL_T) {printf(ERRORE_PULL); exit(1); }
    data->pointer -= sizeof(float);
    float *real = data->pointer;
    return *real;
}

char getStackType(Stack* data) {
    if(getMemoryUsage(data) == 0) return 0;
    char* type = data->pointer - 1;
    return *type;
}



void debugStack(Stack* data) {
    printf("-- DEBUG EXECUTION STACK ->\n");
    void* p;
    int i, n = 20, memUsed = getMemoryUsage(data);
    for(i = 0; i < n; i++) {
        p = data->start;
        p += i;
        char *c = p;
        printf("%d:\t%c\tchar->ashii\t%d", i, *c, *c);
        if(i == memUsed) printf("\tprima pos libera");
        printf("\n");
    }
    printf("Dimensione memoria utilizzata: %d\n", memUsed);
}



