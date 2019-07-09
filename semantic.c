#include "semantic.h"
#include "symbolTable.h"

void semanticControl(PLine rootLine, Pnode root){


    Pnode iterNode, paramNode, declNode, constDeclNode, modNode;
    iterNode = root->child; // punta a ID
    char* idMod = iterNode->value.sval;

    iterNode = iterNode->brother;



    if(iterNode -> value.ival==NOPT_PARAM_LIST){

        paramNode = iterNode->child->child;

        while (paramNode != NULL) {
            paramNode = paramNode->brother;
        }

        iterNode = iterNode->brother;
    }

    iterNode = iterNode->brother;


    if(iterNode->value.ival==NOPT_VAR_SECT){

        declNode = iterNode->child->child;

        while (declNode!=NULL) {

            int decl_type = declNode->child->brother->child->type;
            Pnode idNode = declNode->child->child;

            while(idNode != NULL){

                idNode = idNode->brother;
            }

            declNode = declNode->brother;
        }

        iterNode = iterNode->brother;
    }


    if(iterNode->value.ival==NOPT_CONST_SECT){

        constDeclNode = iterNode->child->child;

        while(constDeclNode != NULL) {

            int decl_type = constDeclNode->child->child->brother->child->type;
            Pnode idNode = constDeclNode->child->child->child;
            Pnode expr = constDeclNode->child->brother;
            constantDeclaration(decl_type, idNode->value.sval, expr);

            while(idNode != NULL){

                idNode = idNode->brother;
            }

            constDeclNode = constDeclNode->brother;
        }

        iterNode = iterNode->brother;                                        //PUNTO A OPT-MODULE/MODULE BODY
    }


    if(iterNode->value.ival==NOPT_MODULE_LIST){

        modNode = iterNode->child;

        while (modNode != NULL) {

            PLine nextModule = findLineByIdAndClass(modNode->child->value.sval, rootLine->class, rootLine->bucket);
            semanticControl(nextModule,modNode);
            modNode = modNode->brother;
        }

        iterNode = iterNode->brother;
    }

    // -------- Controlli ----------
    moduleNameControl(iterNode->child->value.sval, iterNode->child->brother->brother->value.sval, idMod);

    controlOfStatment(iterNode->child->brother->child, rootLine);


}

PLine findLineByIdAndClass(char* id, char* class, PLine *table){


    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toControl = table[i];
        while(toControl!=NULL){
            if(strcmp(id, toControl->id)==0 && strcmp(class, toControl->class)==0) {
                return toControl;
            }
            toControl = toControl->next;
        }
    }


    printf("Errore la riga di \"%s\" non è presente, ma dovrebbe... controlla che non sia una costante\n", id);
    exit(-5);
}

PLine findLineByIdFromPV(char* id, PLine *table){


    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toControl = table[i];
        while(toControl!=NULL){

            if( (strcmp(id, toControl->id)==0) && ( (strcmp("VAR", toControl->class)==0) || (strcmp("PAR", toControl->class)==0) ) ){
//                    printf("Found: %s\n",toControl->id);
                return toControl;
            }
            toControl = toControl->next;
        }
    }


    printf("Errore la riga \"%s\" non è presente, ma dovrebbe", id);
    exit(-5);
}

PLine findLineByIdFromPCV(char* id, PLine *table){


    for(int i=0; i<BUCKET_SIZE; i++){
        PLine toControl = table[i];
        while(toControl!=NULL){

            if( (strcmp(id, toControl->id)==0) && ( (strcmp("VAR", toControl->class)==0) || (strcmp("CON", toControl->class)==0) || (strcmp("PAR", toControl->class)==0) ) ){
//                    printf("Found: %s\n",toControl->id);
                return toControl;
            }
            toControl = toControl->next;
        }
    }


    printf("Errore la riga \"%s\" non è presente, ma dovrebbe", id);
    exit(-5);
}

void controlOfStatment(Pnode stat, PLine moduleLine){

    while(stat!=NULL){ //punta a stat

        int statType = stat->child->value.ival;


        switch (statType){
            case NASSIGN_STAT: {

                Pnode assignStat = stat->child;
                char *type = findLineByIdFromPV(assignStat->child->value.sval, moduleLine->bucket)->type;

                char* typeExpr = typeOfExpr(assignStat->child->brother->child, moduleLine);

                if (strcmp(type, typeExpr) != 0) {
                    printf("Errore nell'assegnamento di \"%s\" nel corpo del modulo %s: richiesto %s invece è presente %s\n", assignStat->child->value.sval, moduleLine->id, type, typeExpr);
                    exit(-9);
                }
                break;
            }
            case NIF_STAT:{
                break;
            }
            case NWHILE_STAT:{
                break;
            }
            case NRETURN_STAT: {

                Pnode returnStat = stat->child;
                char* moduleType = moduleLine->type;
                char* typeExpr;
                if(returnStat->child->child==NULL){
                    typeExpr = "VOID";
                }
                else {
                    typeExpr = typeOfExpr(returnStat->child->child, moduleLine);
                }

                if(strcmp(moduleLine->type, "VOID") != 0 ){
                    if (strcmp(moduleType, typeExpr) != 0) {
                        printf("Errore nell'operazione di return nel corpo del modulo %s: richiesto %s invece è presente %s\n", moduleLine->id, moduleType, typeExpr);
                        exit(-9);
                    }
                }

                break;
            }
            case NREAD_STAT:{
                break;
            }
            case NWRITE_STAT:{
                break;
            }

        }

        stat= stat->brother;
    }

}

