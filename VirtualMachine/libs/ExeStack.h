#include <stdio.h>
#include <stdlib.h>

typedef struct exestack{ 
    
    Union* data;
    int dimension;
    int pointer;
}ExeStack;
    
ExeStack newExeStack(){
    ExeStack exe;
    exe.data = calloc(STEP, sizeof(Union));
    exe.dimension = STEP;
    exe.pointer = 0;
    return exe;
}


void incPointer( ExeStack* exe ) {
    ExeStack prova = *exe;
    exe->pointer++;
    if ( exe->pointer == exe->dimension) {
        exe->dimension+=STEP;
        exe->data = (Union*) realloc(exe->data, sizeof(Union) * exe->dimension);
    }
}

void pushExeInt( ExeStack* exe, int value ) {
    exe->data[exe->pointer].data.integer = value;
    exe->data[exe->pointer].dimension = 4;
    incPointer(exe);
}

void pushExeReal( ExeStack* exe, float value ) {
    exe->data[exe->pointer].data.real = value;
    exe->data[exe->pointer].dimension = 4;
    incPointer(exe);
}

void pushExeString( ExeStack* exe, char* value ) {
    exe->data[exe->pointer].data.string = value;
    exe->data[exe->pointer].dimension = 8;
    incPointer(exe);
}

void pushExeChar ( ExeStack* exe, char value ) {
    exe->data[exe->pointer].data.charap = value;
    exe->data[exe->pointer].dimension = 1;
    incPointer(exe);
}

int pullExeInt ( ExeStack* exe ) {
    exe->pointer--;
    int result = exe->data[exe->pointer].data.integer;
    return result;
}

float pullExeReal ( ExeStack* exe ) {
    exe->pointer--;
    float result = exe->data[exe->pointer].data.real;
    return result;
}

char* pullExeString ( ExeStack* exe ) {
    exe->pointer--;
    char* result = exe->data[exe->pointer].data.string;
    return result;
}

char pullExeChar ( ExeStack* exe ) {
    exe->pointer--;
    char result = exe->data[exe->pointer].data.charap;
    return result;
}

void pushExeUnion(ExeStack* exe, Union un){
    exe->data[exe->pointer] = un;
    incPointer(exe);
}

Union pullExeUnion( ExeStack* exe ) {
    exe->pointer--;
    Union result = exe->data[exe->pointer];
    return result;
    
}

/**
void debugExe( ExeStack* exe ) {
    int i;
    printf("\n--- DEBUG EXE STACK ->\n");
    for ( i = 0 ; i < exe->pointer ; i++) {
        if (exe->data[i].dimension == 4)
            printf( "pos %d:  %d\n",i, exe->data[i].data.real );
        else if (exe->data[i].dimension == 8)
            printf("pos %d:  %s\n",i, exe->data[i].data.string);
        else 
            printf("pos %d:  %c",i,  exe->data[i].data.charap);
    }
}*/
