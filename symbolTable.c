//
// Created by maria on 24/05/19.
//
#include <string.h>
#include "def.h"
#include "symbolTable.h"


int n;
int i=0;

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

void addLine(PLine l){
    symbolTable[i]=l;
    i++;
}

PLine newLine(){
    PLine l;
    l=(PLine) malloc(sizeof(Line));
    addLine(l); // In questo modo aggiungo il puntatore alla linea direttamente alla tabella quando la creo
    return(l);
}

void displayTable(){

    printf("\nName        Type   \t Number of param    \tClass\n");

    for(int a=0; a<i; a++){
        printf("%s        \t%s \t\t\t%d   \t\t\t %s\n", symbolTable[a]->id, symbolTable[a]->type, symbolTable[a]->numParam, symbolTable[a]->class );
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
}

