#include <stdio.h>
#include "error.h"
#include "exec.h"


void update_cc(Machine * pmach, int res)
{
    if (res < 0)
	pmach->_cc = CC_N;

    else if (res == 0)
	pmach->_cc = CC_Z;

    else
	pmach->_cc = CC_P;
}

unsigned address(Machine * pmach, Instruction instr)
{
    if (instr.instr_generic._indexed)
    {
	return pmach->_registers[instr.instr_indexed._rindex] +
	    instr.instr_indexed._offset;
    }
    else
    {
	return instr.instr_absolute._address;
    }
}

void error_immediate(Machine * pmach, Instruction instr)
{
    if (instr.instr_generic._immediate)
    {
	free(pmach->_text);
	free(pmach->_data);
	error(ERR_IMMEDIATE, pmach->_pc - 1);
    }
}


void error_segstack(Machine * pmach)
{
    if (pmach->_sp < 0 || pmach->_sp >= pmach->_datasize)
    {
	free(pmach->_text);
	free(pmach->_data);
	error(ERR_SEGSTACK, pmach->_pc - 1);
    }
}



void error_segdata(Machine * pmach, unsigned addr)
{
    if (addr >= pmach->_datasize)
    {
	free(pmach->_text);
	free(pmach->_data);
	error(ERR_SEGDATA, pmach->_pc - 1);
    }
}




bool illop(Machine * pmach, Instruction instr)
{
    free(pmach->_text);
    free(pmach->_data);
    error(ERR_ILLEGAL, pmach->_pc - 1);
}


bool nop(Machine * pmach, Instruction instr)
{
    return true;
}


bool load(Machine * pmach, Instruction instr)
{



    if (!instr.instr_generic._immediate)
    {
	unsigned addr = address(pmach, instr);
	error_segdata(pmach, addr);
	pmach->_registers[instr.instr_generic._regcond] =
	    pmach->_data[addr];
    }

    else
    {
	pmach->_registers[instr.instr_generic._regcond] =
	    instr.instr_immediate._value;
    }

    update_cc(pmach, pmach->_registers[instr.instr_generic._regcond]);
    return true;

}

bool store(Machine * pmach, Instruction instr)
{

    error_immediate(pmach, instr);

    unsigned addr = address(pmach, instr);

    error_segdata(pmach, addr);
    pmach->_data[addr] = pmach->_registers[instr.instr_generic._regcond];

    return true;
}

bool add(Machine * pmach, Instruction instr)
{

    if (!instr.instr_generic._immediate)
    {
	unsigned addr = address(pmach, instr);
	error_segdata(pmach, addr);

	pmach->_registers[instr.instr_generic._regcond] +=
	    pmach->_data[addr];
    }
    else
    {
	pmach->_registers[instr.instr_generic._regcond] +=
	    instr.instr_immediate._value;
    }

    update_cc(pmach, pmach->_registers[instr.instr_generic._regcond]);
    return true;
}


bool sub(Machine * pmach, Instruction instr)
{

    if (!instr.instr_generic._immediate)
    {
	unsigned addr = address(pmach, instr);
	error_segdata(pmach, addr);

	pmach->_registers[instr.instr_generic._regcond] -=
	    pmach->_data[addr];
    } 
    else
    {
	pmach->_registers[instr.instr_generic._regcond] -=
	    instr.instr_immediate._value;
    }

    update_cc(pmach, pmach->_registers[instr.instr_generic._regcond]);
    return true;
}

bool jump_allowed(Machine * pmach, Instruction instr)
{

    switch (instr.instr_generic._regcond) {

    case NC:			//Pas de condition
	return true;
    case EQ:			//Egal à 0
	return (pmach->_cc == CC_Z);
    case NE:			//Different de zero
	return (pmach->_cc != CC_Z);
    case GT:			//Strictement positif
	return (pmach->_cc == CC_P);
    case GE:			//Positif ou nul
	return (pmach->_cc == CC_P || pmach->_cc == CC_Z);
    case LT:			//Strictement négatif
	return (pmach->_cc == CC_N);
    case LE:
	return (pmach->_cc == CC_N || pmach->_cc == CC_Z);
    default:			//Négatif ou nul
	free(pmach->_text);
	free(pmach->_data);
	error(ERR_CONDITION, pmach->_pc -1);
    }
}


bool branch(Machine * pmach, Instruction instr)
{

    error_immediate(pmach, instr);

    if (jump_allowed(pmach, instr))
    {
	pmach->_pc = address(pmach, instr);
    }

    return true;
}

bool call(Machine * pmach, Instruction instr)
{
    error_immediate(pmach, instr);

    if (jump_allowed(pmach, instr))
    {
	pmach->_data[pmach->_sp--] = pmach->_pc;
	pmach->_pc = address(pmach, instr);
    }

    return true;
}

bool ret(Machine * pmach, Instruction instr)
{
    ++pmach->_sp;
    error_segstack(pmach);
    pmach->_pc = pmach->_data[pmach->_sp];
    return true;
}

bool push(Machine * pmach, Instruction instr)
{
    error_segstack(pmach);

    if (!instr.instr_generic._immediate)
    {
	unsigned addr = address(pmach, instr);
	error_segdata(pmach, addr);
	pmach->_data[pmach->_sp] = pmach->_data[addr];
    }

    else
	pmach->_data[pmach->_sp] = instr.instr_immediate._value;

    --pmach->_sp;
    return true;
}

bool pop(Machine * pmach, Instruction instr)
{
    ++pmach->_sp;
    error_segstack(pmach);
    error_immediate(pmach, instr);

    unsigned addr = address(pmach, instr);

    if (addr < pmach->_datasize)
	pmach->_data[addr] = pmach->_data[pmach->_sp];

    return true;
}

bool halt(Machine * pmach, Instruction instr)
{
    warning(WARN_HALT, pmach->_pc - 1);
    return false;
}





bool decode_execute(Machine * pmach, Instruction instr)
{

    unsigned addr = address(pmach, instr);

    switch (instr.instr_generic._cop)
    {

    case LOAD:
	return load(pmach, instr);
    case STORE:
	return store(pmach, instr);
    case ADD:
	return add(pmach, instr);
    case SUB:
	return sub(pmach, instr);
    case BRANCH:
	return branch(pmach, instr);
    case CALL:
	return call(pmach, instr);
    case RET:
	return ret(pmach, instr);
    case PUSH:
	return push(pmach, instr);
    case POP:
	return pop(pmach, instr);
    case HALT:
	return halt(pmach, instr);
    case NOP:
	return nop(pmach, instr);
    case ILLOP:
	return illop(pmach, instr);
    default:
	error(ERR_UNKNOWN, addr);
    }
}



void
trace(const char *msg, Machine * pmach, Instruction instr, unsigned addr)
{
    printf("TRACE: %s: 0x%04x: ", msg, addr);
    print_instruction(instr, addr);
    printf("\n");
}
