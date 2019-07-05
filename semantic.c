//
// Created by maria on 05/07/19.


#include "semantic.h"


/*
 * Controlli:
    1. Uguaglianza del nome del modulo con i nomi che delimitano il corpo del modulo
    2. Definizione delle costanti nella sezione const: compatibilità del valore con il tipo dichiarato
    3. Visibilità degli identificatori referenziati (nella gerarchia degli ambienti)
    4. Compatibilità in numero e tipo dei parametri attuali con i parametri formali
    5. Compatibilità delle espressioni di ritorno con il tipo del valore di ritorno del modulo
    6. Istruzione di return espressa sempre nel modulo che restituisce un valore diverso da void
    7. Compatibilità degli operatori con gli operandi
    8. Compatibilità dell'identificatore con l'espressione di assegnamento
    9. Compatibilità delle espressioni con le istruzioni in cui sono coinvolte
 * Type inference: computazione del tipo risultante di ogni operazione
 */

//1. Uguaglianza del nome del modulo con i nomi che delimitano il corpo del modulo
void moduleNameControl(char* h, char* q, char* id){
    if(strcmp(h,q)){
        printf("Errore gli ID non corrispondono: \"begin %s ... end %s\"", h, q);
        exit(-2);
    }
    else{
        if(strcmp(h, id)){
            printf("Errore gli ID non corrispondono: \"module %s ... begin %s\"",id, h);
            exit(-2);
        }
        else if(strcmp(q,id)){
            printf("Errore gli ID non corrispondono: \"module %s ... end %s\"", id, q);
            exit(-2);
        }
    }
}

//2. Definizione delle costanti nella sezione const: compatibilità del valore con il tipo dichiarato
void constantDeclaration(int type, Pnode h){

    char* id = h->child->child->child->value.sval;
    Pnode expr = h->child->brother;

    while(expr->child!=NULL){
        expr = expr->child;
    }

    switch (type){
        case T_CHAR:
            if(expr->type!=T_CHARCONST  && expr->type!=T_ID ){
                printf("Errore: id \"%s\" ci si aspetta un CHAR\n", id);
                exit(-3);
            }
            break;
        case T_INT:
            if(expr->type!=T_INTCONST){
                printf("Errore: id \"%s\" ci si aspetta un INT\n", id);
                exit(-3);
            }
            break;
        case T_REAL:
            if(expr->type!=T_REALCONST){
                printf("Errore: id \"%s\" ci si aspetta un REAL\n", id);
                exit(-3);
            }
            break;
        case T_STRING:
            if(expr->type!=T_STRCONST){
                printf("Errore: id \"%s\" ci si aspetta un STRING\n", id);
                exit(-3);
            }
            break;
        case T_BOOL:
            if(expr->type!=T_BOOLCONST){
                printf("Errore: id \"%s\" ci si aspetta un BOOL\n",id);
                exit(-3);
            }
            break;
    }

}


//3. Visibilità degli identificatori referenziati (nella gerarchia degli ambienti)

//4. Compatibilità in numero e tipo dei parametri attuali con i parametri formali

//5. Compatibilità delle espressioni di ritorno con il tipo del valore di ritorno del modulo

//6. Istruzione di return espressa sempre nel modulo che restituisce un valore diverso da void

//7. Compatibilità degli operatori con gli operandi

//8. Compatibilità dell'identificatore con l'espressione di assegnamento

//9. Compatibilità delle espressioni con le istruzioni in cui sono coinvolte
