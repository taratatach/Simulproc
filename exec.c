#include <stdio.h>
#include "error.h"
#include "exec.h"

/*!
* \file exec.c
* \brief Execute une instruction.
*/



//! Met à jour le code condition
/*!
* \param pmach la machine/programme en cours d'exécution
* \param res dernier résultat obtenu
*/
void update_cc(Machine * pmach, int res)
{
    if (res < 0)
	pmach->_cc = CC_N;

    else if (res == 0)
	pmach->_cc = CC_Z;

    else
	pmach->_cc = CC_P;
}

//! Calcule l'adresse d'une instruction en mode indexé ou absolu
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr instruction à exécuter
* \return adresse indexée si mode indexé, sinon absolue
*/
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

//! Vérifie si l'instruction est en mode immédiat
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
*/
void check_error_immediate(Machine * pmach, Instruction instr)
{
    if (instr.instr_generic._immediate)
    {
	error(ERR_IMMEDIATE, pmach->_pc - 1);
	free(pmach->_text);
	free(pmach->_data);
    }
}

//! Vérifie si l'on veut sortir de la pile ou non
/*!
* \param pmach la machine/programme en cours d'exécution
*/
void check_error_segstack(Machine * pmach)
{
    if (pmach->_sp < pmach->_dataend || pmach->_sp >= pmach->_datasize)
    {
	error(ERR_SEGSTACK, pmach->_pc - 1);
	free(pmach->_text);
	free(pmach->_data);
    }
}

/*!
* Vérifie si l'on essaaie d'accèder à une donnée en dehors
* du segment de données ou non
*/
/*!
* \param pmach la machine/programme en cours d'exécution
* \param addr l'adresse de la donnée à laquelle on veut accèder
*/
void check_error_segdata(Machine * pmach, unsigned addr)
{
    if (addr >= pmach->_datasize)
    {
	error(ERR_SEGDATA, pmach->_pc - 1);
	free(pmach->_text);
	free(pmach->_data);
    }
}


void check_error_segtext(Machine * pmach)
{
    if (pmach->_pc >= pmach->_textsize)
    {
	error(ERR_SEGTEXT, pmach->_pc - 1);
	free(pmach->_text);
	free(pmach->_data);
    }
}

//! Effectue un ILLOP sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return aucun
*/
bool illop(Machine * pmach, Instruction instr)
{
    error(ERR_ILLEGAL, pmach->_pc - 1);
    free(pmach->_text);
    free(pmach->_data);
}

//! Effectue un NOP sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true
*/
bool nop(Machine * pmach, Instruction instr)
{
    return true;
}

//! Effectue un LOAD sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool load(Machine * pmach, Instruction instr)
{


    if (instr.instr_generic._immediate) // I=1
    {
	pmach->_registers[instr.instr_generic._regcond] =
	    instr.instr_immediate._value;
    }

    else // I=0
    {
      	unsigned addr = address(pmach, instr);
	check_error_segdata(pmach, addr);
	pmach->_registers[instr.instr_generic._regcond] =
	    pmach->_data[addr];	
    }

    update_cc(pmach, pmach->_registers[instr.instr_generic._regcond]);
    return true;

}

//! Effectue un STORE sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool store(Machine * pmach, Instruction instr)
{
    //source registre
    //addressage absolu et indexé
    check_error_immediate(pmach, instr); 

    unsigned addr = address(pmach, instr);

    check_error_segdata(pmach, addr);
    pmach->_data[addr] = pmach->_registers[instr.instr_generic._regcond];

    return true;
}

//! Effectue un ADD sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool add(Machine * pmach, Instruction instr)
{

    if (instr.instr_generic._immediate) // I=1
    {
      	pmach->_registers[instr.instr_generic._regcond] +=
	    instr.instr_immediate._value;
	
    }
    else // I=0
    {
        unsigned addr = address(pmach, instr);
        check_error_segdata(pmach, addr);
	pmach->_registers[instr.instr_generic._regcond] +=
	    pmach->_data[addr];
    }

    update_cc(pmach, pmach->_registers[instr.instr_generic._regcond]);
    return true;
}

//! Effectue un SUB sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool sub(Machine * pmach, Instruction instr)
{

    if (instr.instr_generic._immediate) // I=1
    {
	pmach->_registers[instr.instr_generic._regcond] -=
	    instr.instr_immediate._value;      
    } 
    else // I=0
    {
	unsigned addr = address(pmach, instr);
	check_error_segdata(pmach, addr);
	pmach->_registers[instr.instr_generic._regcond] -=
	    pmach->_data[addr];
    }

    update_cc(pmach, pmach->_registers[instr.instr_generic._regcond]);
    return true;
}

//! Vérifie si la condition nous autorise à effectuer un jump
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si on doit effectuer un jump
*/
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
    case LE:			//Négatif ou nul
	return (pmach->_cc == CC_N || pmach->_cc == CC_Z);
    default:			//Condition inconnue
	error(ERR_CONDITION, pmach->_pc -1);
	free(pmach->_text);
	free(pmach->_data);
    }
}

//! Effectue un BRANCH sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool branch(Machine * pmach, Instruction instr)
{
    //addressage absolu et indexé
    check_error_immediate(pmach, instr);

    if (jump_allowed(pmach, instr))
    {
	pmach->_pc = address(pmach, instr);
    }

    return true;
}

//! Effectue un CALL sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool call(Machine * pmach, Instruction instr)
{
    //addressage absolu et indexé
    check_error_immediate(pmach, instr);

    if (jump_allowed(pmach, instr))
    {
	pmach->_data[pmach->_sp--] = pmach->_pc;
	pmach->_pc = address(pmach, instr);
    }

    return true;
}

//! Effectue un RET sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool ret(Machine * pmach, Instruction instr)
{
    ++pmach->_sp;
    check_error_segstack(pmach);
    pmach->_pc = pmach->_data[pmach->_sp];
    return true;
}

//! Effectue un PUSH sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool push(Machine * pmach, Instruction instr)
{
    check_error_segstack(pmach);

    if (instr.instr_generic._immediate) // I=1
    {
	pmach->_data[pmach->_sp] = instr.instr_immediate._value;
    }

    else // I=0
    {
	unsigned addr = address(pmach, instr);
	check_error_segdata(pmach, addr);
	pmach->_data[pmach->_sp] = pmach->_data[addr];
    }
    --pmach->_sp;
    return true;
}

//! Effectue un POP sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return true si aucune erreur
*/
bool pop(Machine * pmach, Instruction instr)
{
    //addressage absolu et indexé
    ++pmach->_sp;
    check_error_segstack(pmach);
    check_error_immediate(pmach, instr);

    unsigned addr = address(pmach, instr);

    if (addr < pmach->_datasize)
	pmach->_data[addr] = pmach->_data[pmach->_sp];

    return true;
}

//! Effectue un HALT sur la machine
/*!
* \param pmach la machine/programme en cours d'exécution
* \param instr l'instruction à exécuter
* \return false
*/
bool halt(Machine * pmach, Instruction instr)
{
    warning(WARN_HALT, pmach->_pc - 1);
    return false;
}



bool decode_execute(Machine * pmach, Instruction instr)
{
  
  
    
    unsigned addr = address(pmach, instr);
    check_error_segtext(pmach);
    
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



void trace(const char *msg, Machine * pmach, Instruction instr, unsigned addr)
{
    printf("TRACE: %s: 0x%04x: ", msg, addr);
    print_instruction(instr, addr);
    printf("\n");
}
