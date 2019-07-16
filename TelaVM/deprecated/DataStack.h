#define DIM_STACK 1000


/** Struttura di contenimento di costanti e variabili dei record di attivazione.
* *start: puntatore alla PRIMA area di memoria dello stack.
* *pointer: puntatore alla prima area di memoria non occupata.
* Nei record di attivazione verra' salvato il puntatore all'area di memoria
* corrispondente alla prima variabile che viene inizializzata nel modulo
*/
typedef struct
{
    int dimension;
    void *start;
    void *pointer;
    void *freeSpace;
}DataStack;

/** Costruttore di DataStack.
* Allocati DIM blocchi di memoria con *p = malloc(n) (funzione che ritorna un
* puntatore *p ad n caratteri consecutivi).
* Salva il primo indirizzo di memoria e inizializza il puntatore.
* Ritorna un oggetto Datastack.
*/
DataStack newDataStack();

/** Funzione di DataStack.
* Aggiunge una variabile di tipo carattere a DataStack: tipo 'c' e valore passato.
*/
void pushChar(DataStack *data, char value);

/** Funzione di DataStack.
* Aggiunge una variabile di tipo intero a DataStack: tipo 'i' e valore passato.
*/
void pushInt(DataStack *data, int value);

/** Funzione di DataStack
* Aggiunge una variabile di tipo stringa a DataStack: tipo 's' e valore passato.
*/
void pushString(DataStack *);

/** Funzione di DataStack
* Aggiunge una variabile di tipo float a DataStack: tipo 'f' e valore passato
*/
void pushFloat(DataStack *data, float value);

/** Funzione di DataStack
* Ritorna la dimensione attuale della memoria utilizzata, facendo
* la differenza tra i puntatori "pointer" e "start"
*/
int getMemoryUsage(DataStack* data);

/** Funzione di Datastack
* Ritorna il puntatore alla prima posizione libera nella memoria
*/
void* getPointer(DataStack* data);

DataStack newDataStack()
{
    DataStack data;
    data.start = malloc(DIM_STACK);
    data.pointer = data.start;
    return data;

}

void pushChar(DataStack *data, char value)
{
    char *charac;
    charac = data->pointer;
    *charac = value;
    // Operazione equivalente: *data.pointer++;
    data->pointer += sizeof(char);
}

void goAhead(DataStack* data, int index){
    int i;
    for (i = 0; i<index; i++){
        char* c = data->pointer;
        data->pointer++;
        switch (atoi(c)){
            case 'i' : data->pointer += sizeof(int);
            case 'c' : data->pointer += sizeof(char);
            case 's' : data->pointer += sizeof(char*);
            case 'r' : data->pointer += sizeof(float);
        }
    }
}

void pushInt(DataStack *data, int value)
{
    char *c;
    // -> perch� data � un puntatore
    // operazione equivalente: c=*data.pointer;
    c = data->pointer;
    // Salvo il carattere nella prima posizione disponibile
    *c = 'i';
    // Dato che ho scritto solamente 8bit incremento il puntatore (dafault: 8bit)
    data->pointer++;
    int *integer;
    integer = data->pointer;
    *integer = value;
    // Dopo aver salvato il tipo e il valore incremento il puntatore alla prossima
    // area libera di 4Byte
    data->pointer += sizeof(int);
}

void* pushString(DataStack *data)
{
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
}

void pushFloat(DataStack *data, float value)
{
    char *c;
    // -> perch� data � un puntatore
    // operazione equivalente: c=*data.pointer;
    c = data->pointer;
    // Salvo il carattere nella prima posizione disponibile
    *c = 'f';
    // Dato che ho scritto solamente 8bit incremento il puntatore (dafault: 8bit)
    data->pointer++;
    float *real;
    real = data->pointer;
    *real = value;
    // Dopo aver salvato il tipo e il valore incremento il puntatore alla prossima
    // area libera di 4Byte
    data->pointer += sizeof(float);
}

void* initVar(char type){
    
    char* c = data->pointer;
    *c = type;
    data->pointer++;
}

int getMemoryUsage(DataStack* data)
{
    return (int) data->pointer - (int) data->start;
}

void* getPointer(DataStack* data)
{
    return data->pointer;
}

void setPointer(DataStack* data, void* pointer){
    data->pointer = pointer;
}

void setPointerToFree(DataStack* data){
    data->pointer = data->freeSpace;
}
void debug(DataStack* data)
{
    void* p;
    int i, n = 20;
    for(i = 0; i < n; i++){
        p = data->start;
        p += i;
        char *c = p;
        printf("%d:\t%c\tchar->ashii\t%d\n", i, *c, *c);
    }
    printf("Dimensione memoria utilizzata: %d", getMemoryUsage(data));
}
/**
void* get(DataStack* data, int index, void* startPoint)
{
    if (index == 0)
        return startPoint++;
    char* startChar = startPoint;
    switch (*startChar)
    {
        case 'i': startPoint+=sizeof(int)+1; get(data, index--, startPoint);break;
        case 'f': startPoint+=sizeof(float)+1; get(data, index--, startPoint);break;
        case 'c': startPoint+=sizeof(char)+1; get(data, index--, startPoint);break;
        case 'b': startPoint+=sizeof(int)+1; get(data, index--, startPoint);break;
        case 's': jumpString()
    }
}*/
