Simple Assembler
================

An increadibly simple assembler for a simple SAP-1 assembly language to compliment http://github.com/IJMacD/simple-vm.

Usage
-----
```
as infile [outfile]
```

Example asm file:

```
LDI 01H
STA 0FH
LDA 0FH
OUT
STA 0DH
ADD 0EH
STA 0FH
LDA 0DH
STA 0EH
JMP 02H
```

Command to compile:

```
as fib.asm fib.o
```
