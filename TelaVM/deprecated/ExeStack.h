#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIM_STACK 1000

#define INT_T 'i'
#define REAL_T 'r'
#define STR_T 's'
#define CHAR_T 'c'

#define ERRORE_PULL "!ERRORE! pullExe*(): carattere di tipo non corrispondente!\n"

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
}ExeStack;

/** Costruttore di ExeStack.
* Allocati DIM blocchi di memoria con *p = malloc(n) (funzione che ritorna un
* puntatore *p ad n caratteri consecutivi).
* Salva il primo indirizzo di memoria e inizializza il puntatore.
* Ritorna un oggetto ExeStack.
*/
ExeStack newExeStack() {
    ExeStack data;
    data.start = malloc(DIM_STACK);
    data.pointer = data.start;
    return data;

}

/** Funzione di ExeStack
* Ritorna la dimensione attuale della memoria utilizzata, facendo
* la differenza tra i puntatori "pointer" e "start"
*/
int getMemoryExeUsage(ExeStack* data) {
    return data->pointer - data->start;
}

/** Funzione di ExeStack
* Ritorna il puntatore alla prima posizione libera nella memoria
*/
void* getExePointer(ExeStack* data) {
    return data->pointer;
}

/**
 * Ritorna il puntatore alla prima posizione della memoria
*/
void* getExeStart(ExeStack* data) {
    return data->start;
}

/** Funzione di ExeStack.
* Aggiunge una variabile di tipo carattere a ExeStack
*/
void pushExeChar(ExeStack *data, char value) {
    
    char *charac;
    charac = data->pointer;
    *charac = value;
    // Operazione equivalente: *data.pointer++;
    data->pointer += sizeof(char);
}

/** Funzione di ExeStack
* Ritorna il carattere in cima alla lista e lo rimuove
*/
char pullExeChar(ExeStack *data) {
    data->pointer -= sizeof(char);
    char* value = data->pointer;
    return *value;
    
}

/** Funzione di ExeStack.
* Aggiunge una variabile di tipo intero a ExeStack
*/
void pushExeInt(ExeStack *data, int value) {
    int *integer;
    integer = data->pointer;
    *integer = value;
    // Dopo aver salvato il tipo e il valore incremento il puntatore alla prossima
    // area libera di 4Byte
    data->pointer += sizeof(int);
}

/** Funzione di ExeStack
* Ritorna il carattere in cima alla lista e lo rimuove
*/
int pullExeInt(ExeStack *data) {
    data->pointer -= sizeof(char);
    char* tipo = data->pointer;
    if(*tipo != INT_T) printf(ERRORE_PULL);
    data->pointer -= sizeof(int);
    int *value = data->pointer;
    return *value;
    
}

