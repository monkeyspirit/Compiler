#include <stdio.h>
#include <stdlib.h>

typedef union {
    int integer;
    float real;
    char* string;
    char charap;
} Type;

typedef struct {
    Type data;
    int dimension;
} Union;

typedef struct {
    Union* array;
    int size;
    int dimension;
    int index;
}DataArray;

int getStackSize(DataArray* s) {
    return s->size;
}

void setDataSize(DataArray* s, int size) {
    s->size = size;
}

int getStackDimension(DataArray* s) {
    return s->dimension;
}

DataArray newDataArray() {
    DataArray S;
    S.array = calloc(STEP, sizeof(Union));
    S.size = 0;
    S.dimension = STEP;
    S.index = 0;
    return S;
}

void increaseSize(DataArray* s) {
    s->size++;
    if(s->size >= s->dimension) {
        s->dimension+=STEP;
        s->array = realloc(s->array, sizeof(Union)*s->dimension);
    }
}

void setDAUnion(DataArray* DA, Union value) { DA->array[DA->index] = value; DA->index++; }

Union getDAUnion(DataArray* DA) {  return DA->array[DA->index]; }

void increaseDAIndex(DataArray* DA) {  DA->index++; }

void moveDAIndexToLastPosition(DataArray* DA) {  DA->index = DA->size; }

void allocDataArray(DataArray* DA, int dimension ) {
    Union u;
    u.dimension = dimension;
    DA->array[DA->index] = u;
    DA->index++;
}

