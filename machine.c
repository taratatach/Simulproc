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
  int fd = open("dump.prog", O_WRONLY|O_CREAT|O_TRUNC, 0644);
  
  write(fd, &(pmach->_textsize), 4);
  write(fd, &(pmach->_datasize), 4);
  write(fd, &(pmach->_dataend), 4);

  printf("Instruction text[] {\n");
  for(unsigned i=0; i < (pmach->_textsize/4 + pmach->_textsize%4); i++) {
    printf("    ");
    for(unsigned j=0; j < 4; j++) {
      if(i+j < pmach->_textsize) {
	write(fd, &(pmach->_text[4*i+j]), 4);
	printf("0x%08x", (pmach->_text[4*i+j])._raw);
	if (3*i+j != pmach->_textsize) printf(", ");
      }
    }
    printf("\n");
  }
  printf("};\nunsigned textsize = %d;\n\n", pmach->_textsize);

  printf("Word data[] {\n");
  for(unsigned i=0; i < (pmach->_dataend/4 + pmach->_dataend%4); i++) {
    printf("    ");
    for(unsigned j=0; j < 4; j++) {
      if(i+j < pmach->_dataend) {
	write(fd, &(pmach->_data[4*i+j]), 4);
	printf("0x%08x", pmach->_data[4*i+j]);
	if (3*i+j != pmach->_dataend) printf(", ");
      }
    }
    printf("\n");
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
  for(unsigned i = 0; i < pmach->_datasize/3 + pmach->_datasize%3; i++) {
    for(unsigned j = 0; j < 3; j++)
      if (3*i+j < pmach->_datasize) printf("0x%04x: 0x%08x %d\t", 3*i+j, pmach->_data[3*i+j], pmach->_data[3*i+j]);
    printf("\n");
  }
}

void print_cpu(Machine *pmach) {
  printf("\n\n*** CPU ***\n");
  printf("PC:  0x%08x   CC: %c\n\n", pmach->_pc,
	 (pmach->_cc == CC_U) ? 'U' : (pmach->_cc == CC_Z) ? 'Z' : (pmach->_cc == CC_P) ? 'P' : 'N');
  for(unsigned i = 0; i < NREGISTERS/3 + NREGISTERS%3; i++) {
    for(unsigned j = 0; j < 3; j++)
      if (3*i+j < NREGISTERS) printf("R%02d: 0x%08x %d\t", 3*i+j, pmach->_registers[3*i+j], pmach->_registers[3*i+j]);
    printf("\n");
  }
}

void simul(Machine *pmach, bool debug) {
  if (debug) {
    while(pmach->_pc < pmach->_textsize && decode_execute(pmach, pmach->_text[pmach->_pc]))
      if (!debug_ask(pmach)) { debug = false; break; }
  }
  if (!debug) {
    while (pmach->_pc < pmach->_textsize && decode_execute(pmach, pmach->_text[pmach->_pc])) {
      printf("0x%08x\n", pmach->_pc);
    }
  }
}
