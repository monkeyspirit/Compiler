
#include "def.h"
#include "parser.h"
#include "syntaxTree.h"
#include "symbolTable.h"
#include "semantic.h"

struct S {
    int i, j, k;
    int next[3];
};

int main() {
    Pnode root = parse();

    syntaxTree(root); // stampa su file l'albero sintattico

    PLine rootLine = symbolTable(root);
    semanticControl(rootLine, root->child);
    displayTable();

    return(0);
}