#include <stdlib.h>
#include <stdio.h>
#include "instruction.h"

const char *cop_names[] = {"ILLOP", "NOP", "LOAD", "STORE", "ADD", "SUB", "BRANCH", "CALL", "RET", "PUSH", "POP", "HALT"};
const char *condition_names[] = {"NC", "EQ", "NE", "GT", "GE", "LT", "LE"};

void print_instruction(Instruction instr, unsigned addr) {
	Code_Op op = instr.instr_generic._cop;
	printf("%s ", cop_names[op]); 

	if (op == LOAD || op == STORE || op == ADD || op == SUB) {
		printf("R%02x, ", instr.instr_generic._regcond);
	}
	else if (op == BRANCH || op == CALL) {
		printf("%s, ", condition_names[instr.instr_generic._regcond]);
	}
	if (op != ILLOP && op != NOP && op != RET && op != HALT) {
		if (instr.instr_generic._immediate == 1) {
			printf("#%d", instr.instr_absolute._address);
		}
		else {
			if (instr.instr_generic._indexed == 0) {
				printf("@0x%04x", instr.instr_immediate._value);
			}
			else if (instr.instr_generic._indexed == 1) {
				printf("%d[R%02d]", instr.instr_indexed._offset, instr.instr_indexed._rindex);
			}
		}
	}
}
