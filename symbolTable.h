/*
 * Header file del symbolTable.c
 */

/*
 * Definizione della symbolTable
 */
typedef struct structLine{
    char* id;
    //PUNTATORE
    char* class;
    char* type;
    struct structLine *bucket; // AMBIENTE
    //Formali
    int numParam;
    struct structLine *next;

} Line;


/*
 * Definizione metodi e tipi
 */
typedef Line *PLine;

PLine symbolTable[50], newLine();
void displayTable(),
        programLine(Pnode),
        module_declLine(Pnode),
        param_declLine(Pnode),
        vardecl_listLines(int, Pnode),
        vardecl_Line(int, Pnode),
        constdecl_listLines(int, Pnode),
        constdecl_Line(int, Pnode),
        addLine(PLine,char* );

int hash(char*), search(int );
