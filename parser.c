#include "def.h"
#include "parser.h"
#include "semantic.h"

extern char *yytext;
extern Lexval value;
extern int line;
extern FILE *yyin;
extern FILE *yyout;

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
	return(p);
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
	p->value.sval = value.sval; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo char*/
Pnode charconstnode()
{
	Pnode p;
	p = newnode(T_CHARCONST);
	p->value.sval = value.sval; /*Regole di stato del parsing*/

	return(p);
}

Pnode opnode(Typenode node)
{
	Pnode p;
	p = newnode(node);
	p->value.ival = value.ival; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo int*/
Pnode intconstnode()
{
	Pnode p;
	p = newnode(T_INTCONST);
	p->value.ival = value.ival; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo real*/
Pnode realconstnode()
{
	Pnode p;
	p = newnode(T_REALCONST);
	p->value.rval = value.rval; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo string*/
Pnode strconstnode()
{
	Pnode p;
	p = newnode(T_STRCONST);
	p->value.sval = value.sval; /*Regole di stato del parsing*/
	return(p);
}

/*Meotod che crea un nodo associato ad una costanre di tipo bool*/
Pnode boolconstnode()
{
	Pnode p;
	p = newnode(T_BOOLCONST);
	p->value.ival = value.ival; /*Regole di stato del parsing*/
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
	Pnode p, d, c, h , j, m;
	int i = 0;
	match(MODULE);
	p = d = m = c = h = j = idnode();
	next();
	match(LBRACE);

	if(lookahead==ID){
		p->brother = nonterminalnode(NOPT_PARAM_LIST);
		p->brother->child = opt_param_list();
		d = c = h = j = p->brother;
	}

	match(RBRACE);
	match(COLON);

	d->brother = nonterminalnode(NTYPE);
	d->brother->child = type();
	m = c = h = j = d->brother;

	if(lookahead==VAR){
		m->brother = nonterminalnode(NOPT_VAR_SECT);
		m->brother->child = opt_var_sect();
		c = h = j = m->brother;
	}

	if(lookahead==CONST){
		c->brother = nonterminalnode(NOPT_CONST_SECT);
		c->brother->child = opt_const_sect();
		h = j = c->brother;
	}

	if(lookahead==MODULE){
		h->brother = nonterminalnode(NOPT_MODULE_LIST);
		h->brother->child = opt_module_list();
		j = h->brother;
	}

	j->brother = nonterminalnode(NMODULE_BODY);
	j->brother->child = module_body();

	return(p);
}

Pnode opt_param_list()
{
	Pnode p;
	p = nonterminalnode(NPARAM_LIST);
	p->child = param_list();

	return(p);
}

Pnode param_list()
{
	Pnode head, p;
	head = p = nonterminalnode(NPARAM_DECL);
	p->child = param_decl();
	while(lookahead==COMMA)
	{
		match(COMMA);
		p->brother = nonterminalnode(NPARAM_DECL);
		p = p-> brother;
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
	p->brother = nonterminalnode(NTYPE);
	p->brother->child = type();
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
	match(VAR);
	p = nonterminalnode(NDECL_LIST);
	p->child = decl_list();
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
		match(SEMICOLON);
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
	p->brother->child = type();
	return(p);
}

Pnode id_list()
{
	Pnode head, p;
	head =  p = idnode();
	next();
	while(lookahead==COMMA){
		match(COMMA);
		p->brother = idnode();
		p=p->brother;
		next();
	}
	return(head);
}

Pnode opt_const_sect()
{
	Pnode p;
	match(CONST);
	p = nonterminalnode(NCONST_LIST);
	p->child = const_list();
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
	}
	return(head);
}


Pnode const_decl()
{
	Pnode p;
	p = nonterminalnode(NDECL);
	p->child = decl();
	match(ASSIGN);
	p->brother = nonterminalnode(NEXPR);
	p->brother->child = expr();
	match(SEMICOLON);
	return(p);
}

Pnode opt_module_list()
{
	Pnode p, h;
	p = h = nonterminalnode(NMODULE_DECL);
	p->child = module_decl();

	while(lookahead==MODULE){
		h->brother = nonterminalnode(NMODULE_DECL);
		h=h->brother;
		h->child = module_decl();
	}

	return(p);
}

Pnode module_body()
{
	Pnode p;
	match(BEGIn);
	p = idnode();
	next();
	p->brother = nonterminalnode(NSTAT_LIST);
	p->brother->child = stat_list();
	match(END);
	p->brother->brother = idnode();
	next();
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
		p=p->brother;
		p->child = stat();
		match(SEMICOLON);
	}
	return(head);
}

Pnode stat()
{
	Pnode p, d;
	if (lookahead==ID){
		d=idnode();
		next();
		if(lookahead==ASSIGN){
			p = nonterminalnode(NASSIGN_STAT);
			p->child = assign_stat(d);
			return(p);
		}
		else if (lookahead==LBRACE){
			p = nonterminalnode(NMODULE_CALL);
			p->child=module_call(d);
			return(p);
		}
		else{
			parserror();
		}
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

Pnode assign_stat(Pnode p)
{

	match(ASSIGN);
	p->brother = nonterminalnode(NEXPR);
	p->brother->child = expr();
	return(p);
}

Pnode if_stat()
{
	Pnode p, h, l;
	match(IF);
	p = h = l = nonterminalnode(NEXPR);
	p->child = expr();
	match(THEN);
	p->brother = nonterminalnode(NSTAT_LIST);
	p->brother->child = stat_list();
	h = l = p->brother;

	if(lookahead==ELSEIF){
		h->brother = nonterminalnode(NOPT_ELSEIF_STAT_LIST);
		h->brother->child = opt_elseif_stat_list();
		l = h->brother;
	}

	if(lookahead==ELSE){
		l->brother = nonterminalnode(NOPT_ELSE_STAT);
		l->brother->child = opt_else_stat();
	}


	match(END);
	return(p);
}


Pnode opt_elseif_stat_list()
{
	Pnode p, m;
	match(ELSEIF);
	p = m = nonterminalnode(NEXPR);
	p->child = expr();
	match(THEN);
	p->brother = nonterminalnode(NSTAT_LIST);
	p->brother->child = stat_list();
	m = p->brother;

	if(lookahead==ELSEIF){
		m->brother = nonterminalnode(NOPT_ELSEIF_STAT_LIST);
		m->brother->child = opt_elseif_stat_list();
	}

	return(p);
}

Pnode opt_else_stat()
{
	Pnode p;
	match(ELSE);
	p = nonterminalnode(NSTAT_LIST);
	p->child = stat_list();

	return(p);
}

Pnode while_stat()
{
	Pnode p;
	match(WHILE);
	p = nonterminalnode(NEXPR);
	p->child = expr();
	match(DO);
	p->brother = nonterminalnode(NSTAT_LIST);
	p->brother->child = stat_list();
	match(END);
	return(p);
}

Pnode return_stat()
{
	Pnode p;
	match(RETURN);
	if(lookahead!=SEMICOLON){
		p = nonterminalnode(NOPT_EXPR);
		p->child = opt_expr();
	}
	else{
		p=keynode(T_RETURNULL);
	}
	return(p);
}

Pnode opt_expr()
{
	Pnode p;
	if(lookahead==MINUS || lookahead==NOT || lookahead==LBRACE || lookahead==ID || lookahead==CHARCONST || lookahead == INTCONST || lookahead== REALCONST || lookahead==STRCONST || lookahead==BOOLCONST || lookahead==IF || lookahead==CHAR || lookahead==INT || lookahead==REAL || lookahead==STRING || lookahead==BOOL || lookahead==VOID){
		p = nonterminalnode(NEXPR);
		p->child = expr();
	}
	return(p);
}

Pnode read_stat()
{
	Pnode p;
	match(READ);
	match(LBRACE);
	p = nonterminalnode(NID_LIST);
	p->child = id_list();
	match(RBRACE);
	return(p);
}

Pnode write_stat()
{
	Pnode p;
	match(WRITE);
	match(LBRACE);
	p = nonterminalnode(NEXPR_LIST);
	p->child = expr_list();
	match(RBRACE);
	return(p);
}


Pnode expr_list()
{
	Pnode head, p;
	head = p = nonterminalnode(NEXPR);
	p->child = expr();
	while(lookahead==COMMA){
		match(COMMA);
		p->brother = nonterminalnode(NEXPR);
		p->brother->child = expr();
		p=p->brother;
	}
	return(head);
}

Pnode expr()
{
	Pnode p;
	p = nonterminalnode(NBOOL_TERM);
	p->child = bool_term();
	if(lookahead==AND || lookahead==OR){
		p->brother = nonterminalnode(NEXPR1);
		p->brother->child = expr1();
	}
	return(p);
}

Pnode expr1()
{
	Pnode p;
	if(lookahead==AND || lookahead==OR){
		p = nonterminalnode(NBOOLOP);
		p->child = bool_op();
		p->brother = nonterminalnode(NBOOL_TERM);
		p->brother->child = bool_term();
		if(lookahead==AND || lookahead==OR){
			p->brother->brother = nonterminalnode(NEXPR1);
			p->brother->brother->child = expr1();
		}
	}
	return(p);
}

Pnode bool_op()
{
	Pnode p;
	if(lookahead==AND){
		p = opnode(T_AND);
		next();
		return(p);
	}
	else if (lookahead==OR){
		p = opnode(T_OR);
		next();
		return(p);
	}
	else{
		parserror();
	}
}

Pnode bool_term()
{
	Pnode p;
	p = nonterminalnode(NREL_TERM);
	p->child = rel_term();
	if(lookahead==LE || lookahead==GE || lookahead==EQ || lookahead==NEQ || lookahead==LT || lookahead==GT){
		p->brother = nonterminalnode(NRELOP);
		p->brother->child = rel_op();
		p->brother->brother = nonterminalnode(NREL_TERM);
		p->brother->brother->child = rel_term();
	}
	return(p);
}

Pnode rel_op()
{
	Pnode p;
	if(lookahead==LE){
		p = opnode(T_LE);
		next();
		return(p);
	}
	else if (lookahead==GE){
		p = opnode(T_GE);
		next();
		return(p);
	}
	else if (lookahead==EQ){
		p = opnode(T_EQ);
		next();
		return(p);
	}
	else if (lookahead==NEQ){
		p = opnode(T_NEQ);
		next();
		return(p);
	}
	else if (lookahead==LT){
		p = opnode(T_LT);
		next();
		return(p);
	}
	else if (lookahead==GT){
		p = opnode(T_GT);
		next();
		return(p);
	}
	else{
		parserror();
	}
}

Pnode rel_term()
{
	Pnode p;
	p = nonterminalnode(NLOW_TERM);
	p->child = low_term();
	if(lookahead==PLUS || lookahead==MINUS){
		p->brother = nonterminalnode(NREL_TERM1);
		p->brother->child = rel_term1();
	}
	return(p);
}

Pnode rel_term1()
{
	Pnode p;
	if(lookahead==PLUS || lookahead==MINUS){
		p = nonterminalnode(NLOW_BINOP);
		p->child = low_binop();
		p->brother = nonterminalnode(NLOW_TERM);
		p->brother->child = low_term();
		if(lookahead==PLUS || lookahead==MINUS){
			p->brother->brother = nonterminalnode(NREL_TERM1);
			p->brother->brother->child = rel_term1();
		}
	}
	return(p);
}

Pnode low_binop()
{
	Pnode p;
	if(lookahead==PLUS){
		p = opnode(T_PLUS);
		next();
		return(p);
	}
	else if (lookahead==MINUS){
		p = opnode(T_MINUS);
		next();
		return(p);
	}
	else{
		parserror();
	}
}

Pnode low_term()
{
	Pnode p;
	p = nonterminalnode(NFACTOR);
	p->child = factor();
	if(lookahead==AST || lookahead==FRAC){
		p->brother = nonterminalnode(NLOW_TERM1);
		p->brother->child = low_term1();
	}
	return(p);
}

Pnode low_term1()
{
	Pnode p;
	if(lookahead==AST || lookahead==FRAC){
		p = nonterminalnode(NHIGH_BINOP);
		p->child = high_binop();
		p->brother = nonterminalnode(NFACTOR);
		p->brother->child = factor();
		if(lookahead==AST || lookahead==FRAC){
			p->brother->brother = nonterminalnode(NLOW_TERM1);
			p->brother->brother->child = low_term1();
		}
	}
	return(p);
}

Pnode high_binop()
{
	Pnode p;
	if(lookahead==AST){
		p = opnode(T_AST);
		next();
		return(p);
	}
	else if (lookahead==FRAC){
		p = opnode(T_FRAC);
		next();
		return(p);
	}
	else{
		parserror;
	}
}

Pnode factor()
{
	Pnode p, d;
	if(lookahead==MINUS || lookahead==NOT){
		p = nonterminalnode(NUNARYOP);
		p->child = unary_op();
		p->brother = nonterminalnode(NFACTOR);
		p->brother->child = factor();
		return(p);
	}
	else if (lookahead==LBRACE){
		match(LBRACE);
		p = nonterminalnode(NEXPR);
		p->child = expr();
		match(RBRACE);
		return(p);
	}
	else if (lookahead==ID){
		d = idnode();
		next();
		if(lookahead==LBRACE){
			p = nonterminalnode(NMODULE_CALL);
			p->child = module_call(d);
			return(p);
		}
		else{
			return(d);
		}

	}
	else if (lookahead==CHARCONST || lookahead == INTCONST || lookahead== REALCONST || lookahead==STRCONST || lookahead==BOOLCONST){
		p = nonterminalnode(NCONSTANT);
		p->child = constant();
		return(p);
	}
	else if (lookahead==IF){
		p = nonterminalnode(NCOND_EXPR);
		p->child = cond_expr();
		return(p);
	}
	else if (lookahead==CHAR || lookahead==INT || lookahead==REAL || lookahead==STRING || lookahead==BOOL || lookahead==VOID){
		p = nonterminalnode(NTYPE);
		p->child = type();
		match(LBRACE);
		p->brother = nonterminalnode(NEXPR);
		p->brother->child = expr();
		match(RBRACE);
		return(p);
	}
	else{
		parserror();
	}
}

Pnode unary_op()
{
	Pnode p;
	if(lookahead==MINUS){
		p = opnode(T_MINUS);
		next();
		return(p);
	}
	else if (lookahead==NOT){
		p = opnode(T_NOT);
		next();
		return(p);
	}
	else{
		parserror();
	}
}

Pnode constant()
{
	Pnode p;
	if(lookahead==CHARCONST){
		p = charconstnode();
		next();
		return(p);
	}
	else if (lookahead==INTCONST){
		p = intconstnode();
		next();
		return(p);
	}
	else if (lookahead==REALCONST){
		p = realconstnode();
		next();
		return(p);
	}
	else if (lookahead==STRCONST){
		p = strconstnode();
		next();
		return(p);
	}
	else if (lookahead==BOOLCONST){
		p = boolconstnode();
		next();
		return(p);
	}
	else{
		parserror();
	}
}

Pnode module_call(Pnode p)
{
	match(LBRACE);
	if(lookahead==MINUS || lookahead==NOT || lookahead==LBRACE || lookahead==ID || lookahead==CHARCONST || lookahead == INTCONST || lookahead== REALCONST || lookahead==STRCONST || lookahead==BOOLCONST || lookahead==IF || lookahead==CHAR || lookahead==INT || lookahead==REAL || lookahead==STRING || lookahead==BOOL || lookahead==VOID) {
		p->brother = nonterminalnode(NOPT_EXPR_LIST);
		p->brother->child = opt_expr_list();
	}
	match(RBRACE);
	return(p);
}

Pnode opt_expr_list()
{
	Pnode p;
	p = nonterminalnode(NEXPR_LIST);
	p->child = expr_list();
	return(p);
}

Pnode cond_expr()
{
	Pnode p, m,d;
	match(IF);
	p = m = d = nonterminalnode(NEXPR);
	p->child = expr();
	match(THEN);
	p->brother = nonterminalnode(NEXPR);
	p->brother->child = expr();
	m = d = p->brother;

	if(lookahead==ELSEIF){
		m->brother = nonterminalnode(NOPT_ELSEIF_EXPR_LIST);
		m->brother->child = opt_elseif_expr_list();
		d = m->brother;
	}

	match(ELSE);
	d->brother = nonterminalnode(NEXPR);
	d->brother->child = expr();

	match(END);
	return(p);
}

Pnode opt_elseif_expr_list()
{
	Pnode p;
	match(ELSEIF);
	p = nonterminalnode(NEXPR);
	p->child = expr();
	match(THEN);
	p->brother = nonterminalnode(NEXPR);
	p->brother->child = expr();

	if(lookahead==ELSEIF){
		p->brother->brother = nonterminalnode(NOPT_ELSEIF_EXPR_LIST);
		p->brother->brother->child = opt_elseif_expr_list();
	}

	return(p);
}



Pnode parse(){
    yyin = fopen("../prog.tela", "r");
    yyout = fopen("../yyout", "w");
	next();
	root = nonterminalnode(NPROGRAM);
	root->child = program();
    return root;
}





