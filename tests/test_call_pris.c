#include "machine.h"


Instruction text[] = {
  {.instr_absolute = {LOAD, false, false, 0, 0}},    //0
  {.instr_absolute = {LOAD, false, false, 1, 1}},    //1
  {.instr_immediate = {SUB, true, false, 1, 7}},     //2
  {.instr_absolute = {CALL, false, false, LE, 14}}, //3
  {.instr_absolute = {CALL, false, false, LT, 14}}, //4
  {.instr_immediate = {ADD, true, false, 1, 7}},     //5
  {.instr_absolute = {CALL, false, false, GE, 14}}, //6
  {.instr_absolute = {CALL, false, false, GT, 14}}, //7
  {.instr_immediate = {SUB, true, false, 1, 5}},     //8
  {.instr_absolute = {CALL, false, false, EQ, 14}}, //9
  {.instr_immediate = {ADD, true, false, 1, 1}},     //10
  {.instr_absolute = {CALL, false, false, NE, 14}}, //11
  {.instr_absolute = {CALL, false, false, NC, 14}}, //12
  {.instr_generic = {HALT,                   }},     //13
  {.instr_immediate = {ADD, true, false, 2, 1}},     //14
  {.instr_absolute = {RET,                   }}      //15
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
