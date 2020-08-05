Projects 10 and 11 develop the front-end of the two-tier compiler of the computer. 

The front-end of the compiler is divided into two stages(projets 10 and 11). 

The project 10, the first stage of the front-end of the compiler, Jack syntax analyzer, develops the capability of the compiler to analyze the jack programs by tokenizing the input
into keywords, symbols, interger constants, string constants and identifiers and parses them; which is nothing but to "fit" the given Jack input into a parse tree which follows the
rules of Jack language grammar.

This stage of the front-end, JackAnalyzer, takes high-level jack programs as input and generates two output .xml files, one of which contains all the tokens one by one in the given 
Jack program ignoring whitespaces and comments. The other .xml file contains the textual parse tree of the given Jack program, following the grammatical rules of the jack language.
So we write a JackAnalyzer program in some high level language of our choice that does the job.
