#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MODULE 258
#define ID 259
#define LBRACE 260
#define RBRACE 261
#define COLON 262
#define SEMICOLON 263
#define COMMA 264 
#define CHAR 265
#define INT 266
#define REAL 267
#define STRING 268
#define BOOL 269
#define VOID 270
#define VAR 271
#define CONST 272
#define BEGIN 273
#define END 274 
#define IF 275
#define THEN 276
#define ELSE 277
#define ELSEIF 278
#define WHILE 279
#define DO 280
#define RETURN 281
#define READ 282
#define WRITE 283
#define AND 284
#define OR 285
#define NOT 286
#define ASSIGN 287
#define LE 288
#define GE 289
#define EQ 290
#define NEQ 291
#define PLUS 292
#define MINUS 293
#define AST 294
#define FRAC 295
#define INTCONST 296
#define REALCONST 297
#define CHARCONST 298
#define STRCONST 299
#define BOOLCONST 300
#define ID 301
#define ERROR 302

/*Elenco dei non terminali*/
typedef enum
{
	NPROGRAM,
	NMODULE_DECL,
	NOPT_PARAM_LIST,
	NPARAM_LIST,
	NPARAM_DECL,
	NTYPE,
	NOPT_VAR_SECT,
	NDECL_LIST,
	NDECL,
	NID_LIST,
	NOPT_CONST_SECT,
	NCONST_LIST,
	NCONST_DECL,
	NOPT_MODULE_LIST,
	NMODULE_BODY,
	NSTAT_LIST,
	NSTAT,
	NASSIGN_STAT,
	NIF_STAT,
	NOPT_ELSEIF_STAT_LIST,
	NOPT_ELSE_STAT,
	NWHILE_STAT,
	NRETURN_STAT,
	NOPT_EXPR,
	NREAD_STAT,
	NWRITE_STAT,
	NEXPR_LIST,
	NEXPR,
	NBOOLOP,
	NBOOL_TERM,
	NRELOP,
	NREL_TERM,
	NLOW_BINOP,
	NLOW_TERM,
	NHIGH_BINOP,
	NFACTOR,
	NUNARYOP,
	NCONSTANT,
	NMODULE_CALL,
	NOPT_EXPR_LIST,
	NCOND_EXPR,
	NOPT_ELSEIF_EXPR_LIST
} Nonterminal;

/*Elenco dei tipi di nodo (che devono conservare dati)*/
typedef enum
{
	T_CHAR,
	T_INT,
	T_REAL,
	T_STRING,
	T_BOOL,
	T_VOID,
	T_CHARCONST,
	T_INTCONST,
	T_REALCONST,
	T_STRCONST,
	T_BOOLCONST,
	T_ID,
	T_NONTERMINAL
} Typenode;

/*Definizione della union del Lexval*/
typedef union
{
	int ival;
	float rval;
	char *sval;
} Lexval;

/*Definizione della struttura di un nodo*/
typedef struct structNode
{
	Typenode type;
	Lexval value;
	struct structNode *child, *brother;
} Node;


typedef Node *Pnode; /*Definisco il tipo Pnode per semplicità*/

/*Elenco dei vari metodi utilizzati, solo tipo, nome e parametri*/

char *strcpy(char*, const char*);

void  strcopy(char*, const char*), match(int), next(), perserror(), treeprint(Pnode, int);

Pnode nontermnode(Nonterminal), 
      idnode(), 
      keynode(Typenode), 
      charconstnode(),
      intconstnode(),
      realconstnode(),
      strconstnode(),
      boolconstnode(),
      newnode(Typenode);
      program(),
      module_decl(),
      opt_param_list(),
      param_list()
      param_decl(),
      type(),
      opt_var_sect(),
      decl_list(),
      decl(),
      id_list(),
      opt_const_sect(),
	  const_list(),
	  const_decl(),
	  opt_module_list(),
	  module_body(),
	  stat_list()
	  stat(),
	  assign_stat(),
	  if_stat(),
	  opt_elseif_stat_list(),
	  opt_else_stat(),
	  while_stat(),
	  return_stat(),
	  opt_expr(),
	  read_stat(),
	  write_stat(),
	  expr_list(),
	  expr(),
	  expr1(),
	  boolop(),
	  bool_term(),
	  relop(),
	  relterm(),
	  relterm1(),
	  low_binop(),
	  low_term(),
	  lowterm1(),
	  high_binop(), <------------
	  factor(),
	  unaryop(),
	  constant(),
	  module_call(),
	  opt_expr_list(),
	  cond_expr(),
	  opt_elseif_expr_list();

