#include <stdlib.h>
#include <stdio.h>
#include "error.h"

//fonction qui affiche le type d'erreur
void error(Error err, unsigned addr) {
	printf("ERROR: ");
	switch(err) {
		case ERR_NOERROR:
			printf("NOERROR Pas d'erreur");
			break;
		case ERR_UNKNOWN:
			printf("UNKNOWN Instruction inconnue");
			break;	
		case ERR_ILLEGAL:
			printf("ILLEGAL Instruction illégale");
			break;
		case ERR_CONDITION:
			printf("CONDITION Condition illégale");
			break;
		case ERR_IMMEDIATE:
			printf("IMMEDIATE Valeur immédiate interdite");
			break;
		case ERR_SEGTEXT:
			printf("SEGTEXT Violation de taille du segment de texte");
			break;
		case ERR_SEGDATA:
			printf("SEGDATA Violation de taille du segment de données");
			break;
		case ERR_SEGSTACK:
			printf("SEGSTACK Violation de taille du segment de pile");
			break;
	}
	printf(": 0x%x\n", addr);
	exit(0);
}

//fonction qui affiche le type d'avertissement
void warning(Warning warn, unsigned addr) {
	if (warn == WARN_HALT) {
		printf("WARNING: HALT fin normale du programme atteinte à l'adresse: 0x%x\n", addr);
	}
}
