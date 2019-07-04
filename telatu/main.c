#include "def.h"

int main() {

    Pnode root = parse();
    treePrint(root);

    syntax(root);
    //displayTable();

    printf("tutto ok");
    return(0);
}
