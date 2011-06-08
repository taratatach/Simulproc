#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "machine.h"
#include "debug.h"
#include "exec.h"

void load_program(Machine *pmach,
		  unsigned textsize, Instruction text[textsize],
		  unsigned datasize, Word data[datasize], unsigned dataend) {
  (dataend <= (datasize - MINSTACKSIZE)) ? pmach->_dataend = dataend : exit(1); //TODO : erreur à afficher
  pmach->_text = text;
  pmach->_textsize = textsize;
  pmach->_data = data;
  pmach->_datasize = datasize;
  pmach->_pc = 0;
  pmach->_cc = CC_U;

  for(unsigned i=0; i < NREGISTERS-1; i++) pmach->_registers[i] = 0;
  pmach->_sp = datasize - 1;
}


void read_program(Machine *mach, const char *programfile) {
  Word *data;
  Instruction *text;
  unsigned textsize, datasize, dataend;
  int fd = open(programfile, O_RDONLY);

  read(fd, &textsize, 4);
  read(fd, &datasize, 4);
  read(fd, &dataend, 4);

  text = malloc(sizeof(Instruction)*textsize);
  for(unsigned i=0; i < textsize; i++) read(fd, text + i, 4);
  data = malloc(sizeof(Word)*datasize);
  for(unsigned i=0; i < dataend; i++) read(fd, data + i, 4);

  close(fd);

  load_program(mach, textsize, text, datasize, data, dataend);
}

void dump_memory(Machine *pmach) {
  int fd = open("dump.bin", O_WRONLY|O_CREAT|O_TRUNC, 0644);
  
  write(fd, &(pmach->_textsize), 4);
  write(fd, &(pmach->_datasize), 4);
  write(fd, &(pmach->_dataend), 4);

  printf("Instruction text[] {\n");
  for(unsigned i=0; i < pmach->_textsize; i++) {
    write(fd, &(pmach->_text[i]), 4);

    if (i%4 == 0) printf("    ");
    printf("0x%08x", (pmach->_text[i])._raw);
    if (i < pmach->_textsize - 1) printf(", ");
    if (i%4 == 3 || i==pmach->_textsize - 1) printf("\n");
  }

  printf("};\nunsigned textsize = %d;\n\n", pmach->_textsize);

  printf("Word data[] {\n");
  for(unsigned i=0; i < pmach->_dataend; i++) {
    write(fd, &(pmach->_data[i]), 4);
    
    if (i%4 == 0) printf("    ");
    printf("0x%08x", pmach->_data[i]);
    if (i < pmach->_dataend - 1) printf(", ");
    if (i%4 == 3 || i==pmach->_dataend - 1) printf("\n");
  }
  printf("};\nunsigned datasize = %d;\n", pmach->_datasize);
  printf("unsigned dataend = %d;\n", pmach->_dataend);
}


void print_program(Machine *pmach) {
  printf("\n*** PROGRAM (size: %d) ***\n", pmach->_textsize);
  for(unsigned i = 0; i < pmach->_textsize; i++) {
    printf("0x%04x: 0x%08x \t ", i, pmach->_text[i]._raw);
    print_instruction(pmach->_text[i], i);
    printf("\n");
  }
}

void print_data(Machine *pmach) {
  printf("\n\n*** DATA (size: %d, end = 0x%08x (%d)) ***\n", pmach->_datasize, pmach->_dataend, pmach->_dataend);
  for(unsigned i = 0; i < pmach->_datasize; i++) {
    printf("0x%04x: 0x%08x %d\t", i, pmach->_data[i], pmach->_data[i]);
    if (i%3 == 2 || i == pmach->_datasize - 1) printf("\n");
  }
}

void print_cpu(Machine *pmach) {
  printf("\n\n*** CPU ***\n");
  printf("PC:  0x%08x   CC: %c\n\n", pmach->_pc,
	 (pmach->_cc == CC_U) ? 'U' : (pmach->_cc == CC_Z) ? 'Z' : (pmach->_cc == CC_P) ? 'P' : 'N');
  for(unsigned i = 0; i < NREGISTERS; i++) {
    printf("R%02d: 0x%08x %d\t", i, pmach->_registers[i], pmach->_registers[i]);
    if (i%3 == 2 || i == NREGISTERS - 1) printf("\n");
  }
}

void simul(Machine *pmach, bool debug) {
  trace("", pmach, pmach->_text[pmach->_pc], pmach->_pc);
  while(pmach->_pc < pmach->_textsize && decode_execute(pmach, pmach->_text[pmach->_pc++])) {
    if (debug) debug = debug_ask(pmach);
    trace("Executing", pmach, pmach->_text[pmach->_pc], pmach->_pc);
  }
}
