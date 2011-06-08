#include <stdio.h>
#include <string.h>
#include "instruction.h"
#include "machine.h"
#include "exec.h"
#include "debug.h"

/*
 * Affiche les instructions une par une et demande des 
 * instructions à l'utilisateur sur la suite de l'exécution du programme
 */
bool debug_ask(Machine *pmach){

    //le tableau de char de réponse
    char tab[101] = {0};
    //on demande de taper quelque chose
    while(true){
        printf("DEBUG? ");
        fgets (tab, 100, stdin);
        
        //Si on a appuyé sur Entrée directement
        if(tab[0] == '\n'){
            return true;
        }
        //Sinon on supprime le \n
        else{
            char *c;
            c = strchr(tab, '\n');
            c[0] = 0;
        }
        switch(tab[0]){
            //help
            case 'h': 
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
                break;
            //continue (exit interactive debug mode)
            case 'c':
                return false;
            //step by step (next instruction)
            case 's':
                return true;
            //print registers
            case 'r':
                print_cpu(pmach);
                break;
            //print data memory
            case 'd':
                print_data(pmach);
                break;
            //print text (program) memory
            case 't':
                print_program(pmach);
                break;
            //print text (program) memory
            case 'p':
                print_program(pmach);
                break;
            //print registers and data memory
            case 'm': 
                print_cpu(pmach);
                print_data(pmach);
                break;
        }
    }
    
    return false;
}
