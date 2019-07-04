//
// Created by maria on 04/07/19.
//


#include "def.h"
#include "parser.h"
#include "syntaxTree.h"
#include "symbolTable.h"

int main()
{

    Pnode root = parse();
    syntaxTree(root);

    programLine(root);
    displayTable();

    return(0);
}