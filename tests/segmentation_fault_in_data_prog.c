/*
 * Segmentation fault in data
*/

#include "machine.h"

Instruction text[] = {
    0x00002f09, 0x00003f09, 0x0000a007, 0x00002f44, 
    0x00002003, 0x0000000b, 0x00000001, 0x00000001, 
    0x00000001, 0x00000001, 0x0003f082, 0x0002f182, 
    0x00001145, 0x00011606, 0x0003f084, 0x00001145, 
    0x0000d006, 0x00000008, 
};
int textsize = 18;

Word data[] = {
    0x00000000, 0x00000000, 0x00000014, 0x00000005, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
int datasize = 2;

const unsigned dataend = 10;
