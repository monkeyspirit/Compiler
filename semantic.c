#include "semantic.h"
#include "symbolTable.h"

/*
Controlli:
    1. Uguaglianza del nome del modulo con i nomi che delimitano il corpo del modulo
    2. Definizione delle costanti nella sezione const: compatibilità del valore con il tipo dichiarato
    3. Visibilità degli identificatori referenziati (nella gerarchia degli ambienti)
    4. Compatibilità in numero e tipo dei parametri attuali con i parametri formali
    5. Compatibilità delle espressioni di ritorno con il tipo del valore di ritorno del modulo
    6. Istruzione di return espressa sempre nel modulo che restituisce un valore diverso da void
    7. Compatibilità degli operatori con gli operandi
    8. Compatibilità dell'identificatore con l'espressione di assegnamento
    9. Compatibilità delle espressioni con le istruzioni in cui sono coinvolte
Type inference: computazione del tipo risultante di ogni operazione
*/

PLine wholeSymbolTable; // riferimento alla testa dell'INTERA symbol table

// Funzioni di utilità per la ricerca nella symbol table

int getGapModuleAmbient(PLine caller, PLine called){

    //m1 chiama m2

    if(caller == called && caller == wholeSymbolTable){
        return 0;
    }else{
        //m1 genitore di m2
        if(isChildDirectly(called, caller)!=0){
            return 0;
        }
            //m1 fratello m2
        else if(caller!=wholeSymbolTable && called!=wholeSymbolTable && isBrother(caller, called)==1){
            return 1;
        }
            //altrimenti
        else{
            /*...*/
            return getLevelModule(caller, wholeSymbolTable)-getLevelModule(called,wholeSymbolTable)+1;

             /*...*/
        }
    }


}

int objectInBuckets(PLine *bucket){

}


int getLevelModule(PLine module, PLine table){


    for (int i = 0; i <BUCKET_SIZE ; ++i) {
        PLine line = table->bucket[i];
        while(line!=NULL){

            if(module==line){
                return 0;
            }
            if(getLevelModule(module, line)!=-1){
                return 1+getLevelModule(module, line);
            }
            line = line->next;
        }
    }

    return -1;

}

int isChildDirectly(PLine probableChild, PLine probableFather){

    int gap = 0;

    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toControl = probableFather->bucket[i];


        while (toControl!=NULL){
            if(toControl==probableChild){
                return  1;
            }

            toControl = toControl->next;
        }

    }

}

PLine getFather(PLine child, PLine table){

    if(child == table){
        return table;
    }

    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toControl = table->bucket[i];

        while (toControl!=NULL){
            if(toControl==child){
                return table;
            }

            toControl = toControl->next;
        }

    }

    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toControl = table->bucket[i];

        while (toControl!=NULL){
            if(strcmp(toControl->class, "MOD")==0){
                if(getFather(child, toControl)!=NULL){
                    return getFather(child, toControl);
                }
            }
            toControl = toControl->next;
        }

    }

    return NULL;
}

int isBrother(PLine module1, PLine module2){

    PLine fatherM1 = getFather(module1, wholeSymbolTable);
    PLine fatherM2 = getFather(module2, wholeSymbolTable);

    if(fatherM1==fatherM2){
        return 1;

    }

    return 0;
}


// dato l'oid di un modulo, cerca ricorsivamente quale modulo è suo padre
PLine findModuleFather(PLine moduleLine, int childModuleOid) {
    for(int i=0; i<BUCKET_SIZE; i++){
        PLine line = moduleLine->bucket[i];
        while (line != NULL) {
            if (strcmp(line->class, "MOD")==0){
                if(line->oid == childModuleOid)
                    return moduleLine;
                else {
                    PLine line1 = findModuleFather(line, childModuleOid);
                    if(line1 != NULL)
                        return line1;

                }
            }
            line = line->next;
        }
    }
    return NULL;
}

// cerca gerarchicamente (in risalita) nella symboltable una riga con un dato id
PLine findLineById(char* id, PLine fatherModuleLine){

    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toControl = fatherModuleLine->bucket[i];
        while(toControl!=NULL){

            if( strcmp(id, toControl->id)==0 )
                return toControl;

            toControl = toControl->next;
        }
    }

    if(fatherModuleLine->oid != wholeSymbolTable->oid){
        PLine fatherModule = findModuleFather(wholeSymbolTable, fatherModuleLine->oid);
        if(fatherModule!=NULL) {
            return findLineById(id, fatherModule);
        } else
            printf("PROBLEMONE GROSSO COME UNA * DI CASA");

    }

    if( strcmp(id, wholeSymbolTable->id)==0 )
        return wholeSymbolTable;

    printf("Errore la riga \"%s\" non è presente, ma dovrebbe", id);
    exit(-5);
}