/** Funzione di ExeStack
* Aggiunge una variabile puntatore alla stringa puntata da value alla pila
*/
void pushExeString(ExeStack *data, char* value) {
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
char* pullExeString(ExeStack *data) {
    char** c;
    data->pointer -= sizeof(char);
    char* tipo = data->pointer;
    if(*tipo != STR_T) { printf(ERRORE_PULL); exit(1); }
    data->pointer -= sizeof(char*);
    c = data->pointer;
    return *c;
}

/** Funzione di ExeStack
* Aggiunge una variabile di tipo float alla pila
*/
void pushExeReal(ExeStack *data, float value) {
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
float pullExeReal(ExeStack *data) {
    data->pointer -= sizeof(char);
    char* tipo = data->pointer;
    if(*tipo != REAL_T) { printf(ERRORE_PULL); exit(1); }
    data->pointer -= sizeof(float);
    float *real = data->pointer;
    return *real;
}

/**
* Metodi matematici applicati alla pila 
* Prendo due numeri (float o interi dipende dall'operazione invocata) 
* in modo distruttivo, esegue un operazione su quei valori e mette
* il risultato in cima alla pila
* 
*/
void RADD(ExeStack* exe) {
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    float result = first + second;
    printf("-- DEBUG: RADD -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void IADD(ExeStack* exe) {
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    int result = first + second;
    printf("-- DEBUG: IADD -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}

void RSUB(ExeStack* exe) {
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    float result = first - second;
    printf("-- DEBUG: RSUB -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void ISUB(ExeStack* exe) {
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    int result = first - second;
    printf("-- DEBUG: ISUB -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}

void RMUL(ExeStack* exe) {
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    float result = first * second;
    printf("-- DEBUG: RMUL -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void IMUL(ExeStack* exe) {
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    int result = first * second;
    printf("-- DEBUG: IMUL -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}

void RDIV(ExeStack* exe) {
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    if (second==0) {
        printf("XXX: EXE ERROR IN RDIV: DIVISION BY ZERO\n");
        exit(1);
    }
    float result = first / second;
    printf("-- DEBUG: RDIV -> first: %f | second:%f result: %f\n", first, second, result);
    pushExeReal(exe, result);
}

void IDIV(ExeStack* exe) {
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    if (second==0) {
        printf("XXX: EXE ERROR IN IDIV: DIVISION BY ZERO\n");
        exit(1);
    }
    int result = first / second;
    printf("-- DEBUG: IDIV -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}

/**
 * Funzioni logiche (bisogna farle distruttive? bhoo, ci penseremo)
*/
void CEQ(ExeStack* exe) {
    char first = pullExeChar(exe);
    char second = pullExeChar(exe);
    int result = first == second;
    printf("-- DEBUG: CEQ -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
}

void IEQ(ExeStack* exe) {
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    int result = first == second;
    printf("-- DEBUG: IEQ -> first: %d | second:%d result: %d\n", first, second, result);
    pushExeInt(exe, result);
    
}

void REQ(ExeStack* exe) {
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    int result = first == second;
    printf("-- DEBUG: REQ -> first: %f | second:%f result: %d\n", first, second, result);
    pushExeInt(exe, result);
    
}

void SEQ(ExeStack* exe) {
    char* first = pullExeString(exe);
    char* second = pullExeString(exe);
    int result = strcmp(first, second);
    printf("-- DEBUG: SEQ -> first: %s | second:%s result: %d\n", first, second, result);
    if(result == 0) pushExeInt(exe, 1);
    else pushExeInt(exe, 0);
    
}

void EQU(ExeStack* exe) {
    char* tipo = exe->pointer - 1;
    switch(*tipo) {
        case CHAR_T: CEQ(exe); break;
        case INT_T: IEQ(exe); break;
        case REAL_T: REQ(exe); break;
        case STR_T: SEQ(exe); break;
    }
}
/**
 * Metodo che nega il risultato della EQU //adesso lo facciamo così poi vedremo come ottimizzarlo
 */
 
void NEQ(ExeStack* exe) {
    EQU(exe);
    int result = pullExeInt(exe);
    result = result == 0 ? 1 : 0;
    pushExeInt(exe, result);
}


// ---------------------------------OPERATORI DI CONFRONTO--------------------------------


/**
 * Operatori di confronto riguardanti i caratteri, trattati come interi
 */
void CGT(ExeStack* exe) {
   
    char first = pullExeChar(exe);
    char second = pullExeChar(exe);
    if (first > second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void CGE(ExeStack* exe) {
   
    char first = pullExeChar(exe);
    char second = pullExeChar(exe);
    if (first >= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void CLT(ExeStack* exe) {
   
    char first = pullExeChar(exe);
    char second = pullExeChar(exe);
    if (first < second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void CLE(ExeStack* exe) {
   
    char first = pullExeChar(exe);
    char second = pullExeChar(exe);
    if (first <= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

/**
 * Operatori di confronto riguardanti interi
 */
 
 
void IGT(ExeStack* exe) {
   
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    if (first > second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void IGE(ExeStack* exe) {
   
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    if (first >= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void ILT(ExeStack* exe) {
   
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    if (first < second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void ILE(ExeStack* exe) {
   
    int first = pullExeInt(exe);
    int second = pullExeInt(exe);
    if (first <= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

/**
 * Metodi di confronto applicati ai reali
 */
 
void RGT(ExeStack* exe) {
   
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    if (first > second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void RGE(ExeStack* exe) {
   
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    if (first >= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void RLT(ExeStack* exe) {
   
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    if (first < second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

void RLE(ExeStack* exe) {
   
    float first = pullExeReal(exe);
    float second = pullExeReal(exe);
    if (first <= second)
        pushExeInt(exe, 1);
    else
        pushExeInt(exe, 0);
    return;
}

/**
 * Metodi di confronto applicati alle stringhe utilizzanto strcmp
 */
 
void SGT(ExeStack* exe) {
   
    char* first = pullExeString(exe);
    char* second = pullExeString(exe);
    int result = strcmp(first, second) <= 0 ? 0 : 1;
    pushExeInt(exe, result);
    return;
}

void SGE(ExeStack* exe) {
   
    char* first = pullExeString(exe);
    char* second = pullExeString(exe);
    int result = strcmp(first, second) < 0 ? 0 : 1;
    pushExeInt(exe, result);
    return;
}

void SLT(ExeStack* exe) {
   
    char* first = pullExeString(exe);
    char* second = pullExeString(exe);
    int result = strcmp(first, second) >= 0 ? 0 : 1;
    pushExeInt(exe, result);
    return;
}

void SLE(ExeStack* exe) {
   
    char* first = pullExeString(exe);
    char* second = pullExeString(exe);
    int result = strcmp(first, second) > 0 ? 0 : 1;
    pushExeInt(exe, result);
    return;
}

//---------------------------------------------------METODI DI CASTING-----------------------

/** 
 * Cast tra intero e reale
 */
 
void TOINT(ExeStack* exe){
    float number = pullExeReal(exe);
    int casted = number;
    pushExeInt(exe, casted);
}

/** 
 * Cast da reale a intero
 */

void TOREAL(ExeStack* exe){
    int number = pullExeInt(exe);
    float casted = number;
    pushExeReal(exe, casted);
}


/**
* Visualizza il contenuto dei primi n byte di memoria
*/
void debugExe(ExeStack* data) {
    printf("-- DEBUG EXECUTION STACK ->\n");
    void* p;
    int i, n = 15, memUsed = getMemoryExeUsage(data);
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

// --------------------Per ora non servono----------------------- sono contento di questo dai, anche io dai
#define ERRORE_GET_T1 "!ERRORE! non gestito: getTypeExeHeadRecoursive(): trovato un tipo inesistente!\n"
#define ERRORE_GET_T2 "!ERRORE! parzialmente gestito: getTypeExeHeadRecoursive():  out of bounds!\n"
/**
 * Funzione che scorre la "lista di tipi"
*/
char getTypeExeHeadRecoursive(ExeStack* data, void* index) {
    /**
    * il valore puntato deve assolutamente 
    * essere un tipo, a seconda del tipo si procede al prossimo tipo 
    * incrementando il puntatore della dimensione del tipo
    */
    char* tipo = index;
    index++;
    switch(*tipo) {
        case CHAR_T: index ++; break;
        case INT_T: index = index + sizeof(int); break;
        case REAL_T: index = index + sizeof(float); break;
        case STR_T: index = index + sizeof(char*); break;
        default: printf(ERRORE_GET_T1);
    }
    /**
    * Se il puntatore punta alla prima posizione libera di memoria allora 
    * significa che è l'ultimo valore salvato di cui bisogna ritornare il tipo
    */
    if(index == getExePointer(data)) return *tipo;
    // Caso in cui l'index supera il pointer
    if((index - getExeStart(data)) > getMemoryExeUsage(data)) {
        printf(ERRORE_GET_T2);
        return 0;
    }
    // Non è ancora l'ultimo valore
    return getTypeExeHeadRecoursive(data, index);
}

/**
 * Ritorna il tipo dell'ultimo elemento aggiunto allo stack: si appoggia alla 
 * funzione getTypeExeHeadRecoursive <- (fa partire lo scorrimento dello stack 
 * dal valore più vecchio inserito)
*/
char getTypeExeHead(ExeStack* data) {
    if(getMemoryExeUsage(data) == 0) return 0;
    return getTypeExeHeadRecoursive(data, getExeStart(data));
}