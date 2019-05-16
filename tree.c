#include"def.h"

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
	"BOOLOP",
	"RELOP",
	"LOWBINOP",
	"HIGHBINOP",
	"UNOP",
	"NONTERMINAL"
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

void treePrint(Pnode root, int indent)
{
	int i;
	Pnode p;
  	
	for(i=0; i<indent; i++)
    	printf("    ");
 
	printf("%s", (root->type == T_NONTERMINAL ? tabNonTerm[root->value.ival] : tabTypes[root->type]));

	if(root->type == T_ID || root->type == T_CHARCONST || root->type == T_STRCONST || root->type == T_BOOLOP ||root->type == T_RELOP || root->type == T_LOWBINOP || root->type == T_HIGHBINOP || root->type == T_UNOP){
		printf(" (%s)", root->value.sval);
	}
	else if (root->type == T_INTCONST){
		printf(" (%d)", root->value.ival);
	}
	else if (root->type == T_BOOLCONST){
		printf(" (%s)", (root->value.ival == 0? "false" : "true"));
	}
	else if (root->type == T_REALCONST){
		printf(" (%f)", root->value.rval);
	}

	printf("\n");
	for(p=root->child; p != NULL; p = p->brother){
		treePrint(p, indent+1);
	}
    


}