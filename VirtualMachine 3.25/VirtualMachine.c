#define STEP 30
#include "libs/ActivationRecord.h"
#include "libs/OpList.h"
#include "libs/Executor.h"
#include "libs/StringDB.h"
#include "libs/Interpreter.h"
#include "libs/lexer.c"


typedef struct {
    Interpreter* gfl;
    StringDB* sdb;
    OpList* list;
    char** params;
    int parsLeft;
}VirtualMachine;

VirtualMachine newVM(){
    VirtualMachine vm;
    vm.gfl = malloc(sizeof(Interpreter));
    vm.sdb = malloc(sizeof(StringDB));
    vm.list = malloc(sizeof(OpList));
    *(vm.gfl) = newInterpreter();
    *(vm.sdb) = newStringDB();
    *(vm.list) = newOpList();
    return vm;
}

void READf (StringDB* sdb, Interpreter* gfl, char type, int jumps, int oID) {
    Union u;
    switch (type) {
        case 'c' : {
            char value;
            do{
            value = getchar();
            }while(value==10);
            u.data.charap = value;
            u.dimension = 1;
        }break;
        case 's' : {
            char* value = malloc(100);
            scanf("%[^\n]%*c", value); //un po bruttino ma era per non eseguire la gets
            value = getStringPointer(sdb, value);
            u.data.string = value;
            u.dimension = 8;
        }break;
        case 'i' : {
            int value;
            scanf("%d", &value);
            u.data.integer = value;
            u.dimension = 4;
        }break;
        case 'r' : {
            float value;
            scanf("%f", &value);
            u.data.real = value;
            u.dimension = 4;
        }
        
    }
    pushExeUnion(gfl->exe, u);
    STOf(gfl, jumps, oID);
}

void READfromArgs(VirtualMachine* vm, char type){
    if (vm->parsLeft <= 0){
        printf("ERROR: Numero di argomenti non sufficienti, esecuzione terminata");
        exit(1);
    }
    StringDB* sdb = vm->sdb;
    Union u;
    char* string = *(vm->params);
    switch (type) {
        case 'c' : {
            char value;
            sscanf(string, "%c", &value);
            u.data.charap = value;
            u.dimension = 1;
        }break;
        case 's' : {
            char* value = malloc(100);
            sscanf(string, "%[^\n]%*c", value); //un po bruttino ma era per non eseguire la gets
            value = getStringPointer(sdb, value);
            u.data.string = value;
            u.dimension = 8;
        }break;
        case 'i' : {
            int value;
            sscanf(string, "%d", &value);
            u.data.integer = value;
            u.dimension = 4;
        }break;
        case 'r' : {
            float value;
            sscanf(string, "%f", &value);
            u.data.real = value;
            u.dimension = 4;
        }
        
    }
    pushExeUnion(vm->gfl->exe, u);
    vm->parsLeft--;
    vm->params++;
}

/* deprecated
void WRITEf (ExeStack* exe, char* params) {
    int i = 0; char p;
    while( ( p = params[i]) != '\0' ) {
        switch (p) {
            case 'i' : printf("output: %d\n", pullExeInt(exe)); break;
            case 'r' : printf("output: %f\n", pullExeReal(exe));break;
            case 's' : 
            {
                char* stringa;
                stringa = pullExeString(exe);
                if (strcmp (stringa, "lamperti") == 0 )
                    printf("Lunga vita al nostro amato presidente del consiglio della Repubblica Italiana <3\n");
                else 
                    printf("output: %s\n", stringa);
            }break;
            case 'c' : printf("output: %c\n", pullExeChar(exe));
            
        }
        i++;
    }
}*/

void WRITEf (ExeStack* exe, char* params) {
    int length = strlen(params);
    Union temp[length];
    for ( int i = 0; i < length; i++)
        temp[i] = pullExeUnion(exe);
    for ( int i = 0; i < length; i++){
        Union current = temp[(length-1)-i];
        switch (params[i]) {
            case 'i' : printf("%d\n", current.data.integer); break;
            case 'r' : printf("%f\n", current.data.real);break;
            case 's' : 
            {
                char* stringa;
                stringa = current.data.string;
                if (strcmp (stringa, "lamperti") == 0 )
                    printf("Lunga vita al nostro amato presidente del consiglio della Repubblica Italiana Conte <3\n");
                else 
                    printf("%s\n", stringa);
            }break;
            case 'c' : printf("%c\n", current.data.charap);
            
        }
    }
}

