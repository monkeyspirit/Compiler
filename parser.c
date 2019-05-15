#include "def.h"
extern char *yytext;
extern Lexval lexval;
extern int line;
extern FILE *yyin;

int lookahead;

Pnode root = NULL;

/*Metodo per il prossimo carattere*/
void next()
{
	lookahead = yylex();
}

/*Metodo per mangiare il carattere e proseguire*/
void match(int symbol)
{
	if(lookahead == symbol){
		next();
	}
	else{
		parserror();
	}
}

/*Metodo per la generazione della stringa di errore con passaggio di numero di linea*/
void parserror()
{
	fprintf(stderr, "Line %d: syntax error on symbol \"%s\"\n", line, yytext );
	exit(-1);
}

/*Metodo che crea un nuovo nodo*/
Pnode newnode(Typenode tnode)
{
	Pnode p;
	p = (Pnode) malloc(sizeof(Node));
	p->type = tnode; /*Creo il nodo e come tipo gli associo il tipo passato*/
	p->child = p->brother = NULL; /*Appena creato non ha figli o fratelli*/
	return(p);
}

/*Metodo che crea un nodo non terminale*/
Pnode nonterminalnode(Nonterminal nonterm)
{
	Pnode p;
	p = newnode(T_NONTERMINAL);
	p->value.ival = nonterm; /*Regole di stato del parsing*/
}

/*Meotod che crea un nodo associato ad una keyword*/
Pnode keynode(Typenode keyword)
{
	return(newnode(keyword));
}

/*Meotod che crea un nodo associato ad un id*/
Pnode idnode()
{
	Pnode p;
	p = newnode(T_ID);
	p->value.sval = lexval.sval; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo char*/
Pnode charconstnode()
{
	Pnode p;
	p = newnode(T_CHARCONST);
	p->value.sval = lexval.sval; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo int*/
Pnode intconstnode()
{
	Pnode p;
	p = newnode(T_INTCONST);
	p->value.ival = lexval.ival; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo real*/
Pnode realconstnode()
{
	Pnode p;
	p = newnode(T_REALCONST);
	p->value.rval = lexval.rval; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo string*/
Pnode strconstnode()
{
	Pnode p;
	p = newnode(T_STRCONST);
	p->value.sval = lexval.sval; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo bool*/
Pnode boolconstnode()
{
	Pnode p;
	p = newnode(T_BOOLCONST);
	p->value.ival = lexval.ival; /*Regole di stato del parsing*/
	return(p);
}

/*I metodi da qui in giù servono per il matching al fine di creare un albero sintattico*/
/*Ogni metodo fa il match dell'espressione regolare a cui fa riferimento (vedi il nome e la BNF)*/

Pnode program()
{
	Pnode p;
	p = nonterminalnode(NMODULE_DECL);
	p->child = module_decl();
	return(p);
}

Pnode module_decl()
{
	Pnode p;
	match(MODULE);
	p = idnode();
	next();
	match(LBRACE);
	p->brother = nonterminalnode(NOPT_PARAM_LIST);
	p->brother->child = opt_param_list();
	match(RBRACE);
	match(COLON);
	p->brother = nonterminalnode(NTYPE);
	p->brother->child = type();
	p->brother = nonterminalnode(NOPT_VAR_SECT);
	p->brother->child = opt_var_sect();
	p->brother = nonterminalnode(NOPT_CONST_SECT);
	p->brother->child = opt_const_sect();
	p->brother = nonterminalnode(NOPT_MODULE_LIST);
	p->brother->child = opt_module_list();
	p->brother = nonterminalnode(NMODULE_BODY);
	p->brother->child = module_body();
}

Pnode opt_param_list()
{
	Pnode p;
	if(lookahead==ID){
		p = nonterminalnode(NPARAM_LIST);
		p->child = param_list();
	}
	return(p);
}

Pnode param_list()
{
	Pnode head, p;
	head = p = nonterminalnode(NPARAM_DECL);
	p->child = param_decl();
	while(lookahead==COMMA){
		match(COMMA);
		p->brother = nonterminalnode(NPARAM_DECL);
		p = p->brother;
		p->child = param_decl();
	}
	return(head);
}

Pnode param_decl()
{
	Pnode p;
	p = idnode();
	next();
	match(COLON);
	p->child = nonterminalnode(NTYPE);
	p->child = type();
	return(p);
}

Pnode type()
{
	Pnode p;
	if(lookahead==CHAR){
		p = keynode(T_CHAR);
		next();
		return(p);
	} 
	else if (lookahead==INT){
		p = keynode(T_INT);
		next();
		return(p);
	} 
	else if (lookahead==REAL){
		p = keynode(T_REAL);
		next();
		return(p);
	}
	else if (lookahead==STRING){
		p = keynode(T_STRING);
		next();
		return(p);
	} 
	else if (lookahead==BOOL){
		p = keynode(T_BOOL);
		next();
		return(p);
	}
	else if (lookahead==VOID){
		p = keynode(T_VOID);
		next();
		return(p);
	}
	else{
		parserror();
	}

}

Pnode opt_var_sect()
{

}

Pnode decl_list()
{

}

Pnode decl()
{

}

Pnode id_list()
{

}

Pnode opt_const_sect()
{

}

const_list(),
const_decl(),

Pnode opt_module_list()
{

}

Pnode module_body()
{

}
stat_list()
stat(),
assign_stat(),
if_stat(),
elseif_stat_list(),
else_stat(),
while_stat(),
return_stat(),
opt_expr(),
read_stat(),
write_stat(),
expr_list(),
expr(),
boolop(),
bool_term(),
relop(),
relterm(),
low_binop(),
low_term(),
high_termop(),
factor(),
unaryop(),
constant(),
module_call(),
opt_expr_list(),
cond_expr(),
opt_elseif_expr_list();

void parse(){
	next();
	root = nonterminalnode(NPROGRAM);
	root->child = program();
}

int main()
{
	yyin = stdin;
	parse();
	treeprint(root, 0);
	return(0);
}