// Analisi semantica

// dati due tipi e un operazione controlla che questa sia valida e ritorna il tipo totale
char* checkOperationAndGetType(int op, char *typeExpr1, char *typeExpr2){
    switch (op){
        //logic expr:
        case 24 : case 25:
            if( (strcmp(typeExpr1, "BOOL")!=0) || (strcmp(typeExpr2, "BOOL")!=0)){
                printf("Errore nell'espressione logica: richiesto un BOOL, invece sono inseriti un %s e un %s\n", typeExpr1, typeExpr2);
                exit(-8);
            }
            return "BOOL";

            //rel expr:
        case 18 : case 19: case 20: case 21: case 22: case 23:
            if( (strcmp(typeExpr1,typeExpr2)!=0)){
                printf("Errore nell'espressione relazionale: richiesti tipi uguali, invece sono inseriti un %s e un %s\n", typeExpr1, typeExpr2);
                exit(-8);
            }
            return "BOOL";

            //math expr:
        case 13 : case 14: case 15: case 16:
            if( ( (strcmp(typeExpr1, "INT")==0) ||  (strcmp(typeExpr1, "REAL")==0) ) && (strcmp(typeExpr1,typeExpr2)==0) ){
                return typeExpr1;
            }
            else{
                printf("Errore nell'espressione relazionale: richiesti tipi uguali o INT o REAL, invece sono inseriti un %s e un %s\n", typeExpr1, typeExpr2);
                exit(-8);
            }
        default:
            return NULL;

    }

}

// controlla che la const-decl sia corretta (tipo dell'espressione e visibilità degli attributi)
void checkConstDecl(Pnode constDeclNode, PLine moduleLine){

    char *id = constDeclNode->child->child->child->value.sval;
    PLine constLine = findLineById(id, moduleLine);

    Pnode expr = constDeclNode->child->brother;
    char *exprType = getExprType(expr, moduleLine);

    if(strcmp(constLine->type, exprType)!=0){
        printf("Errore nell'espressione della costante %s\n richiesto un %s ma trovato un %s",
               id, constLine->type, exprType);
        exit(-15);
    }
    /*
    while(expr->child!=NULL){
        expr = expr->child;
        if(expr->value.ival==NUNARYOP)
            expr = expr->brother;
    }

    switch (constDeclType){
        case T_CHAR:
            if(expr->type!=T_CHARCONST  && expr->type!=T_ID ){
                printf("Errore: id \"%s\" ci si aspetta un CHAR\n", id);
                exit(-3);
            }
            break;
        case T_INT:
            if(expr->type!=T_INTCONST){
                printf("Errore: id \"%s\" ci si aspetta un INT\n", id);
                exit(-3);
            }
            break;
        case T_REAL:
            if(expr->type!=T_REALCONST){
                printf("Errore: id \"%s\" ci si aspetta un REAL\n", id);
                exit(-3);
            }
            break;
        case T_STRING:
            if(expr->type!=T_STRCONST){
                printf("Errore: id \"%s\" ci si aspetta un STRING\n", id);
                exit(-3);
            }
            break;
        case T_BOOL:
            if(expr->type!=T_BOOLCONST){
                printf("Errore: id \"%s\" ci si aspetta un BOOL\n",id);
                exit(-3);
            }
            break;
        default:
            printf("operazione non supportata");
            exit(-1);
    }*/

}

