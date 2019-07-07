
#include "def.h"
#include "parser.h"
#include "syntaxTree.h"
#include "symbolTable.h"

struct S {
    int i, j, k;
    int next[3];
};

int main() {
    Pnode root = parse();

    syntaxTree(root); // stampa su file l'albero sintattico

    symbolTable(root);
    displayTable();

    return(0);
}