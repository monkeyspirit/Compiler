#include"def.h"
#include "syntaxTree.h"
#include "semantic.h"

// extern FILE *yyout;

char* tabTypes[]=
        {
                "CHAR",//0
                "INT",//1
                "REAL",//2
                "STRING",//3
                "BOOL",//4
                "VOID",//5
                "CHARCONST",//6
                "INTCONST",//7
                "REALCONST",//8
                "STRCONST",//9
                "BOOLCONST",//10
                "ID",//11
                "NONTERMINAL",//12
                "AST",//13
                "FRAC",//14
                "MINUS",//15
                "PLUS",//16
                "NOT",//17
                "LE",//18
                "GE",//19
                "LT",//20
                "GT",//21
                "EQ",//22
                "NEQ",//23
                "AND",//24
                "OR",//25
                "NULL"//26
        };

char* tabNonTerm[] =
        {
                "PROGRAM",//0
                "MODULE_DECL",//1
                "OPT_PARAM_LIST",//2
                "PARAM_LIST",//3
                "PARAM_DECL",//4
                "TYPE",//5
                "OPT_VAR_SECT",//6
                "DECL_LIST",//7
                "DECL",//8
                "ID_LIST",//9
                "OPT_CONST_SECT",//10
                "CONST_LIST",//11
                "CONST_DECL",//12
                "OPT_MODULE_LIST",//13
                "MODULE_BODY",//14
                "STAT_LIST",//15
                "STAT",//16
                "ASSIGN_STAT",//17
                "IF_STAT",//18
                "OPT_ELSEIF_STAT_LIST",//19
                "OPT_ELSE_STAT",//20
                "WHILE_STAT",//21
                "RETURN_STAT",//22
                "OPT_EXPR",//23
                "READ_STAT",//24
                "WRITE_STAT",//25
                "EXPR_LIST",//26
                "EXPR",//27
                "EXPR1",//28
                "BOOLOP",//29
                "BOOL_TERM",//30
                "RELOP",//31
                "REL_TERM",//32
                "REL_TERM1",//33
                "LOW_BINOP",//34
                "LOW_TERM",//35
                "LOW_TERM1",//36
                "HIGH_BINOP",//37
                "FACTOR",//38
                "UNARYOP",//39
                "CONSTANT",//40
                "MODULE_CALL",//41
                "OPT_EXPR_LIST",//42
                "COND_EXPR",//43
                "OPT_ELSEIF_EXPR_LIST"//44
        };



void syntaxTree(Pnode root){
    char *outTreeFileName = malloc(sizeof(char*));
    sprintf(outTreeFileName, "%s.syntaxtree", telaFileName);
     treePrint(fopen(outTreeFileName, "w"), root, 0);
}

void treePrint(FILE* file, Pnode root, int indent)
{
	int i;
	Pnode p;
  	
	for(i=0; i<indent; i++)
    	fprintf(file, "    ");
 
	fprintf(file, "%s", (root->type == T_NONTERMINAL ? tabNonTerm[root->value.ival] : tabTypes[root->type]));

	if(root->type == T_ID ){
        fprintf(file, " (%s)", root->value.sval);
	}
	else if(root->type == T_CHARCONST){

        fprintf(file, " (%s)", root->value.sval);
	}
	else if(root->type == T_STRCONST){

        fprintf(file, " (%s)", root->value.sval);
	}
	else if (root->type == T_INTCONST){

		fprintf(file, " (%d)", root->value.ival);
	}
	else if (root->type == T_BOOLCONST){

		fprintf(file, " (%s)", (root->value.ival == 0? "false" : "true"));
	}
	else if (root->type == T_REALCONST){

		fprintf(file, " (%f)", root->value.rval);
	}
	else if(root->type == T_RETURNULL){
		fprintf(file,"\n");
	}


	fprintf(file, "\n");
	for(p=root->child; p != NULL; p = p->brother){
		treePrint(file, p, indent+1);
	}
    


}