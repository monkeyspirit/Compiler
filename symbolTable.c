//
// Created by maria on 24/05/19.
//
#include <string.h>
#include "def.h"
#include "symbolTable.h"
#define TOT 100;

int n;
extern FILE *yyin;


char* tableTypes[]=
{
        "CHAR",
        "INT",
        "REAL",
        "STRING",
        "BOOL",
        "VOID"
};

char* tableClass[]={
    "VAR",
    "MOD",
    "CON",
    "PAR"
};

/*
 * Metodi di utilità per l'analisi e inserimento
 */

//Metodo che calcola l'hash
int hash(char* id)
{
    int i, h=0;
    int tot=TOT;

    for(i=0; id[i] != '\0'; i++) {
        h = ((h << tot) + id[i]) % TOT;
    }
    return h;
}

//Metodo che dice se a quell'indice di hash c'è già un elemento:
// - 1 se c'è qualcuno
// - 0 se non c'è nessuno
int search(int hash){

    if(symbolTable[hash]!=NULL){
        return 1;
    }
    else{
        return 0;
    }

}


void addLine(PLine l, char* id){

    int index = hash(id);
    PLine p;
    int flag = search(index);
    if(flag==1){
        p = symbolTable[index];
        while(p->next!=NULL){
            p = p->next;
        }
        p->next = l;
    }
    else{
        symbolTable[index]=l;
    }

}



/*
 * Metodi per analizzare l'albero
 */

PLine newLine(){
    PLine l;
    l=(PLine) malloc(sizeof(Line));
    return(l);
}

void displayTable(){

    printf("\nName        Type   \t Number of param    \tClass \tHash\n");

    int tot = TOT;

    for(int a=0; a<tot; a++){
        if(symbolTable[a]!=NULL){
            int r = hash(symbolTable[a]->id);
            printf("%s        \t%s \t\t\t%d   \t\t\t %s \t %d\n", symbolTable[a]->id, symbolTable[a]->type, symbolTable[a]->numParam, symbolTable[a]->class, r );
            PLine next = symbolTable[a]->next;
            while(next!=NULL){
                int r = hash(next->id);
                printf("\t\t%s        \t%s \t\t\t%d   \t\t\t %s \t %d\n",next->id, next->type, next->numParam, next->class, r );
                next=next->next;
            }
        }
    }

}

void programLine (Pnode p){

    yyin = fopen("../out", "r");
    module_declLine(p->child);                  //passo MODULE-DECL
}


