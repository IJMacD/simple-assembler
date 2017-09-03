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

int writeMne(char mne, FILE *out) {
  fputc(mne, out);
  return 1;
}
int writeMneImmediate(char mne, char hex, FILE *out) {
  fputc(mne, out);
  fputc(hex, out);
  return 2;
}
int writeMneAddress(char mne, short hex, FILE *out) {
  fputc(mne, out);
  fputc(hex & 0xFF, out);
  fputc(hex >> 8, out);
  return 3;
}

int decodeLine(char *mne, char *mod, short hex, FILE *out) {

  if (!strcmp(mne, "NOP")) return fputc(NOP, out);

  if (!strcmp(mne, "LDA")) return writeMneAddress(LDA, hex, out);

  if (!strcmp(mne, "ADD")){
    if(mod[0] == 'B') return fputc(ADD_B, out);
    if(mod[0] == 'C') return fputc(ADD_C, out);
    return 0;
  }

  if (!strcmp(mne, "SUB")) {
    if(mod[0] == 'B') return fputc(SUB_B, out);
    if(mod[0] == 'C') return fputc(SUB_C, out);
    return 0;
  }

  if (!strcmp(mne, "INR")) {
    if(mod[0] == 'A') return fputc(INR_A, out);
    if(mod[0] == 'B') return fputc(INR_B, out);
    if(mod[0] == 'C') return fputc(INR_C, out);
    return 0;
  }

  if (!strcmp(mne, "DCR")) {
    if(mod[0] == 'A') return fputc(DCR_A, out);
    if(mod[0] == 'B') return fputc(DCR_B, out);
    if(mod[0] == 'C') return fputc(DCR_C, out);
    return 0;
  }

  if (!strcmp(mne, "CMA")) return fputc(CMA, out);

  if (!strcmp(mne, "RAL")) return fputc(RAL, out);

  if (!strcmp(mne, "RAR")) return fputc(RAR, out);

  if (!strcmp(mne, "ANA")) {
    if(mod[0] == 'B') return fputc(ANA_B, out);
    if(mod[0] == 'C') return fputc(ANA_C, out);
    return 0;
  }

  if (!strcmp(mne, "XRA")) {
    if(mod[0] == 'B') return fputc(XRA_B, out);
    if(mod[0] == 'C') return fputc(XRA_C, out);
    return 0;
  }

  if (!strcmp(mne, "ORA")) {
    if(mod[0] == 'B') return fputc(ORA_B, out);
    if(mod[0] == 'C') return fputc(ORA_C, out);
    return 0;
  }

  if (!strcmp(mne, "ANI")) return writeMneImmediate(ANI, hex, out);
  if (!strcmp(mne, "XRI")) return writeMneImmediate(XRI, hex, out);
  if (!strcmp(mne, "ORI")) return writeMneImmediate(ORI, hex, out);

  if (!strcmp(mne, "STA")) return writeMneAddress(STA, hex, out);

  if (!strcmp(mne, "MVI")) {
    if(mod[0] == 'A') return writeMneImmediate(MVI_A, hex, out);
    if(mod[0] == 'B') return writeMneImmediate(MVI_B, hex, out);
    if(mod[0] == 'C') return writeMneImmediate(MVI_C, hex, out);
    return 0;
  }

  if (!strcmp(mne, "MOV")) {
    if(!strcmp(mod, "A,B")) return fputc(MOV_AB, out);
    if(!strcmp(mod, "A,C")) return fputc(MOV_AC, out);
    if(!strcmp(mod, "B,A")) return fputc(MOV_BA, out);
    if(!strcmp(mod, "B,C")) return fputc(MOV_BC, out);
    if(!strcmp(mod, "C,A")) return fputc(MOV_CA, out);
    if(!strcmp(mod, "C,B")) return fputc(MOV_CB, out);
    return 0;
  }

  if (!strcmp(mne, "JMP")) return writeMneAddress(JMP, hex, out);

  if (!strcmp(mne, "JZ")) return writeMneAddress(_JZ, hex, out);

  if (!strcmp(mne, "JNZ")) return writeMneAddress(JNZ, hex, out);

  if (!strcmp(mne, "OUT")) return writeMneImmediate(_OUT, hex, out);

  if (!strcmp(mne, "HLT")) return fputc(HLT, out);

  fprintf(stderr, "Error: Mnemonic not recognised: \"%s\"\n", mne);
  return 0;
}

void assemble(FILE *in, FILE *out) {
  char line[255];
  char mne[4] = { 0 };
  char mod[4] = { 0 };
  short hex = 0;
  int scan = 0;
  int line_no = 0;
  while(fgets(line, 255, in) != NULL) {
    line_no++;
    scan = sscanf(line, "%3s %[ABC],%2hXH", mne, mod, &hex);
    if(scan == 3) {
      decodeLine(mne, mod, hex, out);
      continue;
    }
    scan = sscanf(line, "%3s %4hXH", mne, &hex);
    if(scan == 2) {
      decodeLine(mne, mod, hex, out);
      continue;
    }
    scan = sscanf(line, "%3s %[ABC,]", mne, mod);
    if(scan > 0) {
      decodeLine(mne, mod, hex, out);
      continue;
    }
    fprintf(stderr, "Error: unrecognised line (%d): \"%s\"\n", line_no, line);
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

  FILE *output = fopen(outfile, "wb");

  if (output == NULL) {
    fprintf(stderr, "Error opening output file \"%s\"\n", outfile);
    exit(-1);
  }

  assemble(input, output);
}
