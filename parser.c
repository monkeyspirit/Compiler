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
	p->brother->brother = nonterminalnode(NOPT_VAR_SECT);
	p->brother->brother->child = opt_var_sect();
	p->brother->brother->brother = nonterminalnode(NOPT_CONST_SECT);
	p->brother->brother->brother->child = opt_const_sect();
	p->brother->brother->brother->brother = nonterminalnode(NOPT_MODULE_LIST);
	p->brother->brother->brother->brother->child = opt_module_list();
	p->brother->brother->brother->brother->brother = nonterminalnode(NMODULE_BODY);
	p->brother->brother->brother->brother->brother->child = module_body();
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
	p->child->child = type();
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
	p->brother->child = expr();
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
		p->brother->child = opt_module_list();
	}
	return(p);
}

Pnode module_body()
{
	Pnode p;
	match(BEGIN);
	p = idnode();
	p->brother = nonterminalnode(NSTAT_LIST);
	p->brother->child = stat_list();
	match(END);
	p = idnode();
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
		p = p->brother;
		p->child = stat();
		match(SEMICOLON);
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
	next();
	match(ASSIGN);
	p->brother = nonterminalnode(NEXPR);
	p->brother->child = expr();
	return(p);
}

Pnode if_stat()
{
	Pnode p;
	match(IF);
	p = nonterminalnode(NEXPR);
	p->child = expr();
	match(THEN);
	p->brother = nonterminalnode(NSTAT_LIST);
	p->brother->child = stat_list();
	p->brother->brother = nonterminalnode(NOPT_ELSEIF_STAT_LIST);
	p->brother->brother->child = opt_elseif_stat_list();
	p->brother->brother->brother = nonterminalnode(NOPT_ELSE_STAT);
	p->brother->brother->brother->child = opt_else_stat();
	match(END);
	return(p);
}


Pnode opt_elseif_stat_list()
{
	Pnode p;
	if(lookahead==ELSEIF){
	match(ELSEIF);
	p = nonterminalnode(NEXPR);
	p->child = expr();
	match(THEN);
	p->brother = nonterminalnode(NSTAT_LIST);
	p->brother->child = stat_list();
	p->brother->brother = nonterminalnode(NOPT_ELSEIF_STAT_LIST);
	p->brother->brother->child = opt_elseif_stat_list();
	}
	return(p);
}

Pnode opt_else_stat()
{
	Pnode p;
	if(lookahead==ELSE){
	match(ELSE);
	p = nonterminalnode(NSTAT_LIST);
	p->child = stat_list();
	}
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
	p = nonterminalnode(NOPT_EXPR);
	p->child = opt_expr();
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
	match(write_stat);
	match(LBRACE);
	p = nonterminalnode(NEXPR_LIST);
	p->child = expr_list();
	match(RBRACE);
	return(p);
}


Pnode expr_list()
{
	Pnode p;
	p = nonterminalnode(NEXPR);
	p->child = expr();
	while(lookahead==MINUS || lookahead==NOT || lookahead==LBRACE || lookahead==ID || lookahead==CHARCONST || lookahead == INTCONST || lookahead== REALCONST || lookahead==STRCONST || lookahead==BOOLCONST || lookahead==IF || lookahead==CHAR || lookahead==INT || lookahead==REAL || lookahead==STRING || lookahead==BOOL || lookahead==VOID){
		match(COMMA);
		p->brother = nonterminalnode(NEXPR);
		p->brother->child = expr();
	}
	return(p);
}

Pnode expr()
{
	Pnode p;
	p = nonterminalnode(NBOOL_TERM);
	p->child = bool_term();
	p->brother = nonterminalnode(NEXPR1);
	p->brother->child = expr1();
	return(p);
}
	 
Pnode expr1()
{
	Pnode p;
	if(lookahead==AND || lookahead==OR){
		p = nonterminalnode(NBOOLOP);
		p->child = boolop();
		p->brother = nonterminalnode(NBOOL_TERM);
		p->brother->child = bool_term();
		p->brother->brother = nonterminalnode(NEXPR1);
		p->brother->brother->child = expr1();
	}
	return(p);
}

