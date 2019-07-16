#include <stdlib.h>
#include <stdio.h>
#define REALLOC_STEP 30

typedef struct stringhash{
    char* pointer;
    int key;
}HashString;

typedef struct stringdb{
    HashString* strings;
    int size;
    int dimension;
}StringDB;

int getHash (const char* word)
{
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + word[i];
    }
    return hash;
}

HashString newHashString( int hash, char* pointer ){
    HashString hs;
    hs.key = hash;
    hs.pointer = malloc(sizeof(pointer));
    strcpy(hs.pointer, pointer);
    return hs;
}

StringDB newStringDB () {
    StringDB sdb;
    sdb.strings = malloc(sizeof(HashString)*REALLOC_STEP);
    sdb.size = 0;
    sdb.dimension = REALLOC_STEP;
    return sdb;
}

char* getStringPointer(StringDB* stringDB, char* string){
    int hash = getHash(string);
    int i;
    for ( i = 0; i < stringDB->size; i++ ) {
        if ( stringDB->strings[i].key == hash)
            return stringDB->strings[i].pointer;
    }
    
    if (stringDB->dimension == i) {
        stringDB->dimension+=REALLOC_STEP;
        stringDB->strings=realloc(stringDB->strings, sizeof(HashString)*REALLOC_STEP);
    }
    
    HashString newHash = newHashString ( hash, string );
    stringDB->strings[i] = newHash;
    stringDB->size++;
    return newHash.pointer;
}