%{
#include "def.h"
#define YYSTYPE PNODE
PNODE root = NULL;
extern Lexval lexval;
%}

%token 

%%


%%

int yylex()
{
	int c;
	c = getchar();
	if (isdigit(c)) {
		yylval = c -'0';
		return()
	}
}

void yyerror()
{
	fprintf(stderr, "Syntax Error\n");
}

void main()
{
	yyparse();
}