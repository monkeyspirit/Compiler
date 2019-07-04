//
// Created by maria on 04/07/19.
//


#include "def.h"

int main()
{

    Pnode root = parse();
    syntaxTree(root);

    programLine(root);
    displayTable();

    return(0);
}