Pnode boolop()
{
	Pnode p;
	if(lookahead==AND){
		match(AND);
		return(p);
	} 
	else if (lookahead==OR){
		match(OR);
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
	p->child = relterm();
	if(lookahead==LE || lookahead==GE || lookahead==EQ || lookahead==NEQ || lookahead==LT || lookahead==GT){
		p->brother = nonterminalnode(NRELOP);
		p->brother->child = relop();
		p->brother->brother = nonterminalnode(NREL_TERM);
		p->brother->brother->child = relterm();
	}
	return(p);
}

Pnode relop()
{
	Pnode p;
	if(lookahead==LE){
		match(LE);
		return(p);
	} 
	else if (lookahead==GE){
		match(GE);
		return(p);
	} 
	else if (lookahead==EQ){
		match(EQ);
		return(p);
	}
	else if (lookahead==NEQ){
		match(NEQ);
		return(p);
	}
	else if (lookahead==LT){
		match(LT);
		return(p);
	}
	else if (lookahead==GT){
		match(GT);
		return(p);
	}
	else{
		parserror();
	}
}

Pnode relterm()
{
	Pnode p;
	p = nonterminalnode(NLOW_TERM);
	p->child = low_term();
	p->brother = nonterminalnode(NREL_TERM1);
	p->brother->child = relterm1();
	return(p);
}
	 
Pnode relterm1()
{
	Pnode p;
	if(lookahead==PLUS || lookahead==MINUS){
		p = nonterminalnode(NLOW_BINOP);
		p->child = low_binop();
		p->brother = nonterminalnode(NLOW_TERM);
		p->brother->child = low_term();
		p->brother->brother = nonterminalnode(NREL_TERM1);
		p->brother->brother->child = relterm1();
	}
	return(p);
}

Pnode low_binop()
{
	Pnode p;
	if(lookahead==PLUS){
		match(PLUS);
		return(p);
	} 
	else if (lookahead==MINUS){
		match(MINUS);
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
	p->brother = nonterminalnode(NLOW_TERM1);
	p->brother->child = lowterm1();
	return(p);
}

Pnode lowterm1()
{
	Pnode p;
	if(lookahead==AST || lookahead==FRAC){
		p = nonterminalnode(NHIGH_BINOP);
		p->child = high_binop();
		p->brother = nonterminalnode(NFACTOR);
		p->brother->child = factor();
		p->brother->brother = nonterminalnode(NLOW_TERM1);
		p->brother->brother->child = low_term1();
	}
	return(p);
}

Pnode high_binop()
{
	Pnode p;
	if(lookahead==AST){
		match(AST);
		return(p);
	} 
	else if (lookahead==FRAC){
		match(FRAC);
		return(p);
	} 
	else{
		parserror();
	}
}

Pnode factor()
{
	Pnode p;
	if(lookahead==MINUS || lookahead==NOT){
		p = nonterminalnode(NUNARYOP);
		p->child = unaryop();
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
		p = idnode();
		next();
		if(lookahead==LBRACE){
			p->brother = nonterminalnode(NCOND_EXPR);
			p->brother->child = cond_expr();
		}
		return(p);
	}
	else if (lookahead==CHARCONST || lookahead == INTCONST || lookahead== REALCONST || lookahead==STRCONST || lookahead==BOOLCONST){
		p = nonterminalnode(NCONSTANT);
		p->child = constant();
		return(p);
	}
	else if (lookahead==IF){
		p = nonterminalnode(NMODULE_CALL);
		p->child = module_call();
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

Pnode unaryop()
{
	Pnode p;
	if(lookahead==MINUS){
		match(MINUS);
		return(p);
	} 
	else if (lookahead==NOT){
		match(NOT);
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
		p = keynode(T_CHARCONST);
		next();
		return(p);
	} 
	else if (lookahead==INTCONST){
		p = keynode(T_INTCONST);
		next();
		return(p);
	} 
	else if (lookahead==REALCONST){
		p = keynode(T_REALCONST);
		next();
		return(p);
	}
	else if (lookahead==STRCONST){
		p = keynode(T_STRCONST);
		next();
		return(p);
	} 
	else if (lookahead==BOOLCONST){
		p = keynode(T_BOOLCONST);
		next();
		return(p);
	}
	else{
		parserror();
	}
}

Pnode module_call()
{
	Pnode p;
	p = idnode();
	next();
	match(LBRACE);
	p->brother = nonterminalnode(NOPT_EXPR_LIST);
	p->brother->child = opt_expr_list();
	match(RBRACE);
	return(p);
}

Pnode opt_expr_list()
{
	Pnode p;
	if (lookahead==MINUS || lookahead==NOT || lookahead==LBRACE || lookahead==ID || lookahead==CHARCONST || lookahead == INTCONST || lookahead== REALCONST || lookahead==STRCONST || lookahead==BOOLCONST || lookahead==IF || lookahead==CHAR || lookahead==INT || lookahead==REAL || lookahead==STRING || lookahead==BOOL || lookahead==VOID) {
		p = nonterminalnode(NEXPR_LIST);
		p->child = expr_list();
	}
	return(p);
}

Pnode cond_expr()
{
	Pnode p;
	match(IF);
	p = nonterminalnode(NEXPR);
	p->child = expr();
	match(THEN);
	p->brother = nonterminalnode(NEXPR);
	p->brother->child = expr();
	p->brother->brother = nonterminalnode(NOPT_ELSEIF_EXPR_LIST);
	p->brother->brother->child = opt_elseif_expr_list();
	match(ELSE);
	p->brother->brother->brother = nonterminalnode(NEXPR);
	p->brother->brother->brother->child = expr();
	match(END);
	return(p);
}

Pnode opt_elseif_expr_list()
{
	Pnode p;
	if(lookahead==ELSEIF){
		match(ELSEIF);
		p = nonterminalnode(NEXPR);
		p->child = expr();
		match(THEN);
		p->brother = nonterminalnode(NEXPR);
		p->brother->child = expr();
		p->brother->brother = nonterminalnode(NOPT_ELSEIF_EXPR_LIST);
		p->brother->brother->child = opt_elseif_expr_list();
	}
	return(p);
}


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