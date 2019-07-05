#include"def.h"
#include "syntaxTree.h"

// extern FILE *yyout;

char* tabTypes[]=
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

char* tabNonTerm[] =
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



void syntaxTree(Pnode root){
    treePrint(fopen(OUTPUT_TREE_FILE, "w"), root, 0);
}

void treePrint(FILE* file, Pnode root, int indent)
{
	int i;
	Pnode p;
  	
	for(i=0; i<indent; i++)
    	fprintf(file, "    ");
 
	fprintf(file, "%s", (root->type == T_NONTERMINAL ? tabNonTerm[root->value.ival] : tabTypes[root->type]));

	if(root->type == T_ID || root->type == T_CHARCONST || root->type == T_STRCONST ){
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