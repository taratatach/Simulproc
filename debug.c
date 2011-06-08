#include <stdio.h>
#include "instruction.h"
#include "machine.h"
#include "exec.h"
#include "debug.h"

bool debug_ask(Machine *pmach){

    //le tableau de char de réponse
    char tab[101] = {0};
    char c;
    //on demande de taper quelque chose
    int ok = 0; //indique si on a besoin de redemander de taper qqch
    do{
        printf("DEBUG? ");
        scanf("%100s", tab);
        //help
        if(strcmp(tab, "h")==0){
            printf("Available commands:\n");
            printf("\th\thelp\n");
            printf("\tc\tcontinue (exit interactive debug mode)\n");
            printf("\ts\tstep by step (next instruction)\n");
            printf("\tRET\tstep by step (next instruction)\n");
            printf("\tr\tprint registers\n");
            printf("\td\tprint data memory\n");
            printf("\tt\tprint text (program) memory\n");
            printf("\tp\tprint text (program) memory\n");
            printf("\tm\tprint registers and data memory\n");
        }
        //continue (exit interactive debug mode)
        else if(strcmp(tab, "c")==0){
            return false;
        }
        //step by step (next instruction)
        else if(strcmp(tab, "s")==0 || strcmp(tab, "")==0){
            return true;
        }
        //print registers
        else if(strcmp(tab, "r")==0){
            print_cpu(pmach);
        }
        //print data memory
        else if(strcmp(tab, "d")==0){
            print_data(pmach);
        }
        //print text (program) memory
        else if(strcmp(tab, "t")==0 || strcmp(tab, "p")==0){
            print_program(pmach);
        }
        //print registers and data memory
        else if(strcmp(tab, "m")==0){
            print_cpu(pmach);
            print_data(pmach);
        }
    }while(!ok);
    
}