void execute(VirtualMachine* vm, Op* operation) {
    switch ( operation->command ) {
        
        case NEW : {
            int first = operation->arg1.data.integer;
            NEWf (vm->gfl, first);
        }break;
        
        case LDI : {
            int first = operation->arg1.data.integer;
            pushExeInt (vm->gfl->exe, first);
        }break;
        
        case LDC : {
            char first = operation->arg1.data.charap;
            pushExeChar (vm->gfl->exe, first);
        }break;
        
        case LDR : {
            float first = operation->arg1.data.real;
            pushExeReal (vm->gfl->exe, first);
        }break;
        
        case LDS : {
            char* first = operation->arg1.data.string;
            first = getStringPointer(vm->sdb, first);
            pushExeString (vm->gfl->exe, first);
        }break;
        
        case LOD : {
            int first = operation->arg1.data.integer;
            int second = operation->arg2;
            LODf (vm->gfl, first, second);
        }break;
        
        case STO : {
            int first = operation->arg1.data.integer;
            int second = operation->arg2;
            STOf (vm->gfl, first, second);
        }break;
        
        case JMF : {
            int first = operation->arg1.data.integer;
            int condition = pullExeInt(vm->gfl->exe);
            if (!condition)
                jumpByOffset(vm->list, first);
        }break;
        
        case JMP : {
            int first = operation->arg1.data.integer;
            jumpByOffset(vm->list, first);
        }break;
        
        case EQU : {
            EQUf (vm->gfl->exe);
        }break;
        
        case NEQ : {
            NEQf (vm->gfl->exe);
        }break;
        
        case CGT : {
            CGTf (vm->gfl->exe);
        }break;
        
        case CGE : {
            CGEf (vm->gfl->exe);
        }break;
        
        case CLT : {
            CLTf (vm->gfl->exe);
        }break;
        
        case CLE : {
            CLEf (vm->gfl->exe);
        }break;
        
        case IGT : {
            IGTf (vm->gfl->exe);
        }break;
        
        case IGE : {
            IGEf (vm->gfl->exe);
        }break;
        
        case ILT : {
            ILTf (vm->gfl->exe);
        }break;
        
        case ILE : {
            ILEf (vm->gfl->exe);
        }break;
        
        case RGT : {
            RGTf (vm->gfl->exe);
        }break;
        
        case RGE : {
            RGEf (vm->gfl->exe);
        }break;
        
        case RLT : {
            RLTf (vm->gfl->exe);
        }break;
        
        case RLE : {
            RLEf (vm->gfl->exe);
        }break;
        
        case SGT : {
            SGTf (vm->gfl->exe);
        }break;
        
        case SGE : {
            SGEf (vm->gfl->exe);
        }break;
        
        case SLT : {
            SLTf (vm->gfl->exe);
        }break;
        
        case SLE : {
            SLEf (vm->gfl->exe);
        }break;
        
        case IADD : {
            IADDf (vm->gfl->exe);
        }break;
        
        case ISUB : {
            ISUBf (vm->gfl->exe);
        }break;
        
        case IMUL : {
            IMULf (vm->gfl->exe);
        }break;
        
        case IDIV : {
            IDIVf (vm->gfl->exe);
        }break;
        
        case RADD : {
            RADDf (vm->gfl->exe);
        }break;
        
        case RSUB : {
            RSUBf (vm->gfl->exe);
        }break;
        
        case RMUL : {
            RMULf (vm->gfl->exe);
        }break;
        
        case RDIV : {
            RDIVf (vm->gfl->exe);
        }break;
        
        case IUMI : {
            IUMIf (vm->gfl->exe);
        }break;
        
        case RUMI : {
            RUMIf (vm->gfl->exe);
        }break;
        
        case LNEG : {
            LNEGf (vm->gfl->exe);
        }break;
        
        case PUSH : {
            int first = operation->arg1.data.integer;
            int second = operation->arg2;
            int third = operation->arg3;
            if (third == -1 && vm->parsLeft > 0)
                printf("WARNING: Parametri non utilizzati\n");
            int returnIndex = vm->list->currentOperation + 1; // la current operation è già la successiva (siamo al pop, quello giusto)
            PUSHf (vm->gfl, first, second, third, returnIndex);
        }break;
        
        case GOTO : {
            int first = operation->arg1.data.integer;
            jumpToIndex(vm->list, first);
        }break;
        
        case POP : {
            vm->gfl->currentAR--;
        }break;
        
        case TOINT : {
            TOINTf (vm->gfl->exe);
        }break;
        
        case TOREA : {
            TOREALf (vm->gfl->exe);
        }break;
        
        case READ : {
            char first = operation->arg1.data.charap;
            int second = operation->arg2;
            int third = operation->arg3;
            if ( second == -1 )
                READfromArgs(vm, first);
            else
                READf (vm->sdb, vm->gfl, first, second, third);
        }break;
        
        case WRITE : {
            char* first = operation->arg1.data.string;
            WRITEf(vm->gfl->exe, first);
        }break;
        
        case MOD : {
            //there is nothing to do here, luckly :D
        }break;
        
        case RETURN : {
            int index = RETURNf(vm->gfl);
            jumpToIndex(vm->list, index);
        }break;
        
        case HALT : {
            printf("\n--Execution completed--\n");
            exit(0);
        }
        
    }
}

int main(int args, char** argv) {
    if(args < 2) {
        printf("ERROR: necessario specificare il nome del file sorgente .tcode");
        exit(1);
    }
    char* codePath = calloc(50,1);
    strcpy(codePath, argv[1]);
    yyin = fopen(codePath, "r");
    out = fopen("code.code", "w");
    yyout = out;
    yylex();
    fclose(out);
    
    printf("TelaVM versione 3.25.07\n");
    VirtualMachine* vm = malloc(sizeof(VirtualMachine));
    *vm = newVM(); 
    vm->params = argv+2;
    vm->parsLeft = args-2;
    initList(vm->list, "code.code");
    //debugOp(vm->list);
    printf("--Execution started--\n\n");
    
    while(vm->list->currentOperation < vm->list->rows) {
        Op instruction= getNextOperation(vm->list);
        execute (vm, &instruction);
    } 
    
    
    return 0; 
}



    /*int argc, char **argv*/
    /*
    char* codePath = malloc(50);
    strcpy(codePath, "code.code");
    if (argc == 2)
        strcpy(codePath, argv[1]);
    if (argc > 2)
        {
            printf("Too many parameters for this vmachine\nexecution stopped");
            exit(1);
        }
    */