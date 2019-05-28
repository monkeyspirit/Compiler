//
// Created by maria on 24/05/19.
//

#include "def.h"


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

    printf("\nName \tType \tNumber of param \tClass\n");

    for(int a=0; a<i; a++){
        printf("%s \t\t%s \t\t\t%d \t\t\t%s\n", symbolTable[a]->id, symbolTable[a]->type, symbolTable[a]->numParam, symbolTable[a]->class );
    }

}

void programLine (Pnode p){
    module_declLine(p->child);                  //passo MODULE-DECL
}


void module_declLine(Pnode p){
    Pnode d, h, q, w, e;
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
        decl_listLines(type_decl, h->child);      //PASSO ALLA FUNZIONE IL TIPO E ID_LIST

        h=h->brother;
        while(h!=NULL){

            int type_decl = h->child->brother->child->type;
            decl_listLines(type_decl, h->child);
            h=h->brother;
        }

        d = d->brother;                           //PUNTO A OPT-VAR/OPT-CONST/OPT-MODULE/TYPE
    }

    //----------- VERIFICA PRESENZA OPT-CONST-LIST -----------


    //----------- VERIFICA PRESENZA OPT-MODULE-LIST -----------


    //----------- ANALISI MODULE BODY


    displayTable();
}

void param_declLine(Pnode p){
    PLine l;
    l = newLine();

    //----------- RICONOSCIMENTO ID E TYPE DEL PARAMETRO
    l->id = p->value.sval;
    l->type=tableTypes[p->brother->child->type];
    l->class=tableClass[3];


}

void decl_listLines(int type, Pnode p){

    decl_Line(type, p->child);              //p punta ad ID_LIST, p->child punta ad ID
    p = p->child->brother;                  // p->child->brother punta al secondo ID della lista se esiste
    while(p!=NULL){
        decl_Line(type, p);
        p = p->brother;
    }

}

void decl_Line(int type, Pnode p){

    PLine l;
    l = newLine();

    l->type = tableTypes[type];
    l->class = tableClass[0];
    l->id = p->value.sval;
}



