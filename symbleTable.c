//
// Created by maria on 24/05/19.
//

#include "def.h"


int n;
int j=0;
int lookahead;
extern char *yytext;
extern FILE *yyin;



char* TreeTypes[]=
{
        "CHAR",
        "INT",
        "REAL",
        "STRING",
        "BOOL",
        "VOID",
        "CHARCONST",
        "INTCONST",
        "REALCONST",
        "STRCONST",
        "BOOLCONST",
        "ID",
        "NONTERMINAL",
        "AST",
        "FRAC",
        "MINUS",
        "PLUS",
        "NOT",
        "LE",
        "GE",
        "LT",
        "GT",
        "EQ",
        "NEQ",
        "AND",
        "OR",
        "NULL"
};

char* TreeNonTerm[] =
{
        "PROGRAM",
        "MODULE_DECL",
        "OPT_PARAM_LIST",
        "PARAM_LIST",
        "PARAM_DECL",
        "TYPE",
        "OPT_VAR_SECT",
        "DECL_LIST",
        "DECL",
        "ID_LIST",
        "OPT_CONST_SECT",
        "CONST_LIST",
        "CONST_DECL",
        "OPT_MODULE_LIST",
        "MODULE_BODY",
        "STAT_LIST",
        "STAT",
        "ASSIGN_STAT",
        "IF_STAT",
        "OPT_ELSEIF_STAT_LIST",
        "OPT_ELSE_STAT",
        "WHILE_STAT",
        "RETURN_STAT",
        "OPT_EXPR",
        "READ_STAT",
        "WRITE_STAT",
        "EXPR_LIST",
        "EXPR",
        "EXPR1",
        "BOOLOP",
        "BOOL_TERM",
        "RELOP",
        "REL_TERM",
        "REL_TERM1",
        "LOW_BINOP",
        "LOW_TERM",
        "LOW_TERM1",
        "HIGH_BINOP",
        "FACTOR",
        "UNARYOP",
        "CONSTANT",
        "MODULE_CALL",
        "OPT_EXPR_LIST",
        "COND_EXPR",
        "OPT_ELSEIF_EXPR_LIST"
};



char* tableTypes[]=
{
        "REAL",
        "INT",
        "VOID",
        "CHAR",
        "STRING",
        "BOOL"
};

char* tableClass[]={
    "VAR",
    "MOD",
    "CON",
    "PAR"
};

void insertTable(char* name, SymbClass class, SymbType type, Lexval value) {

    symboleTable[j].name=name;
    symboleTable[j].class=class;
    symboleTable[j].type=type;
    symboleTable[j].value.ssymb=value.sval;

    j++;
}


void displayTable(){

    printf("\nName \tClass \tType \tValue\n");

    for(int i = 0; i<j; i++){
        if(symboleTable[i].type==TY_REAL){
            printf("%s\t %s\t %s\t %f\n", symboleTable[i].name, tableClass[symboleTable[i].class], tableTypes[symboleTable[i].type], symboleTable[i].value.rsymb);
        }
        else if(symboleTable[i].type==TY_INT){
            printf("%s\t %s\t %s\t %d\n",symboleTable[i].name, tableClass[symboleTable[i].class], tableTypes[symboleTable[i].type], symboleTable[i].value.isymb);
        }
        else if(symboleTable[i].type== TY_CHAR || symboleTable[i].type==TY_STRING){
            printf("%s\t %s\t %s\t %s\n",symboleTable[i].name, tableClass[symboleTable[i].class], tableTypes[symboleTable[i].type], symboleTable[i].value.ssymb);
        }
        else if(symboleTable[i].type==TY_BOOL){
            printf("%s\t %s\t %s\t %d\n",symboleTable[i].name, tableClass[symboleTable[i].class],tableTypes[symboleTable[i].type], symboleTable[i].value.isymb);
        }
        else if(symboleTable[i].type==TY_VOID){
            printf("%s\t %s\t %s\t %d\n",symboleTable[i].name,tableClass[symboleTable[i].class], tableTypes[symboleTable[i].type], symboleTable[i].value.isymb);
        }
    }


}

/*Questo metodo legge l'albero sintattico stampato su file ed estrae le variabili, i moduli, le costanti e i parametri*/
void fillTable(){


}