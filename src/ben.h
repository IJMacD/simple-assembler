#include <inttypes.h>

/*****************
 * Control Word
 *****************/
//           |0      7       F0      7       F|
#define HT  0b00000000000000001000000000000000  // Halt clock
#define MI  0b00000000000000000100000000000000  // Memory address register in
#define RI  0b00000000000000000010000000000000  // RAM data in
#define RO  0b00000000000000000001000000000000  // RAM data out
// #define IO  0b00000000000000000000100000000000  // Instruction register out
#define II  0b00000000000000000000010000000000  // Instruction register in
#define AI  0b00000000000000000000001000000000  // A register in
#define AO  0b00000000000000000000000100000000  // A register out
#define EO  0b00000000000000000000000010000000  // ALU out
#define SU  0b00000000000000000000000001000000  // ALU subtract
#define BI  0b00000000000000000000000000100000  // B register in
// #define OI  0b00000000000000000000000000010000  // Output register in
#define CE  0b00000000000000000000000000001000  // Program counter enable
#define PO  0b00000000000000000000000000000100  // Program counter out
#define JP  0b00000000000000000000000000000010  // Jump (program counter in)
#define DR  0b00000000000000000000000000000001  // Decoder reset (T-state)
#define I1  0b00000000000000000000000000000000  // Input 1 in
#define I2  0b00000000000000000000000000000000  // Input 2 in
#define LA  0b00000000000000000000000000000000  // Logical AND
#define LO  0b00000000000000000000000000000000  // Logical OR
#define LX  0b00000000000000000000000000000000  // Logical XOR
#define TI  0b00000000000000000000000000000000  // TMP Register in
#define TO  0b00000000000000000000000000000000  // TMP Register out
#define BO  0b00000000000000000000000000000000  // B register out
#define CI  0b00000000000000000000000000000000  // C register in
#define CO  0b00000000000000000000000000000000  // C register out
#define O3  0b00000000000000000000000000010000  // Output 3 out
#define O4  0b00000000000000000000000000000000  // Output 4 out


/*************
 * ALU Flags
 ************/
#define ALU_FLAG_Z  0b00000000
#define ALU_FLAG_S  0b00000000

/*****************
 * FULL Instructions
 *****************/
#define NOP     0x00  // 0000 0000
#define INR_B   0x04  // 0000 0100  INR | IN_B
#define DCR_B   0x05  // 0000 0101  DCR | IN_B
#define MVI_B   0x06  // 0000 0110  MVI | IN_B
#define INR_C   0x0C  // 0000 1100  INR | IN_C
#define DCR_C   0x0D  // 0000 1101  DCR | IN_C
#define MVI_C   0x0E  // 0000 1110  MVI | IN_C

#define RAL     0x17  // 0001 0111
#define RAR     0x1F  // 0001 1111

#define CMA     0x2F  // 0010 1111

#define STA     0x32  // 0011 0010
#define LDA     0x3A  // 0011 1010  LDX | IN_A
#define INR_A   0x3C  // 0011 1100  INR | IN_A
#define DCR_A   0x3D  // 0011 1101  DCR | IN_A
#define MVI_A   0x3E  // 0011 1110  MVI | IN_A

#define MOV_BC  0x41  // 0100 0001  MOV | OUT_C | IN_B
#define MOV_BA  0x47  // 0100 0111  MOV | OUT_A | IN_B
#define MOV_CB  0x48  // 0100 1000  MOV | OUT_B | IN_C
#define MOV_CA  0x4F  // 0100 1111  MOV | OUT_A | IN_C

#define HLT     0x76  // 0111 0110
#define MOV_AB  0x78  // 0111 1000  MOV | OUT_B | IN_A
#define MOV_AC  0x79  // 0111 1001  MOV | OUT_C | IN_A

#define ADD_B   0x80  // 1000 0000  ADD | OUT_B
#define ADD_C   0x81  // 1000 0001  ADD | OUT_C

#define SUB_B   0x90  // 1001 0000  SUB | OUT_B
#define SUB_C   0x91  // 1001 0001  SUB | OUT_C

#define ANA_B   0xA0  // 1010 0000  ANA | OUT_B
#define ANA_C   0xA1  // 1010 0001  ANA | OUT_C
#define XRA_B   0xA8  // 1010 1000  XRA | OUT_B
#define XRA_C   0xA9  // 1010 1001  XRA | OUT_C

#define ORA_B   0xB0  // 1011 0000  ORA | OUT_B
#define ORA_C   0xB1  // 1011 0001  ORA | OUT_C

#define JNZ     0xC2  // 1100 0010
#define JMP     0xC3  // 1100 0011
#define RET     0xC9  // 1100 1001
#define JZ      0xCA  // 1100 1010
#define CALL    0xCD  // 1100 1101

#define _OUT    0xD3  // 1101 0011    // OUT
#define _IN     0xDB  // 1101 1011    // IN

#define ANI     0xE6  // 1110 0110  ANX | OUT_I
#define XRI     0xEE  // 1110 1110  XRX | OUT_I

#define ORI     0xF6  // 1111 0110  ORX | OUT_I
#define JM      0xFA  // 1111 1010

/*****************************
 * Combinational Instructions
 ****************************/
#define IN_A    0x38  // 0011 1000  // xx 111 xxx
#define IN_B    0x00  // 0000 0000  // xx 000 xxx
#define IN_C    0x08  // 0000 1000  // xx 001 xxx
#define OUT_A   0x07  // 0000 0111  // xx xxx 111
#define OUT_B   0x00  // 0000 0000  // xx xxx 000
#define OUT_C   0x01  // 0000 0001  // xx xxx 001
#define OUT_I   0x06  // 0000 0110  // xx xxx 110

#define LDX     0x02  // 0000 0010  // 00 xxx 010
#define INR     0x04  // 0000 0100  // 00 xxx 100
#define DCR     0x05  // 0000 0101  // 00 xxx 101
#define MVI     0x06  // 0000 0110  // 00 xxx 110

#define MOV     0x40  // 0100 0000  // 01 xxx xxx

#define ADD     0x80  // 1000 0000  // 10 000 xxx
#define SUB     0x90  // 1001 0000  // 10 010 xxx
#define ANA     0xA0  // 1010 0000  // 10 100 xxx
#define XRA     0xA8  // 1010 1000  // 10 101 xxx
#define ORA     0xB0  // 1011 0000  // 10 110 xxx

#define ANX     0xE0  // 1110 0000  // 11 100 xxx
#define XRX     0xE8  // 1110 1000  // 11 101 xxx
#define ORX     0xF0  // 1111 0000  // 11 110 xxx


/*****************
 * Microcode
 *****************/
extern const uint32_t u_instructions[256][16];

extern const char *mne_labels[256];