// controlla i parametri (espressioni) passati alla chiamata di un modulo (sia il numero che il tipo)
void checkModuleCallParams(Pnode optParamListNode, PLine calledModuleLine, PLine fatherModuleLine){

    int numGivenParams = 0;
    char **types = malloc(0);

    if (optParamListNode!=NULL ) { // && optParamListNode->value.ival == NOPT_EXPR_LIST) {
        Pnode paramExpr = optParamListNode->child->child;

        while (paramExpr != NULL) {
            types = realloc(types, (numGivenParams + 1) * sizeof(char *));
            types[numGivenParams++] = getExprType(paramExpr->child, fatherModuleLine);

            paramExpr = paramExpr->brother;
        }
    }

//    char *types[numGivenParams];
/*
    paramExpr = firstParamExprNode;
    for (int i = 0; i < numGivenParams; ++i) {
        types[i] = getExprType(paramExpr->child, ambiente);
        paramExpr = paramExpr->brother;
    }*/

    if(calledModuleLine->nFormalParams!=numGivenParams){
        printf("Errore: nella chiamata al modulo %s il numero di parametri formali non corrisponde a quello dichiarato",
               calledModuleLine->id);
        exit(-6);
    }

    // controllo sui tipi
    PLine *formal = calledModuleLine->formalParams;
    for (int i = 0; i < numGivenParams; ++i)
        if(strcmp(types[i], formal[i]->type) != 0){
            printf("Errore nei parametri della chiamata al modulo %s\n"
                   "all'indice %d ci si aspetta un %s, trovato invece un %s\n",
                   calledModuleLine->id, i+1, formal[i]->type, types[i]);
            exit(-6);
        }

}

// analisi semantica della opt-else-if-stat
void optElseIfStatListControl(Pnode optElseIfStatListNode, PLine fatherModuleLine){
    Pnode expr = optElseIfStatListNode->child;
    char *type = getExprType(expr->child, fatherModuleLine);

    if(strcmp(type, "BOOL")!=0){
        printf("Errore nell'If Stat nel corpo del modulo %s: "
               "richiesto un BOOL invece è presente %s\n", fatherModuleLine->id, type);
        exit(-9);
    }

    Pnode statList = expr->brother;
    statListControl(statList->child, fatherModuleLine);

    if(statList->brother!=NULL && statList->brother->value.ival==NOPT_ELSEIF_STAT_LIST){
        optElseIfStatListControl(statList->brother, fatherModuleLine);
    }

}

// metodo per l'analisi semantica di ogni stat
void statListControl(Pnode firstStatNode, PLine fatherModuleLine){

    Pnode statNode = firstStatNode;
    while(statNode!=NULL) { // cicliamo le stat

        int statType = statNode->child->value.ival;

        switch (statType){
            case NASSIGN_STAT: {
                Pnode assignStat = statNode->child;
                PLine leftOperand = findLineById(assignStat->child->value.sval, fatherModuleLine);
                if (strcmp(leftOperand->class, "CON")==0 || strcmp(leftOperand->class, "MOD")==0){
                    printf("Errore: l'assegnamento puo' essere fatto solo a VAR e PAR trovato %s", leftOperand->class);
                    exit(-12);
                }
                char *type = leftOperand->type;
                char *typeExpr = getExprType(assignStat->child->brother->child, fatherModuleLine);
                if (strcmp(type, typeExpr) != 0) {
                    printf("Errore nell'assegnamento di \"%s\" nel corpo del modulo %s:"
                           " richiesto %s invece è presente %s\n",
                           assignStat->child->value.sval, fatherModuleLine->id, type, typeExpr);
                    exit(-9);
                }
            }
                break;

            case NIF_STAT: {

                Pnode ifStat = statNode->child;
                char *type = getExprType(ifStat->child->child, fatherModuleLine);
                if(strcmp(type, "BOOL")!=0){
                    printf("Errore nell'If Stat nel corpo del modulo %s: richiesto un BOOL invece è presente %s\n", fatherModuleLine->id, type);
                    exit(-9);
                }

                Pnode statList = ifStat->child->brother;
                statListControl(statList->child, fatherModuleLine);

                if(statList->brother!=NULL) {
                    if (statList->brother->value.ival == NOPT_ELSEIF_STAT_LIST) {
                        optElseIfStatListControl(statList->brother, fatherModuleLine);
//                        statList = statList->brother;
                    } else if (statList->brother->value.ival == NOPT_ELSE_STAT)
                        statListControl(statList->brother->child->child, fatherModuleLine);
                }
            }
                break;

            case NWHILE_STAT:

                if(strcmp(getExprType(statNode->child->child->child, fatherModuleLine), "BOOL")!=0) {
                    printf("Errore nella condizione del ciclo while, richiesto un BOOL ma trovato un %s\n",
                           getExprType(statNode->child->child->child, fatherModuleLine));
                    exit(-72);
                }
                statListControl(statNode->child->child->brother->child, fatherModuleLine);
                break;

            case NRETURN_STAT: {

                Pnode returnStat = statNode->child;
                char *moduleType = fatherModuleLine->type, *typeExpr;

                if(returnStat->child->child==NULL)
                    typeExpr = "VOID";
                else
                    typeExpr = getExprType(returnStat->child->child, fatherModuleLine);

                if (strcmp(moduleType, typeExpr) != 0) {
                    printf("Errore nell'operazione di return nel corpo del modulo %s: richiesto %s invece è presente %s\n", fatherModuleLine->id, moduleType, typeExpr);
                    exit(-9);
                }

            }
                break;

            case NREAD_STAT: {
                Pnode idNode = statNode->child->child->child;
                while (idNode != NULL) {
                    PLine idLine = findLineById(idNode->value.sval, fatherModuleLine);
                    if (strcmp(idLine->class, "CON") == 0 || strcmp(idLine->class, "MOD") == 0) {
                        printf("Errore: nelle read stat si aspetta un VAR o PAR "
                               "trovato invece %s", idLine->class);
                        exit(-13);
                    }
                    idNode = idNode->brother;
                }
            }
                break;

            case NWRITE_STAT:{
                Pnode exprNode = statNode->child->child->child;
                while (exprNode != NULL) {
                    getExprType(exprNode->child, fatherModuleLine); // non serve veramente il tipo... la uso solo perchè fa i controlli

                    exprNode = exprNode->brother;
                }
            }
                break;

            case NMODULE_CALL: {

                PLine calledModuleLine = findLineById(statNode->child->child->value.sval, fatherModuleLine);
                if (strcmp(calledModuleLine->class, "MOD") != 0){
                    printf("Errore: l'id %s non risulta essere un modulo ma un %s", calledModuleLine->id, calledModuleLine->class);
                    exit(-11);
                }
                checkModuleCallParams(statNode->child->child->brother,
                                      calledModuleLine,
                                      fatherModuleLine);
            }
                break;

            default:
                printf("stat non riconosciuta");
                exit(-1);

        }

        statNode= statNode->brother;
    }

}

