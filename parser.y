%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int yylex ();
void yyerror ();

%}



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