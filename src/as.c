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

char decodeLine(char *nme, char hex) {

  if (!strcmp(nme, "NOP")) return NOP;

  if (!strcmp(nme, "LDA")) return LDA | (hex & 0xF);

  if (!strcmp(nme, "ADD")) return ADD | (hex & 0xF);

  if (!strcmp(nme, "SUB")) return SUB | (hex & 0xF);

  if (!strcmp(nme, "STA")) return STA | (hex & 0xF);

  if (!strcmp(nme, "LDI")) return LDI | (hex & 0xF);

  if (!strcmp(nme, "JMP")) return JMP | (hex & 0xF);

  if (!strcmp(nme, "OUT")) return OPT;

  if (!strcmp(nme, "HLT")) return HLT;

}

void assemble(FILE *in, FILE *out) {
  char nme[4] = { 0 };
  char hex = 0;
  while(fscanf(in, "%3s %2hhXH", nme, &hex) != EOF) {
    char byte = decodeLine(nme, hex);
    if (byte) {
      fputc(byte, out);
    }
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
