#include "debug.h"

bool debug_ask(Machine *pmach){

    //le tableau de char de réponse
    char tab[101] = {0};
    //on demande de taper quelque chose
    int ok = 0; //indique si on a besoin de redemander de taper qqch
    do{
        printf("DEBUG? ");
        scanf("%100s", tab);
        //help
        if(strcmp(tab, "h")==0){
            return false;
        }
        //continue (exit interactive debug mode)
        else if(strcmp(tab, "c")==0){
            return true;
        }
        //step by step (next instruction)
        else if(strcmp(tab, "s")==0 || strcmp(tab, "RET")==0 || strcmp(tab, "\n")==0){
            return false;
        }
        //print registers
        else if(strcmp(tab, "r")==0){
            ok=1;
        }
        //print data memory
        else if(strcmp(tab, "d")==0){
            ok=1;
        }
        //print text (program) memory
        else if(strcmp(tab, "t")==0 || strcmp(tab, "p")==0){
            ok=1;
        }
        //print registers and data memory
        else if(strcmp(tab, "m")==0){
            ok=1;
        }
    }while(!ok);
    
}
