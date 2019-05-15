#include "def.h"
extern char *yytext;
extern Lexval lexval;
extern int line;
extern FILE *yyin;

int lookahead;

Pnode root = NULL;

void next()
{
	lookahead = yylex();
}

void match(int symbol)
{
	if(lookahead == symbol){
		next();
	}
	else{
		parserror();
	}
}

void parserror()
{
	fprintf(stderr, "Line %d: syntax error on symbol \"%s\"\n", line, yytext );
	exit(-1);
}

Pnode newnode()

