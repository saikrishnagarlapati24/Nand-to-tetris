A computer is supposed to execte the programs written in some high-level languages like c++, java, python etc. But for it, somebody has to translate those high-level programs into
machine language programs which are understable to the computer.  A compiler is the program that does that job for us.
The high-level language that we use to write programs on this computer is called Jack, an object-oriented java like language. Building a compiler and operating system for this computer is what part 2 is all about. 
We develop a two-tier compiler for our computer which has a front-end and a back-end. 
The high-level language programs in Jack are first translated to an intermediary language called virtual machine language (VM language) in the front-end of the compiler which runs on a stack-based virtual machine.
In the back-end, these VM language programs are translated to the assembly language of the computer(assembler translates these assembly programs into machine language).
Projects 7 and 8 are about developing the back-end of our compiler, i.e., VM translator. 
In project 7 we develop a parial VM translator which takes an input .vm file which has only push, pop and arithmetic commands of the VM language and translates them into assembly
language commands
