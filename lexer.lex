%{
#include "def.h"

int line =	1;
Lexval value;	
%}
%option noyywrap

delimiter		[ \t]
comment			"#"(.)*\n
spacing			{delimiter}+
uppercase		[A-Z]
lowercase		[a-z]
letter			{uppercase}|{lowercase}
digit			[0-9]
initialdigit	[1-9]
charconst		\'{letter}\'
sgn 			"-"
intconst		{sgn}?{initialdigit}{digit}*|0
realconst		{intconst}\.{digit}?
strconst		\"[^\n\"]*\"
boolconst		false|true 
id 				{letter}({letter}|{digit})*

%%

{spacing}		;
{comment}		;
\n 				{line++;}
module			{return(MODULE);}
"("				{return(LBRACE);}
")"				{return(RBRACE);}
":"				{return(COLON);}
";"				{return(SEMICOLON);}
","				{return(COMMA);}
char 			{return(CHAR);}
int 			{return(INT);}
real 			{return(REAL);}
string			{return(STRING);}
bool 			{return(BOOL);}
void			{return(VOID);}
var				{return(VAR);}
const 			{return(CONST);}
begin			{return(BEGIn);}
end				{return(END);}
if				{return(IF);}
then			{return(THEN);}
else 			{return(ELSE);}
elseif 			{return(ELSEIF);}
while			{return(WHILE);}
do				{return(DO);}
return 			{return(RETURN);}
read			{return(READ);}
write			{return(WRITE);}
and				{return(AND);}
or				{return(OR);}
not				{return(NOT);}
"="				{return(ASSIGN);}
"<="			{return(LE);}
"<"				{return(LT);}
">"				{return(GT);}
">="			{return(GE);}
"=="			{return(EQ);}
"!="			{return(NEQ);}
"+"				{return(PLUS);}
"-"				{return(MINUS);}
"*"				{return(AST);}
"/"				{return(FRAC);}
{intconst}		{value.ival = atoi(yytext); return(INTCONST);}
{realconst}		{value.rval = atof(yytext); return(REALCONST);}
{charconst}		{value.sval = newstring(yytext); return(CHARCONST);}
{strconst}		{value.sval = newstring(yytext); return(STRCONST);}
{boolconst}		{value.ival = (yytext[0]=='f' ? 0 : 1); return(BOOLCONST);}
{id}			{value.sval = newstring(yytext); return(ID);}
.				{return(ERROR);}

%%


char *newstring(char *s) {
	char *p;
	p = malloc(strlen(s)+1);
	strcpy(p, s);
	return(p);
}
