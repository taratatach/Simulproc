#include "machine.h"


Instruction text[] = {
  {.instr_absolute = {BRANCH, false, false, NC, 20}},
  {.instr_absolute = {HALT,                   }}
};

const unsigned textsize = sizeof(text) / sizeof(Instruction);

Word data[20] = {
  10,
  5,
  20,
  0
};

const unsigned dataend = 10;

const unsigned datasize = sizeof(data) / sizeof(Word);
