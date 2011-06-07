#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void error(Error err, unsigned addr) {
	printf("%d : ", addr);
	switch(err) {
		case ERR_UNKNOWN:
			printf("Instruction inconnue");
			break;	
		case ERR_ILLEGAL:
			printf("Instruction illégale");
			break;
		case ERR_CONDITION:
			printf("Condition illégale");
			break;
		case ERR_IMMEDIATE:
			printf("Valeur immédiate interdite");
			break;
		case ERR_SEGTEXT:
			printf("Violation de taille du segment de texte");
			break;
		case ERR_SEGDATA:
			printf("Violation de taille du segment de données");
			break;
		case ERR_SEGSTACK:
			printf("Violation de taille du segment de pile");
			break;
	}
	fflush(stdout);
	exit(0);
}

void warning(Warning warn, unsigned addr) {
	printf("%d : fin normale du programme\n", addr);
}
