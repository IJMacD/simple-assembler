@echo off
if not exist obj mkdir obj
if not exist bin mkdir bin
cd obj
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
set compilerflags=/Od /Zi /EHsc /DUNICODE
set linkerflags=/OUT:../bin/as.exe
cl.exe %compilerflags% ../src/*.c /link %linkerflags%
