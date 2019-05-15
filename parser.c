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
	Pnode p;
	if(lookahead==VAR){
		match(VAR);
		p = nonterminalnode(NDECL_LIST);
		p->child = decl_list();
	}
	return(p);
}

Pnode decl_list()
{
	Pnode head, p;
	head = p = nonterminalnode(NDECL);
	p->child = decl();
	match(SEMICOLON);
	while(lookahead==ID){
		p->brother = nonterminalnode(NDECL);
		p = p->brother;
		p->child = decl();
	}
	return(head);
}

Pnode decl()
{
	Pnode p;
	p = nonterminalnode(NID_LIST);
	p->child = id_list();
	match(COLON);
	p->brother = nonterminalnode(NTYPE);
	p->brother = type();
	return(p);
}

Pnode id_list()
{
	Pnode head, p;
	head =  p = idnode();
	while(lookahead==COMMA){
		match(COMMA);
		p->brother = nonterminalnode(NID_LIST);
		p = p->brother;
		p->child = id_list();
	}
	return(head);
}

Pnode opt_const_sect()
{
	Pnode p;
	if(lookahead==CONST){
		match(CONST);
		p = nonterminalnode(NCONST_LIST);
		p->child = const_list();
	}
	return(p);
}

Pnode const_list()
{
	Pnode head, p;
	head = p = nonterminalnode(NCONST_DECL);
	p->child = const_decl();
	while(lookahead==ID){
		p->brother = nonterminalnode(NCONST_DECL);
		p = p->brother;
		p->child = const_decl();
	return(head);
}


Pnode const_decl()
{
	Pnode p;
	p = nonterminalnode(NDECL);
	p->child = decl();
	match(ASSIGN);
	p->brother = nonterminalnode(NEXPR);
	p->brother = expr();
	match(SEMICOLON);
	return(p);
}

Pnode opt_module_list()
{
	Pnode p;
	if(lookahead==MODULE){
		p = nonterminalnode(NMODULE_DECL);
		p->child = module_decl();
		p->brother = nonterminalnode(NOPT_MODULE_LIST);
		p->brother = opt_module_list();
	}
	return(p);
}

Pnode module_body()
{
	Pnode p;
	match(BEGIN);
	p = idnode();
	p->brother=nonterminalnode(NSTAT_LIST);
	p->brother=stat_list();
	match(END);
	p = idnode();
	return(p);
}

Pnode stat_list()
{
	Pnode head, p;
	head = p = nonterminalnode(NSTAT);
	p->child = stat();
	match(SEMICOLON);
	while(lookahead==ID || lookahead==IF || lookahead==WHILE || lookahead==RETURN || lookahead==READ || lookahead==WRITE){
		p->brother = nonterminalnode(NSTAT);
		p = p->brother;
		p->child = stat();
	}
	return(head);
}

Pnode stat()
{
	Pnode p;
	if (lookahead==ID){
		p = nonterminalnode(NASSIGN_STAT);
		p->child = assign_stat();
		return(p);
	} 
	else if (lookahead==IF){
		p = nonterminalnode(NIF_STAT);
		p->child = if_stat();
		return(p);
	} 
	else if (lookahead==WHILE){
		p = nonterminalnode(NWHILE_STAT);
		p->child = while_stat();
		return(p);
	}
	else if (lookahead==RETURN){
		p = nonterminalnode(NRETURN_STAT);
		p->child = return_stat();
		return(p);
	} 
	else if (lookahead==READ){
		p = nonterminalnode(NREAD_STAT);
		p->child = read_stat();
		return(p);
	} 
	else if (lookahead==WRITE){
		p = nonterminalnode(NWRITE_STAT);
		p->child = write_stat();
		return(p);
	}
	else {
		parserror();
	}
	
}

Pnode assign_stat()
{
	Pnode p;
	p = idnode();
	match(ASSIGN);
	p->brother = nonterminalnode(NEXPR);
	p->brother = expr();
}

Pnode if_stat()
{

}

Pnode elseif_stat_list()
{

}

Pnode else_stat()
{

}

Pnode while_stat()
{

}

Pnode return_stat()
{

}

Pnode opt_expr()
{

}

Pnode read_stat()
{

}

Pnode write_stat()
{

}

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