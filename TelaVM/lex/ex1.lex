
%{
#include <stdio.h>
#include "tCodeDef.h"
int pos = 1;
%}
%option noyywrap
uppercase [A-Z];
codice uppercase uppercase uppercase
%%
^TCODE {}
^NEW {printf("%d", NEW);}
^LDC {printf("%d", LDC);}
^LDI {printf("%d", LDI);}
^LDR {printf("%d", LDR);}
^LDS {printf("%d", LDS);}
^LOD {printf("%d", LOD);}
^STO {printf("%d", STO);}
^JMF {printf("%d", JMF);}
^JMP {printf("%d", JMP);}
^EQU {printf("%d", EQU);}
^NEQ {printf("%d", NEQ);}
^CGT {printf("%d", CGT);}
^CGE {printf("%d", CGE);}
^CLT {printf("%d", CLT);}
^CLE {printf("%d", CLE);}
^IGT {printf("%d", IGT);}
^IGE {printf("%d", IGE);}
^ILT {printf("%d", ILT);}
^ILE {printf("%d", ILE);}
^RGT {printf("%d", RGT);}
^RGE {printf("%d", RGE);}
^RLT {printf("%d", RLT);}
^RLE {printf("%d", RLE);}
^SGT {printf("%d", SGT);}
^SGE {printf("%d", SGE);}
^SLT {printf("%d", SLT);}
^SLE {printf("%d", SLE);}
^IADD {printf("%d", IADD);}
^ISUB {printf("%d", ISUB);}
^IMUL {printf("%d", IMUL);}
^IDIV {printf("%d", IDIV);}
^RADD {printf("%d", RADD);}
^RSUB {printf("%d", RSUB);}
^RMUL {printf("%d", RMUL);}
^RDIV {printf("%d", RDIV);}
^IUMI {printf("%d", IUMI);}
^RUMI {printf("%d", RUMI);}
^LNEG {printf("%d", LNEG);}
^PUSH {printf("%d", PUSH);}
GOTO {printf("%d", GOTO);}
^POP {printf("%d", POP);}
^TOINT {printf("%d", TOINT);}
^TOREA {printf("%d", TOREA);}
^READ {printf("%d", READ);}
^WRITE {printf("%d", WRITE);}
^MOD {printf("%d", MOD);}
^RETURN {printf("%d", RETURN);}
^HALT {printf("%d", HALT);}
%%
void main()
{ 
  yylex(); 
}
