
#include "def.h"
#include "parser.h"
#include "syntaxTree.h"
#include "symbolTable.h"
#include "semantic.h"
#include "tcode.h"
#include "dynamicArray.h"
#include <stdio.h>



int main(int nArgs, char  **args) {
    telaFileName = args[1]; // l'ultimo argomento da linea di comando sarà il nome del file

    char *arg[nArgs-2];

    for (int i = 2; i <nArgs ; ++i) {
        arg[i-2] = args[i];
    }

//    telaFileName = "main";
    Pnode root = parse();

    syntaxTree(root); // stampa su file l'albero sintattico


    PLine rootLine = symbolTable(root);
    displayTable();


    semanticControl(root->child, rootLine);

    genTCode(rootLine, root->child, arg, nArgs);

    return(0);
}