//pushconstant3030
@3030 
D=A 
@SP 
AM=M+1 
A=A-1 
M=D 
//poppointer0
@SP 
AM=M-1 
D=M 
@THIS 
M=D 
//pushconstant3040
@3040 
D=A 
@SP 
AM=M+1 
A=A-1 
M=D 
//poppointer1
@SP 
AM=M-1 
D=M 
@THAT 
M=D 
//pushconstant32
@32 
D=A 
@SP 
AM=M+1 
A=A-1 
M=D 
//popthis2
@THIS 
D=M 
@2 
D=D+A 
@R13 
M=D 
@SP 
AM=M-1 
D=M 
@R13 
A=M 
M=D 
//pushconstant46
@46 
D=A 
@SP 
AM=M+1 
A=A-1 
M=D 
//popthat6
@THAT 
D=M 
@6 
D=D+A 
@R13 
M=D 
@SP 
AM=M-1 
D=M 
@R13 
A=M 
M=D 
//pushpointer0
@THIS 
D=M 
@SP 
AM=M+1 
A=A-1 
M=D 
//pushpointer1
@THAT 
D=M 
@SP 
AM=M+1 
A=A-1 
M=D 
//add
@SP 
AM=M-1 
D=M 
A=A-1 
M=D+M 
//pushthis2
@THIS 
D=M 
@2 
A=D+A 
D=M 
@SP 
AM=M+1 
A=A-1 
M=D 
//sub
@SP 
AM=M-1 
D=M 
A=A-1 
M=M-D 
//pushthat6
@THAT 
D=M 
@6 
A=D+A 
D=M 
@SP 
AM=M+1 
A=A-1 
M=D 
//add
@SP 
AM=M-1 
D=M 
A=A-1 
M=D+M 
