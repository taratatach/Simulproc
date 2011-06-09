#include "machine.h"


Instruction text[] = {
  {.instr_absolute = {LOAD, false, false, 0, 0}},     //0
  {.instr_absolute = {LOAD, false, false, 1, 1}},     //1
  {.instr_immediate = {SUB, true, false, 1, 7}},      //2
  {.instr_absolute = {BRANCH, false, false, GT, 5}},  //3
  {.instr_generic = {NOP,                  }},        //4
  {.instr_absolute = {BRANCH, false, false, GE, 7}},  //5
  {.instr_generic = {NOP,                  }},        //6
  {.instr_immediate = {ADD, true, false, 1, 7}},      //7
  {.instr_absolute = {BRANCH, false, false, LT, 10}}, //8
  {.instr_generic = {NOP,                  }},        //9
  {.instr_absolute = {BRANCH, false, false, LE, 12}}, //10
  {.instr_generic = {NOP,                  }},        //11
  {.instr_immediate = {SUB, true, false, 1, 5}},      //12
  {.instr_absolute = {BRANCH, false, false, NE, 15}}, //13
  {.instr_generic = {NOP,                  }},        //14
  {.instr_immediate = {ADD, true, false, 1, 1}},      //15
  {.instr_absolute = {BRANCH, false, false, EQ, 18}}, //16
  {.instr_generic = {NOP,                  }},        //17
  {.instr_generic = {HALT,                   }}       //18
};

const unsigned textsize = sizeof(text) / sizeof(Instruction);

Word data[20] = {
  10,
  5,
  20,
  0
};

const unsigned dataend = 17;

const unsigned datasize = sizeof(data) / sizeof(Word);