/*
 * Controlli:
    1. Uguaglianza del nome del modulo con i nomi che delimitano il corpo del modulo
    2. Definizione delle costanti nella sezione const: compatibilità del valore con il tipo dichiarato
    3. Visibilità degli identificatori referenziati (nella gerarchia degli ambienti)
    4. Compatibilità in numero e tipo dei parametri attuali con i parametri formali
    5. Compatibilità delle espressioni di ritorno con il tipo del valore di ritorno del modulo
    6. Istruzione di return espressa sempre nel modulo che restituisce un valore diverso da void
    7. Compatibilità degli operatori con gli operandi
    8. Compatibilità dell'identificatore con l'espressione di assegnamento
    9. Compatibilità delle espressioni con le istruzioni in cui sono coinvolte
 * Type inference: computazione del tipo risultante di ogni operazione
 */


void moduleNameControl(char* h, char* q, char* id){
    if(strcmp(h,q)){
        printf("Errore gli ID non corrispondono: \"begin %s ... end %s\"", h, q);
        exit(-2);
    }
    else{
        if(strcmp(h, id)){
            printf("Errore gli ID non corrispondono: \"module %s ... begin %s\"",id, h);
            exit(-2);
        }
        else if(strcmp(q,id)){
            printf("Errore gli ID non corrispondono: \"module %s ... end %s\"", id, q);
            exit(-2);
        }
    }
}

void constantDeclaration(int type, char*id, Pnode expr){

    while(expr->child!=NULL){
        expr = expr->child;
        if(expr->value.ival==NUNARYOP){
            expr = expr->brother;
        }
    }

    switch (type){
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
    }

}


void controlFormalPar(Pnode stat, PLine rootLine, PLine *bucket){
    int numPar = 0;

    if(stat->child->child->brother!=NULL){
        Pnode expr =stat->child->child->brother->child->child;
        while(expr!=NULL){
            numPar++;
            expr = expr->brother;
        }
    }

    if(rootLine->nFormalParams!=numPar){
        printf("Errore: nella chiamata al modulo %s il numero di parametri formali non corrisponde a quello dichiarato",rootLine->id);
        exit(-6);
    }

    char* types[numPar];

    Pnode expr =stat->child->child->brother->child->child;
    int i = 0;
    while(expr!=NULL){
        Pnode type = expr;

        while(type->child!=NULL){
            type=type->child;
        }

        types[i] = findLineByIdFromPCV(type->value.sval,bucket)->type;
        expr = expr->brother;
        i++;
    }

    // controllo sui tipi
    PLine *formal = rootLine->formalParams;
    PLine params[numPar];

    for (int j = 0; j <numPar ; ++j) {
        params[j] = formal[j];
    }

    for(int i=0; i <numPar; i++){
        if(strcmp(types[i], params[i]->type)!=0){
            printf("Errore: nella chiamata al modulo %s i tipi dei parametri non corrispondono, ci si aspetta un %s invece è un %s\n", rootLine->id, params[i]->type, types[i]);
            exit(-6);
        }
    }

}

char* typeOfModuleCall(Pnode module, PLine *bucket){
    char *typeOfModule;

    typeOfModule=findLineByIdAndClass(module->child->value.sval, "MOD", bucket)->type;

    return typeOfModule;
}

char* typeOFConstant(int type){

    switch (type) {
        case T_CHARCONST:
            return "CHAR";
        case T_INTCONST:
            return "INT";
        case T_REALCONST:
            return "REAL";
        case T_STRCONST:
            return "STRING";
        case T_BOOLCONST:
            return "BOOL";
    }
}

