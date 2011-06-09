#include "machine.h"


Instruction text[] = {
  {.instr_absolute = {LOAD, false, false, 0, 0}},    //0
  {.instr_absolute = {LOAD, false, false, 1, 1}},    //1
  {.instr_immediate = {SUB, true, false, 1, 7}},     //2
  {.instr_absolute = {BRANCH, false, false, LE, 5}}, //3
  {.instr_generic = {ILLOP,                  }},     //4
  {.instr_absolute = {BRANCH, false, false, LT, 7}}, //5
  {.instr_generic = {ILLOP,                  }},     //6
  {.instr_immediate = {ADD, true, false, 1, 7}},     //7
  {.instr_absolute = {BRANCH, false, false, GE, 10}}, //8
  {.instr_generic = {ILLOP,                  }},     //9
  {.instr_absolute = {BRANCH, false, false, GT, 12}}, //10
  {.instr_generic = {ILLOP,                  }},     //11
  {.instr_immediate = {SUB, true, false, 1, 5}},     //12
  {.instr_absolute = {BRANCH, false, false, EQ, 15}}, //13
  {.instr_generic = {ILLOP,                  }},     //14
  {.instr_immediate = {ADD, true, false, 1, 1}},     //15
  {.instr_absolute = {BRANCH, false, false, NE, 18}}, //16
  {.instr_generic = {ILLOP,                  }},     //17
  {.instr_absolute = {BRANCH, false, false, NC, 20}}, //18
  {.instr_generic = {ILLOP,                  }},     //19
  {.instr_generic = {HALT,                   }}     //20
};

const unsigned textsize = sizeof(text) / sizeof(Instruction);

Word data[20] = {
  10,
  5,
  20,
  0
};

const unsigned dataend = 5;

const unsigned datasize = sizeof(data) / sizeof(Word);
