
%{
#include <stdio.h>
#include "tCodeDef.h"
int pos = 1;
FILE *out;
%}
%option noyywrap
uppercase [A-Z];
codice uppercase uppercase uppercase
%%
\" {}
\' {}
^TCODE {}
^NEW {fprintf(out, "%d", NEW);}
^LDC {fprintf(out, "%d", LDC);}
^LDI {fprintf(out, "%d", LDI);}
^LDR {fprintf(out, "%d", LDR);}
^LDS {fprintf(out, "%d", LDS);}
^LOD {fprintf(out, "%d", LOD);}
^STO {fprintf(out, "%d", STO);}
^JMF {fprintf(out, "%d", JMF);}
^JMP {fprintf(out, "%d", JMP);}
^EQU {fprintf(out, "%d", EQU);}
^NEQ {fprintf(out, "%d", NEQ);}
^CGT {fprintf(out, "%d", CGT);}
^CGE {fprintf(out, "%d", CGE);}
^CLT {fprintf(out, "%d", CLT);}
^CLE {fprintf(out, "%d", CLE);}
^IGT {fprintf(out, "%d", IGT);}
^IGE {fprintf(out, "%d", IGE);}
^ILT {fprintf(out, "%d", ILT);}
^ILE {fprintf(out, "%d", ILE);}
^RGT {fprintf(out, "%d", RGT);}
^RGE {fprintf(out, "%d", RGE);}
^RLT {fprintf(out, "%d", RLT);}
^RLE {fprintf(out, "%d", RLE);}
^SGT {fprintf(out, "%d", SGT);}
^SGE {fprintf(out, "%d", SGE);}
^SLT {fprintf(out, "%d", SLT);}
^SLE {fprintf(out, "%d", SLE);}
^IADD {fprintf(out, "%d", IADD);}
^ISUB {fprintf(out, "%d", ISUB);}
^IMUL {fprintf(out, "%d", IMUL);}
^IDIV {fprintf(out, "%d", IDIV);}
^RADD {fprintf(out, "%d", RADD);}
^RSUB {fprintf(out, "%d", RSUB);}
^RMUL {fprintf(out, "%d", RMUL);}
^RDIV {fprintf(out, "%d", RDIV);}
^IUMI {fprintf(out, "%d", IUMI);}
^RUMI {fprintf(out, "%d", RUMI);}
^LNEG {fprintf(out, "%d", LNEG);}
^PUSH {fprintf(out, "%d", PUSH);}
^GOTO {fprintf(out, "%d", GOTO);}
^POP {fprintf(out, "%d", POP);}
^TOINT {fprintf(out, "%d", TOINT);}
^TOREA {fprintf(out, "%d", TOREA);}
^READ {fprintf(out, "%d", READ);}
^WRITE {fprintf(out, "%d", WRITE);}
^MOD {fprintf(out, "%d", MOD);}
^RETURN {fprintf(out, "%d", RETURN);}
^HALT {fprintf(out, "%d", HALT);}
%%
void main()
{ 
  yylex(); 
}