char* typeOFConditionalExpr(Pnode condition, PLine moduleLine){


    bool opt = false;
    char *typeExprIF = typeOfExpr(condition->child, moduleLine);

    if(strcmp(typeExprIF, "BOOL") != 0 ){
        printf("Errore la condizione dell'If deve essere di tipo BOOL invece è %s\n", typeExprIF);
        exit(-9);
    }
    else{


        char *typeExprThen = typeOfExpr(condition->brother->child, moduleLine);

        char* typeExprOpt;


        if(condition->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){

            typeExprOpt = typeOfElseIfExprList(condition->brother->brother->child, moduleLine);
            opt =true;

            condition = condition->brother;
        }



        char *typeExprElse = typeOfExpr(condition->brother->brother->child, moduleLine);

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

char* typeOfElseIfExprList(Pnode optExpr, PLine moduleLine){

    bool opt = false;

    char *typeExprELSEIF = typeOfExpr(optExpr->child, moduleLine);

    if(strcmp(typeExprELSEIF, "BOOL") != 0 ){
        printf("Errore la condizione dell'Else If deve essere di tipo BOOL invece è %s\n", typeExprELSEIF);
        exit(-9);
    }
    else{

        char *typeExprThen = typeOfExpr(optExpr->brother->child, moduleLine);

        char* typeExprOpt;

        if(optExpr->brother->brother!=NULL && optExpr->brother->brother->value.ival==NOPT_ELSEIF_EXPR_LIST){
            typeExprOpt = typeOfElseIfExprList(optExpr->brother->brother->child, moduleLine);
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


char* typeOfExpr(Pnode x_term, PLine moduleLine){ //expr punta x_term

    char *typeExpr1, *typeExpr2;
    int typeOp;



    if(x_term->child!=NULL ){
        if(x_term->child->value.ival==NFACTOR){
            if(x_term->child->child->type==11){
                typeExpr1 = typeOfExpr(x_term->child, moduleLine);
            }
            else{
                switch (x_term->child->child->value.ival){
                    case NUNARYOP:{
                        int unaryOp =  x_term->child->child->child->type;
                        typeExpr1 = typeOfExpr(x_term->child->child->brother, moduleLine);
                        return unaryOperationChecking(unaryOp, typeExpr1);

                    }
                    case NMODULE_CALL:{
                        typeExpr1 = typeOfModuleCall(x_term->child->child, moduleLine->bucket);
                        break;
                    }
                    case NEXPR:{
                        typeExpr1 = typeOfExpr(x_term->child->child, moduleLine);
                        break;
                    }
                    case NCONSTANT:{
                        typeExpr1 = typeOFConstant(x_term->child->child->child->type);
                        break;
                    }
                    case NCOND_EXPR:{
//                        printf("Typenode: %d\n", x_term->child->child->child->value.ival);

                        typeExpr1 = typeOFConditionalExpr(x_term->child->child->child, moduleLine);
                        break;
                    }
                    case NTYPE:{
//                        typeExpr1 = typeOfModuleCall(x_term->child->child, moduleLine->bucket);
//                        printf("Cast?\n");
                        if(x_term->child->child->child->type == 2) {
                            if(strcmp(typeOfExpr(x_term->child->child->brother, moduleLine), "INT")){
                                printf("Errore nel casting a REAL, ci si aspetta un INT invece è un %s\n", typeOfExpr(x_term->child->child->brother, moduleLine));
                                exit(-3103);
                            }
                            return "REAL";
                        } else if (x_term->child->child->child->type == 1) {
                            if(strcmp(typeOfExpr(x_term->child->child->brother, moduleLine), "REAL")){
                                printf("Errore nel casting a INT, ci si aspetta un REAL invece è un %s\n", typeOfExpr(x_term->child->child->brother, moduleLine));
                                exit(-3103);
                            }
                            return "INT";
                        } else {
                            printf("errore nel casting, operazione non definita\n");
                            exit(-3103);
                        }

                        break;
                    }
                }
            }
        }
        else{
            typeExpr1 = typeOfExpr(x_term->child, moduleLine);
//            printf("Type: %s\n",typeExpr1);

        }
    }
    else{
        if(x_term->type==11  ){
//            printf("Id: %s Type: %s\n",x_term->value.sval, findLineByIdFromPCV(x_term->value.sval, moduleLine->bucket)->type);
            return findLineByIdFromPCV(x_term->value.sval, moduleLine->bucket)->type;
        }
        else if(x_term->type==10 || x_term->type==9 || x_term->type==8 || x_term->type==7 || x_term->type==6){
            switch (x_term->type){
                case 6:
                    return   "CHAR";
                    break;
                case 7:
                    return  "INT";
                    break;
                case 8:
                    return  "REAL";
                    break;
                case 9:
                    return  "STRING";
                    break;
                case 10:
                    return  "BOOL";
                    break;
            }
        }

    }

    if(x_term->brother!=NULL){

        if(x_term->value.ival==32){//rel term è diverso per costruzione
            typeExpr2 = typeOfExpr(x_term->brother->brother, moduleLine);
            typeOp = x_term->brother->child->type;
        }
        else{
            typeExpr2 = typeOfExpr(x_term->brother->child->brother, moduleLine);
            typeOp = x_term->brother->child->child->type;
        }


        return operationChecking(typeOp, typeExpr1, typeExpr2);

    }
    else{
        return typeExpr1;
    }


}

char* unaryOperationChecking(int op, char *typeExpr1){
    switch (op){
        case 15: {//meno unario
            if( (strcmp(typeExpr1, "INT")==0) ||  (strcmp(typeExpr1, "REAL")==0)){
                return typeExpr1;
            }
            else{
                printf("Errore l'operatore \"-\" usato come unario richiede o INT o REAL, invece è iserito un %s\n", typeExpr1);
                exit(-8);
            }

        }
        case 17:{//not
            if( (strcmp(typeExpr1, "BOOL")==0)){
                return typeExpr1;
            }
            else{
                printf("Errore l'operatore \"not\" richiede un BOOL, invece è iserito un %s\n", typeExpr1);
                exit(-8);
            }
        }
    }
}

char* operationChecking(int op, char *typeExpr1, char *typeExpr2){
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

    }

}