// metodo per l'analisi di un modulo (analizza costanti, sottomoduli ricorsivamente e statements)
void moduleControl(Pnode moduleNode, PLine fatherModuleLine){

    Pnode iterNode = moduleNode->child; // punta a ID
    char* idMod = iterNode->value.sval;

    iterNode = iterNode->brother;

    if(iterNode -> value.ival==NOPT_PARAM_LIST){
        /*
        paramNode = iterNode->child->child;

        while (paramNode != NULL) {
            paramNode = paramNode->brother;
        }
        */

        iterNode = iterNode->brother;
    }

    iterNode = iterNode->brother;


    if(iterNode->value.ival==NOPT_VAR_SECT){
        /*
        declNode = iterNode->child->child;

        while (declNode!=NULL) {

            int decl_type = declNode->child->brother->child->type;
            Pnode idNode = declNode->child->child;

            while(idNode != NULL){

                idNode = idNode->brother;
            }

            declNode = declNode->brother;
        }
        */

        iterNode = iterNode->brother;
    }


    if(iterNode->value.ival==NOPT_CONST_SECT){

        Pnode constDeclNode = iterNode->child->child;

        while(constDeclNode != NULL) {

            checkConstDecl(constDeclNode, fatherModuleLine);

            /*
            while(idNode != NULL){
                idNode = idNode->brother;
            }
            */

            constDeclNode = constDeclNode->brother;
        }

        iterNode = iterNode->brother;
    }

    if(iterNode->value.ival==NOPT_MODULE_LIST){

        Pnode innerModuleNode = iterNode->child;

        while (innerModuleNode != NULL){

            PLine nextModule = findLineById(innerModuleNode->child->value.sval, fatherModuleLine);
//            PLine nextModule = findLineByIdAndClass(innerModuleNode->child->value.sval, moduleLine->class, moduleLine);
            moduleControl(innerModuleNode, nextModule);
            innerModuleNode = innerModuleNode->brother;
        }

        iterNode = iterNode->brother;
    }

    // Controllo che gli id davanti a begin e a end corrispondano all'id del modulo
    if(strcmp(idMod, iterNode->child->value.sval)!=0){
        printf("Errore gli ID non corrispondono: \"module %s ... begin %s\"", idMod, iterNode->child->value.sval);
        exit(-2);
    } else if(strcmp(idMod, iterNode->child->brother->brother->value.sval)!=0){
        printf("Errore gli ID non corrispondono: \"module %s ... end %s\"", idMod, iterNode->child->brother->brother->value.sval);
        exit(-2);
    }

    statListControl(iterNode->child->brother->child, fatherModuleLine); //Gli passo il primo stat

}

