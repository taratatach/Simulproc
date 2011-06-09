#include "machine.h"


Instruction text[] = {
  {.instr_absolute = {LOAD, false, false, 0, 0}},    //0
  {.instr_absolute = {LOAD, false, false, 1, 1}},    //1
  {.instr_immediate = {SUB, true, false, 1, 7}},     //2
  {.instr_absolute = {CALL, false, false, GT, 13}}, //3
  {.instr_absolute = {CALL, false, false, GE, 13}}, //4
  {.instr_immediate = {ADD, true, false, 1, 7}},     //5
  {.instr_absolute = {CALL, false, false, LT, 13}}, //6
  {.instr_absolute = {CALL, false, false, LE, 13}}, //7
  {.instr_immediate = {SUB, true, false, 1, 5}},     //8
  {.instr_absolute = {CALL, false, false, NE, 13}}, //9
  {.instr_immediate = {ADD, true, false, 1, 1}},     //10
  {.instr_absolute = {CALL, false, false, EQ, 13}}, //11
  {.instr_generic = {HALT,                   }},     //12
  {.instr_immediate = {ADD, true, false, 2, 1}},     //13
  {.instr_absolute = {RET,                   }}      //14
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
