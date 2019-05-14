#include <stdio.h>
#include <stdlib.h>

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

typedef union
{
	int ival;
	float rval;
	char *sval;
} Lexval;

typedef struct snode
{
	Typenode type;
	Lexval value;
	struct snode *child, *brother;
} Node;


typedef Node *Pnode;

char *newstring(char*), *strcpy(char*, const char*);

int yylex(), store_id(), lookup(char*);

void strcopy(char*, const char*), match(int), next(), perserror(), treeprint(Pnode, int);

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
      opt_parma_list(),
      param_list()
      param_decl(),
      type(),
      opt_var_sect(),
      decl_list(),
      decl(),
      id_list(),
      const_sect(),
	  const_list(),
	  const_decl(),
	  opt_module_list(),
	  module_body(),
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