// metodo richiamato dall'esterno per cominciare l'analisi
void semanticControl(Pnode rootNode, PLine rootLine){
    wholeSymbolTable = rootLine;
    moduleControl(rootNode, rootLine);
}

// ritorna il tipo della opt-else-ifexpr e controlla che l'expr sia scritta correttamente
char* getOptElseIfExprType(Pnode optExpr, PLine fatherModuleLine){

    bool opt = false;

    char *typeExprELSEIF = getExprType(optExpr->child, fatherModuleLine);

    if(strcmp(typeExprELSEIF, "BOOL") != 0 ){
        printf("Errore la condizione dell'Else If deve essere di tipo BOOL invece è %s\n", typeExprELSEIF);
        exit(-9);
    }
    else{

        char *typeExprThen = getExprType(optExpr->brother->child, fatherModuleLine);

        char* typeExprOpt=NULL;

        if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
            typeExprOpt = getOptElseIfExprType(optExpr->brother->brother->child, fatherModuleLine);
            opt =true;

        }

        if(opt){
            if( strcmp(typeExprThen,  typeExprOpt)==0 ){
                return typeExprThen;
            }
            else{
                printf("Errore nella condizione Else If: ... then ...[%s] elseif ...[%s]  end\n", typeExprThen,typeExprOpt );
                exit(-9);
            }
        }
        else{
            return typeExprThen;
        }
    }
}

// ritorna il tipo della conditional-expr e controlla che l'expr sia scritta correttamente
char* getCondExprType(Pnode condition, PLine fatherModuleLine){

    bool opt = false;
    char *typeExprIF = getExprType(condition->child, fatherModuleLine);

    if(strcmp(typeExprIF, "BOOL") != 0 ){
        printf("Errore la condizione dell'If deve essere di tipo BOOL invece è %s\n", typeExprIF);
        exit(-9);
    }
    else{


        char *typeExprThen = getExprType(condition->brother->child, fatherModuleLine);

        char* typeExprOpt=NULL;


        if(condition->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){

            typeExprOpt = getOptElseIfExprType(condition->brother->brother->child, fatherModuleLine);
            opt =true;

            condition = condition->brother;
        }



        char *typeExprElse = getExprType(condition->brother->brother->child, fatherModuleLine);

        if(opt){
            if( strcmp(typeExprThen,  typeExprOpt)==0 && strcmp(typeExprThen,  typeExprElse)==0){
                return typeExprThen;
            }
            else{
                printf("Errore nella condizione If: if... then ...[%s] elseif ...[%s] else ...[%s] end\n", typeExprThen,typeExprOpt, typeExprElse );
                exit(-9);
            }
        }
        else{
            if (strcmp(typeExprThen,  typeExprElse)!=0){
                printf("Errore nella condizione If: if... then ...[%s] else ...[%s] end\n", typeExprThen,typeExprElse );
                exit(-9);
            }
            else{
                return typeExprThen;
            }
        }
    }

}

