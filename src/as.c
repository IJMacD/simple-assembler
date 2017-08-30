#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "ben.h"

#define DEFAULT_OUTFILE "out.bin"

char parseHexDigit(char digit) {
  if (isdigit(digit)) return digit - '0';
  if (digit >= 'A' && digit <= 'F') return 10 + digit - 'A';
  if (digit >= 'a' && digit <= 'f') return 10 + digit - 'a';
  return 0;
}

char decodeLine(char *mne, char hex) {

  if (!strcmp(mne, "NOP")) return NOP;

  if (!strcmp(mne, "LDA")) return LDA | (hex & 0xF);

  if (!strcmp(mne, "ADD")) return ADD | (hex & 0xF);

  if (!strcmp(mne, "SUB")) return SUB | (hex & 0xF);

  if (!strcmp(mne, "STA")) return STA | (hex & 0xF);

  if (!strcmp(mne, "LDI")) return LDI | (hex & 0xF);

  if (!strcmp(mne, "JMP")) return JMP | (hex & 0xF);

  if (!strcmp(mne, "OUT")) return OPT;

  if (!strcmp(mne, "HLT")) return HLT;
  
  // Need to return something as there are no exceptions
  return NOP;
}

void assemble(FILE *in, FILE *out) {
  char mne[4] = { 0 };
  char hex = 0;
  while(fscanf(in, "%3s %2hhXH", mne, &hex) != EOF) {
    char byte = decodeLine(mne, hex);
    fputc(byte, out);
  }
}

int main(int argc, char *argv[]) {

  if(argc < 2) {
    fprintf(stderr, "Usage: as infile [outfile]\n");
    exit(-1);
  }

  FILE *input = fopen(argv[1], "r");

  if (input == NULL) {
    fprintf(stderr, "Error opening input file \"%s\"\n", argv[1]);
    exit(-1);
  }

  char *outfile = (argc < 3) ? DEFAULT_OUTFILE : argv[2];

  FILE *output = fopen(outfile, "w");

  if (output == NULL) {
    fprintf(stderr, "Error opening output file \"%s\"\n", outfile);
    exit(-1);
  }

  assemble(input, output);
}
