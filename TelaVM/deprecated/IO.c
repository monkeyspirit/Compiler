#include <stdlib.h>
#include <stdio.h>
#include "OpList.h"


void funzione(char* instruction, int n_row) {
    printf("%d->\t%s", n_row, instruction);
    char* p = instruction;
    char* string;
    string = calloc(strlen(instruction), sizeof(char));
    int command, arg2, index = 0;
    command = strtol(instruction, &p, 10);
    if(command == 0) return;
    printf("\t%d\t%c\n", *(p+1), *(p+1));
    
}

int main() {
    
    FILE *fd;
    char buf[20];
    char *res;
    
    // Apri file
    fd = fopen("file.txt", "r");
    
    // Controlla che il file esista
    if(fd == NULL) {
        perror("Errore in apertura del file");
        exit(1);
    }
    
    int more = 1, index = 0;
    while(more) {
        res = fgets(buf, 200, fd);
        if( res==NULL )
            more = 0;
        else { funzione(buf, index); index ++; }
    }
    
    // Chiudi file
    fclose(fd);
    
    return 0;
}