/*
char* getExprType2(Pnode node, PLine fatherModuleLine){ //expr punta x_term

    char *leftExprType = "";
    int typeOp;

    if(node!=NULL) {

        if (node->child == NULL) { // se siamo ai minimi termini
            switch (node->type) {
                case 6:
                    return "CHAR";
                case 7:
                    return "INT";
                case 8:
                    return "REAL";
                case 9:
                    return "STRING";
                case 10:
                    return "BOOL";
                case 11: {
                    // printf("Id: %s Type: %s\n",node->value.sval, findLineByIdFromPCV(node->value.sval, ambiente->bucket)->type);
                    PLine line = findLineById(node->value.sval, fatherModuleLine);
                    if (strcmp(line->class, "MOD") == 0) {
                        printf("Errore: chiamata non valida al modulo %s", line->id);
                        exit(-14);
                    }
                    return line->type;
                }
                default:
                    printf("Errore: tipo \"%d\"non supportato", node->type);
                    exit(-1);
            }
        } else { // passo ricorsivo

            if (node->child->value.ival != NFACTOR)
                leftExprType = getExprType(node->child, fatherModuleLine);
            else {
                if (node->child->child->type == 11)  // per gli id scendiamo di livello
//                printf("%s\n", node->child->child->value.sval);
                    leftExprType = getExprType(node->child, fatherModuleLine);
                else {
                    switch (node->child->child->value.ival) {
                        case NUNARYOP: {
                            leftExprType = getExprType(node->child->child->brother, fatherModuleLine);

                            // Checking del tipo sull'operatore unario
                            switch (node->child->child->child->type) {
                                case 15: // meno unario
                                    if (strcmp(leftExprType, "INT") != 0 || strcmp(leftExprType, "REAL") != 0) {
                                        printf("Errore l'operatore unario \"-\" richiede o INT o REAL, trovato invece un %s\n", leftExprType);
                                        exit(-8);
                                    }
                                    break;
                                case 17: // not
                                    if (strcmp(leftExprType, "BOOL") != 0) {
                                        printf("Errore l'operatore unario \"not\" richiede un BOOL, trovato invece un %s\n", leftExprType);
                                        exit(-8);
                                    }
                                    break;
                                default:
                                    printf("Errore operatore unario non riconosciuto %d", node->child->child->child->type);
                                    exit(-1);
                            }

                            return leftExprType;

                        }
                        case NMODULE_CALL: {
                            PLine calledModuleLine = findLineById(node->child->child->child->value.sval, fatherModuleLine);
                            if (strcmp(calledModuleLine->class, "MOD") != 0){
                                printf("Errore: l'id %s non risulta essere un modulo ma un %s", calledModuleLine->id, calledModuleLine->class);
                                exit(-11);
                            }
                            leftExprType = calledModuleLine->type;
                            checkModuleCallParams(node->child->child->child->brother,
                                                  calledModuleLine,
                                                  fatherModuleLine);
                            break;
                        }
                        case NEXPR:
                            leftExprType = getExprType(node->child->child, fatherModuleLine);
                            break;

                        case NCONSTANT: {
                            switch (node->child->child->child->type) {
                                case T_CHARCONST:
                                    leftExprType = "CHAR";
                                    break;
                                case T_INTCONST:
                                    leftExprType = "INT";
                                    break;
                                case T_REALCONST:
                                    leftExprType = "REAL";
                                    break;
                                case T_STRCONST:
                                    leftExprType = "STRING";
                                    break;
                                case T_BOOLCONST:
                                    leftExprType = "BOOL";
                                    break;
                                default:
                                    printf("Errore type expr");
                                    exit(-1);
                            }
                            break;
                        }
                        case NCOND_EXPR: {
                            leftExprType = getCondExprType(node->child->child->child, fatherModuleLine);
                            break;
                        }
                        case NTYPE: {
//                        typeExpr1 = typeOfModuleCall(x_term->child->child, moduleLine->bucket);
//                        printf("Cast?\n");
                            if (node->child->child->child->type == 2) {
                                if (strcmp(getExprType(node->child->child->brother, fatherModuleLine), "INT") != 0) {
                                    printf("Errore nel casting a REAL, ci si aspetta un INT invece è un %s\n",
                                           getExprType(node->child->child->brother, fatherModuleLine));
                                    exit(-3103);
                                }
                                return "REAL";
                            } else if (node->child->child->child->type == 1) {
                                if (strcmp(getExprType(node->child->child->brother, fatherModuleLine), "REAL") != 0) {
                                    printf("Errore nel casting a INT, ci si aspetta un REAL invece è un %s\n",
                                           getExprType(node->child->child->brother, fatherModuleLine));
                                    exit(-3103);
                                }
                                return "INT";
                            } else {
                                printf("errore nel casting, operazione non definita\n");
                                exit(-3103);
                            }
                        }
                    }
                }
            }

        }


        if (node->brother == NULL)
            return leftExprType;
        else { // se c'è una parte destra bisogna controllare
            char *rightExprType;

            if (node->value.ival == 32) { // rel term è diverso per costruzione
                rightExprType = getExprType(node->brother->brother, fatherModuleLine);
                typeOp = node->brother->child->type;
            } else {
                rightExprType = getExprType(node->brother->child->brother, fatherModuleLine);
                typeOp = node->brother->child->child->type;
            }

            return checkOperationAndGetType(typeOp, leftExprType, rightExprType);
        }

    }

}/**/