void module_declLine(Pnode p){
    Pnode d, h, q;
    PLine l;
    l = newLine();

    p = p->child;                               //PUNTO AD ID
    //----------- RICONOSCIMENTO ID, CLASS -----------

    l->class = tableClass[1];
    l->id = p->value.sval;

    //Inserisco nella symbol table
    addLine(l, l->id);

    d = p->brother;                             //PUNTO AD OPT_PARAM_LIST se esiste

    int j=0;

    //----------- VERIFICA PRESENZA OPT-PARAM-LIST -----------

    if(d->value.ival==NOPT_PARAM_LIST){

        h = d->child->child;                      //PUNTO AL PRIMO PARAM_DECL
        param_declLine(h->child);                 //h->child è ID
        j++;
        while(h->brother!=NULL){
            j++;
            h = h->brother;                       //PUNTO AI VARI PARAM_DECL
            param_declLine(h->child);
        }

        d = d->brother;                           //PUNTO A TYPE
    }

    l->numParam = j;

    l->type = tableTypes[d->child->type];         //PUNTO AL TIPO CHE STA SOTTO TYPE (COME FIGLIO)
    d = d->brother;                               //PUNTO AL FRATELLO CHE PUO ESSERE OPT-VAR OPT-CONST OPT-MODULE MODULE BODY



    //----------- VERIFICA PRESENZA OPT-VAR-LIST -----------

    if(d->value.ival==NOPT_VAR_SECT){

        h = d->child->child;                      //PUNTO AL PRIMO DECL con h
        int type_decl = h->child->brother->child->type;
        vardecl_listLines(type_decl, h->child);      //PASSO ALLA FUNZIONE IL TIPO E ID_LIST

        h=h->brother;
        while(h!=NULL){

            int type_decl = h->child->brother->child->type;
            vardecl_listLines(type_decl, h->child);
            h=h->brother;
        }

        d = d->brother;                           //PUNTO A OPT-CONST/OPT-MODULE/MODULE BODY
    }

    //----------- VERIFICA PRESENZA OPT-CONST-LIST -----------

    if(d->value.ival==NOPT_CONST_SECT){

        h = d->child->child;                                   //PUNTO AL PRIMO CONST_DECL con h
        int type_decl = h->child->child->brother->child->type;
        constdecl_listLines(type_decl, h->child->child);       //PASSO ALLA FUNZIONE IL TIPO E ID_LIST

        h=h->brother;
        while(h!=NULL){

            int type_decl = h->child->child->brother->child->type;
            constdecl_listLines(type_decl, h->child->child);
            h=h->brother;
        }

        d = d->brother;                                        //PUNTO A OPT-MODULE/MODULE BODY
    }

    //----------- VERIFICA PRESENZA OPT-MODULE-LIST -----------

    if(d->value.ival==NOPT_MODULE_LIST){

        h = d->child;                                    //PUNTO AL PRIMO MODULE_DECL con h
        module_declLine(h);                              //PASSO ALLA FUNZIONE IL MODULE_DECL

        h=h->brother;
        while(h!=NULL){
            module_declLine(h);
            h=h->brother;
        }

        d = d->brother;                                  //PUNTO A MODULE BODY
    }

    //----------- ANALISI MODULE BODY -----------

    h = d->child;                   //PUNTO A ID DI BEGIN
    q = d->child->brother->brother; //PUNTO A ID DI END

    if(strcmp(h->value.sval,q->value.sval)){
       printf("Errore gli ID non corrispondono: \"begin %s ... end %s\"", h->value.sval, q->value.sval);
       exit(-2);
    }
    else{
        if(strcmp(h->value.sval, l->id)){
            printf("Errore gli ID non corrispondono: \"module %s ... begin %s\"",l->id, h->value.sval);
            exit(-3);
        }
        else if(strcmp(q->value.sval, l->id)){
            printf("Errore gli ID non corrispondono: \"module %s ... end %s\"", l->id, q->value.sval);
            exit(-4);
        }
    }
}

void param_declLine(Pnode p){
    PLine l;
    l = newLine();

    //----------- RICONOSCIMENTO ID E TYPE DEL PARAMETRO
    l->id = p->value.sval;

    //Inserisco nella symbol table
    addLine(l, l->id);

    l->type=tableTypes[p->brother->child->type];
    l->class=tableClass[3];


}

void vardecl_listLines(int type, Pnode p){

    vardecl_Line(type, p->child);              //p punta ad ID_LIST, p->child punta ad ID
    p = p->child->brother;                  // p->child->brother punta al secondo ID della lista se esiste
    while(p!=NULL){
        vardecl_Line(type, p);
        p = p->brother;
    }

}

void vardecl_Line(int type, Pnode p){

    PLine l;
    l = newLine();
    l->type = tableTypes[type];
    l->class = tableClass[0];
    l->id = p->value.sval;

    //Inserisco nella symbol table
    addLine(l, l->id);

}

void constdecl_listLines(int type, Pnode p){

    constdecl_Line(type, p->child);              //p punta ad ID_LIST, p->child punta ad ID
    p = p->child->brother;                  // p->child->brother punta al secondo ID della lista se esiste
    while(p!=NULL){
        constdecl_Line(type, p);
        p = p->brother;
    }

}

void constdecl_Line(int type, Pnode p){

    PLine l;
    l = newLine();
    l->type = tableTypes[type];
    l->class = tableClass[2];
    l->id = p->value.sval;


    //Inserisco nella symbol table
    addLine(l, l->id);

}

