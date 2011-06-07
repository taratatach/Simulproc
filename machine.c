#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "machine.h"

void load_program(Machine *pmach,
		  unsigned textsize, Instruction text[textsize],
		  unsigned datasize, Word data[datasize], unsigned dataend) {
  printfs("a\n");
  (dataend == (datasize - MINSTACKSIZE - 1)) ? pmach->_dataend = dataend : exit(1); //TODO : erreur à afficher
  pmach->_text = text;
  pmach->_textsize = textsize;
  pmach->_data = data;
  pmach->_datasize = datasize;
  pmach->_pc = 0;
  pmach->_cc = CC_U;

  for(int i=0; i < NREGISTERS-1; i++) pmach->_registers[i] = 0;
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
  for(int i=0; i < textsize; i++) read(fd, text + i, 4);
  data = malloc(sizeof(Word)*datasize);
  for(int i=0; i < dataend; i++) read(fd, data + i, 4);

  close(fd);

  load_program(mach, textsize, text, datasize, data, dataend);
}

void dump_memory(Machine *pmach) {
  int fd = open("dump.prog", O_WRONLY|O_CREAT|O_TRUNC, 0644);
  
  write(fd, &(pmach->_textsize), 4);
  write(fd, &(pmach->_datasize), 4);
  write(fd, &(pmach->_dataend), 4);

  printf("*** Sauvegarde des programmes PERSO et données initiales en format binaire ***\n\n");
  printf("Instruction text[] {\n");
  for(int i=0; i < (pmach->_textsize/4 + pmach->_textsize%4); i++) {
    printf("\t");
    for(int j=0; j < 4; j++) {
      if(i+j < pmach->_textsize) {
	write(fd, &(pmach->_text[i+j]), 4);
	printf("%x", (pmach->_text[i+j])._raw);
	if (i+j != pmach->_textsize) printf(", ");
      }
    }
    printf("\n");
  }
  printf("};\nunsigned textsize = %d;\n\n", pmach->_textsize);

  printf("Word data[] {\n");
  for(int i=0; i < (pmach->_dataend/4 + pmach->_dataend%4); i++) {
    printf("\t");
    for(int j=0; j < 4; j++) {
      if(i+j < pmach->_dataend) {
	write(fd, &(pmach->_data[i+j]), 4);
	printf("%x", pmach->_data[i+j]);
	if (i+j != pmach->_dataend) printf(", ");
      }
    }
    printf("\n");
  }
  printf("};\nunsigned datasize = %d;\n", pmach->_datasize);
  printf("\nunsigned dataend = %d;\n", pmach->_dataend);
}

/*
void print_program(Machine *pmach) {

}

void print_data(Machine *pmach) {

}

void print_cpu(Machine *pmach) {

}

void simul(Machine *pmach, bool debug) {

}
*/