// data un'espressione ne ricava il tipo totale controllando la semantica
char* getExprType(Pnode node, PLine fatherModuleLine){ //expr punta x_term

    char *leftExprType = "";
//    int typeOp;

    if (node->child == NULL) { // se siamo ai minimi termini
        switch (node->type) {
            case 6:
                return "CHAR";
            case 7:
                return "INT";
            case 8:
                return "REAL";
            case 9:
                return "STRING";
            case 10:
                return "BOOL";
            case 11: {
                // printf("Id: %s Type: %s\n",node->value.sval, findLineByIdFromPCV(node->value.sval, ambiente->bucket)->type);
                PLine line = findLineById(node->value.sval, fatherModuleLine);
                if (strcmp(line->class, "MOD") == 0) {
                    printf("Errore: chiamata non valida al modulo %s", line->id);
                    exit(-14);
                }
                return line->type;
            }
            default:
                printf("Errore: tipo \"%d\" non supportato", node->type);
                exit(-1);
        }
    }
    // passo ricorsivo
    switch (node->value.ival) {
        case NUNARYOP:
            leftExprType = getExprType(node->brother, fatherModuleLine);
            switch (node->child->type) {
                case 15: // meno unario
                    if (strcmp(leftExprType, "INT") != 0 && strcmp(leftExprType, "REAL") != 0) {
                        printf("Errore l'operatore unario \"-\" richiede o INT o REAL, trovato invece un %s\n", leftExprType);
                        exit(-8);
                    }
                    break;
                case 17: // not
                    if (strcmp(leftExprType, "BOOL") != 0) {
                        printf("Errore l'operatore unario \"not\" richiede un BOOL, trovato invece un %s\n", leftExprType);
                        exit(-8);
                    }
                    break;
                default:
                    printf("Errore operatore unario: non riconosciuto %d", node->child->child->child->type);
                    exit(-1);
            }
            return leftExprType;
            break;
        case NCOND_EXPR:
            leftExprType = getCondExprType(node->child, fatherModuleLine);
            break;
        case NTYPE:
            if (node->child->type == 2) {
                if (strcmp(getExprType(node->brother, fatherModuleLine), "INT") != 0) {
                    printf("Errore nel casting a REAL, ci si aspetta un INT invece è un %s\n",
                           getExprType(node->brother, fatherModuleLine));
                    exit(-3103);
                }
                return "REAL";
            } else if (node->child->type == 1) {
                if (strcmp(getExprType(node->brother, fatherModuleLine), "REAL") != 0) {
                    printf("Errore nel casting a INT, ci si aspetta un REAL invece è un %s\n",
                           getExprType(node->brother, fatherModuleLine));
                    exit(-3103);
                }
                return "INT";
            } else {
                printf("errore nel casting, operazione non definita\n");
                exit(-3103);
            }
            break;
        case NMODULE_CALL: {
            PLine calledModuleLine = findLineById(node->child->value.sval, fatherModuleLine);
            if (strcmp(calledModuleLine->class, "MOD") != 0){
                printf("Errore: l'id %s non risulta essere un modulo ma un %s", calledModuleLine->id, calledModuleLine->class);
                exit(-11);
            }
            leftExprType = calledModuleLine->type;
            checkModuleCallParams(node->child->brother,
                                  calledModuleLine,
                                  fatherModuleLine);
            break;
        }
        default:
            leftExprType = getExprType(node->child, fatherModuleLine);
    }

    if (node->brother == NULL)
        return leftExprType;
    else { // se c'è una parte destra bisogna controllare
        char *rightExprType;
        int typeOp;
        switch (node->brother->value.ival) {
            case NREL_TERM1:
            case NLOW_TERM1:
            case NEXPR1:
                typeOp = node->brother->child->child->type;
                rightExprType = getExprType(node->brother->child->brother, fatherModuleLine);
                break;
            case NRELOP:
                typeOp = node->brother->child->type;
                rightExprType = getExprType(node->brother->brother, fatherModuleLine);
                break;
            default:
                printf("Errore nella parte destra dell'operazione %d", node->brother->value.ival);
                exit(-189);
        }

        return checkOperationAndGetType(typeOp, leftExprType, rightExprType);
    }

//    }

}