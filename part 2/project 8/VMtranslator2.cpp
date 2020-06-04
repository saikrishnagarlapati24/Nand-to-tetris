#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<algorithm>
#include<dirent.h>
using namespace std;

class parser
{
	public :
		string curcom,arg1,arg2,arg3,cmdtype,curfun;
		int curfuncalls;
		parser()
		{
			curfun="";
			curfuncalls=0;
		}
		int n,l;
		void parse()
		{
			n=curcom.length();
			//push commands
			if(curcom.compare(0,4,"push")==0)
			{
				arg1.assign(curcom,0,4);
				cmdtype="C_PUSH";
				if(curcom.compare(4,5,"local")==0)
				{
					arg2.assign(curcom,4,5);
					arg3.assign(curcom,9,n-9);
				}
				else if(curcom.compare(4,8,"argument")==0)
				{
					arg2.assign(curcom,4,8);
					arg3.assign(curcom,12,n-12);
				}
				else if(curcom.compare(4,4,"this")==0)
				{
					arg2.assign(curcom,4,4);
					arg3.assign(curcom,8,n-8);					
				}
				else if(curcom.compare(4,4,"that")==0)
				{
					arg2.assign(curcom,4,4);
					arg3.assign(curcom,8,n-8);					
				}
				else if(curcom.compare(4,8,"constant")==0)
				{
					arg2.assign(curcom,4,8);
					arg3.assign(curcom,12,n-12);					
				}
				else if(curcom.compare(4,6,"static")==0)
				{
					arg2.assign(curcom,4,6);
					arg3.assign(curcom,10,n-10);					
				}
				else if(curcom.compare(4,4,"temp")==0)
				{
					arg2.assign(curcom,4,4);
					arg3.assign(curcom,8,n-8);					
				}	
				else if(curcom.compare(4,7,"pointer")==0)
				{
					arg2.assign(curcom,4,7);
					arg3.assign(curcom,11,n-11);					
				}																			
			}
			//pop commands
			else if(curcom.compare(0,3,"pop")==0)
			{
				arg1.assign(curcom,0,3);
				cmdtype="C_POP";
				if(curcom.compare(3,5,"local")==0)
				{
					arg2.assign(curcom,3,5);
					arg3.assign(curcom,8,n-8);
				}
				else if(curcom.compare(3,8,"argument")==0)
				{
					arg2.assign(curcom,3,8);
					arg3.assign(curcom,11,n-11);
				}
				else if(curcom.compare(3,4,"this")==0)
				{
					arg2.assign(curcom,3,4);
					arg3.assign(curcom,7,n-7);					
				}
				else if(curcom.compare(3,4,"that")==0)
				{
					arg2.assign(curcom,3,4);
					arg3.assign(curcom,7,n-7);					
				}
				else if(curcom.compare(3,6,"static")==0)
				{
					arg2.assign(curcom,3,6);
					arg3.assign(curcom,9,n-9);					
				}
				else if(curcom.compare(3,4,"temp")==0)
				{
					arg2.assign(curcom,3,4);
					arg3.assign(curcom,7,n-7);					
				}	
				else if(curcom.compare(3,7,"pointer")==0)
				{
					arg2.assign(curcom,3,7);
					arg3.assign(curcom,10,n-10);					
				}					
			}
			//Arithmetic logical commands
			else if((curcom.compare("add")==0)||(curcom.compare("sub")==0)||(curcom.compare("neg")==0)||(curcom.compare("eq")==0)||(curcom.compare("gt")==0)||(curcom.compare("lt")==0)||
			(curcom.compare("and")==0)||(curcom.compare("or")==0)||(curcom.compare("not")==0))                     
			{
				cmdtype="C_ARITHMETIC";
				arg1.assign(curcom);
			}	
			//label and branching commands
			else if(curcom.compare(0,5,"label")==0)
			{
				cmdtype="C_LABEL";
				arg2.assign(curcom,5,n-5);
			}
			else if(curcom.compare(0,7,"if-goto")==0)
			{
				cmdtype="C_IFGOTO";
				arg2.assign(curcom,7,n-7);
			}
			else if(curcom.compare(0,4,"goto")==0)
			{
				cmdtype="C_GOTO";
				arg2.assign(curcom,4,n-4);             
			}
			//function
			else if(curcom.compare(0,8,"function")==0)
			{
				cmdtype="C_FUNCTION";
				curfuncalls=0;
			}
			//call
			else if(curcom.compare(0,4,"call")==0)
			{
				cmdtype="C_CALL";
				curfuncalls++;
			}
			//return
			else if(curcom.compare(0,6,"return")==0)
			{
				cmdtype="C_RETURN";
			}
		}
};

class codewriter
{
	private :
		int eqlabctr;
		int gtlabctr;
		int ltlabctr;
		ofstream asmb;
		int l;
	public :
		codewriter(string outputfil)                  
		{
			eqlabctr=1;
			gtlabctr=1;
			ltlabctr=1;
			asmb.open(outputfil.c_str()); 
		}
		//codewriter push
		void push(parser pr,string classname)
		{
			asmb<<"//"<<pr.curcom<<endl;
			if((pr.arg2).compare("local")==0)
			{
				asmb<<"@LCL \nD=M \n@"<<pr.arg3<<" \nA=D+A \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			}
			else if((pr.arg2).compare("argument")==0)
			{
				asmb<<"@ARG \nD=M \n@"<<pr.arg3<<" \nA=D+A \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			}
			else if((pr.arg2).compare("this")==0)
			{
				asmb<<"@THIS \nD=M \n@"<<pr.arg3<<" \nA=D+A \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			}
			else if((pr.arg2).compare("that")==0)
			{
				asmb<<"@THAT \nD=M \n@"<<pr.arg3<<" \nA=D+A \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			}
			else if((pr.arg2).compare("constant")==0)
			{
				asmb<<"@"<<pr.arg3<<" \nD=A \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			}
			else if((pr.arg2).compare("static")==0)
			{
				asmb<<"@"<<classname<<pr.arg3<<" \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";          
			}
			else if((pr.arg2).compare("temp")==0)
			{
				asmb<<"@"<<5+atoi((pr.arg3).c_str())<<" \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			}
			else if((pr.arg2).compare("pointer")==0)
			{
				if((pr.arg3).compare("0")==0)
				{
					asmb<<"@THIS \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
				}
				else if((pr.arg3).compare("1")==0)
				{
					asmb<<"@THAT \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
				}
			}						
		} 
		//codewriter pop
		void pop(parser pr,string classname)
		{
			asmb<<"//"<<pr.curcom<<endl;
			if((pr.arg2).compare("local")==0)
			{
				asmb<<"@LCL \nD=M \n@"<<pr.arg3<<" \nD=D+A \n@R13 \nM=D \n@SP \nAM=M-1 \nD=M \n@R13 \nA=M \nM=D \n";
			}
			else if((pr.arg2).compare("argument")==0)
			{
				asmb<<"@ARG \nD=M \n@"<<pr.arg3<<" \nD=D+A \n@R13 \nM=D \n@SP \nAM=M-1 \nD=M \n@R13 \nA=M \nM=D \n";
			}
			else if((pr.arg2).compare("this")==0)
			{
				asmb<<"@THIS \nD=M \n@"<<pr.arg3<<" \nD=D+A \n@R13 \nM=D \n@SP \nAM=M-1 \nD=M \n@R13 \nA=M \nM=D \n";
			}
			else if((pr.arg2).compare("that")==0)
			{
				asmb<<"@THAT \nD=M \n@"<<pr.arg3<<" \nD=D+A \n@R13 \nM=D \n@SP \nAM=M-1 \nD=M \n@R13 \nA=M \nM=D \n";
			}
			else if((pr.arg2).compare("static")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \n@"<<classname<<pr.arg3<<" \nM=D \n";
			}
			else if((pr.arg2).compare("temp")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \n@"<<5+atoi((pr.arg3).c_str())<<" \nM=D \n";
			}
			else if((pr.arg2).compare("pointer")==0)
			{
				if((pr.arg3).compare("0")==0)
				{
					asmb<<"@SP \nAM=M-1 \nD=M \n@THIS \nM=D \n";
				}
				else if((pr.arg3).compare("1")==0)
				{
					asmb<<"@SP \nAM=M-1 \nD=M \n@THAT \nM=D \n";
				}
			}
		}
		//codewriter arithmetic logical
		void arithmetic(parser pr)
		{
			asmb<<"//"<<pr.curcom<<endl;
			if((pr.arg1).compare("add")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \nA=A-1 \nM=D+M \n";
			}
			else if((pr.arg1).compare("sub")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \nA=A-1 \nM=M-D \n";
			}
			else if((pr.arg1).compare("neg")==0)
			{
				asmb<<"@SP \nA=M-1 \nM=-M \n";
			}
			else if((pr.arg1).compare("eq")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \nA=A-1 \nD=D-M \n@EQ"<<eqlabctr<<" \nD;JEQ \n@SP \nA=M-1 \nM=0 \n";
				asmb<<"@OVEQ"<<eqlabctr<<" \n0,JMP \n(EQ"<<eqlabctr<<") \n@SP \nA=M-1 \nM=-1 \n(OVEQ"<<eqlabctr<<") \n";   
				eqlabctr++;
			}
			else if((pr.arg1).compare("gt")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \nA=A-1 \nD=D-M \n@GT"<<gtlabctr<<" \nD;JLT \n@SP \nA=M-1 \nM=0 \n";
				asmb<<"@OVGT"<<gtlabctr<<" \n0,JMP \n(GT"<<gtlabctr<<") \n@SP \nA=M-1 \nM=-1 \n(OVGT"<<gtlabctr<<") \n"; 
				gtlabctr++;  
			}
			else if((pr.arg1).compare("lt")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \nA=A-1 \nD=D-M \n@LT"<<ltlabctr<<" \nD;JGT \n@SP \nA=M-1 \nM=0 \n";
				asmb<<"@OVLT"<<ltlabctr<<" \n0,JMP \n(LT"<<ltlabctr<<") \n@SP \nA=M-1 \nM=-1 \n(OVLT"<<ltlabctr<<") \n"; 
				ltlabctr++;  				
			}
			else if((pr.arg1).compare("and")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \nA=A-1 \nM=D&M \n";
			}
			else if((pr.arg1).compare("or")==0)
			{
				asmb<<"@SP \nAM=M-1 \nD=M \nA=A-1 \nM=D|M \n";
			}
			else if((pr.arg1).compare("not")==0)
			{
				asmb<<"@SP \nA=M-1 \nM=!M \n";
			}							
		}
		//bootstrap code
		void writinit(parser &pr,string callcom)
		{
			string funname,nargs,unaltcom;
			unaltcom=callcom;
			//unaltcom=unaltcom.substr(0,unaltcom.find("/",0));
			for(int i=unaltcom.length()-1;i>=0;i--)
			{
				if(unaltcom.at(i)!=' ')
				{
					for(int j=i;j>=0;j--)
					{
						if(unaltcom.at(j)==' ')
						{
							j++;
							nargs.assign(unaltcom,j,i-j+1);
							unaltcom=unaltcom.substr(0,j-1);
							unaltcom.erase(remove(unaltcom.begin(),unaltcom.end(),' '),unaltcom.end());
							funname.assign(unaltcom,4,unaltcom.length()-4);
							i=-1;
							break;
						}
					}
				}
			}
			asmb<<"//bootstrapcode \n";			
			asmb<<"@256 \nD=A \n@SP \nM=D \n@"<<pr.curfun<<"$ret."<<pr.curfuncalls<<" \nD=A \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@LCL \n";
			asmb<<"D=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@ARG \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@THIS \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			asmb<<"@THAT \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@5 \nD=A \n@"<<nargs<<" \nD=D+A \n@SP \nD=M-D \n@ARG \nM=D \n@SP \nD=M \n@LCL \nM=D \n";
			asmb<<"@"<<funname<<" \n0;JMP \n";
			asmb<<"("<<pr.curfun<<"$ret."<<pr.curfuncalls<<") \n";
		}
		
		//codewriter labels and branching
		void ifgoto(parser pr)
		{
			asmb<<"//"<<pr.curcom<<endl;
			asmb<<"@SP \nAM=M-1 \nD=M \n@"<<pr.curfun<<"$"<<pr.arg2<<" \nD,JNE \n";
		}
		void gotoo(parser pr)
		{
			asmb<<"//"<<pr.curcom<<endl;
			asmb<<"@"<<pr.curfun<<"$"<<pr.arg2<<" \n0;JMP \n";
		}
		void label(parser pr)
		{
			asmb<<"//"<<pr.curcom<<endl;
			asmb<<"("<<pr.curfun<<"$"<<pr.arg2<<") \n";
		}
		//codewriter call
		void call(parser &pr,string unaltcom)
		{
			string funname,nargs;
			unaltcom=unaltcom.substr(0,unaltcom.find("/",0));
			for(int i=unaltcom.length()-1;i>=0;i--)
			{
				if(unaltcom.at(i)!=' ')
				{
					for(int j=i;j>=0;j--)
					{
						if(unaltcom.at(j)==' ')
						{
							j++;
							nargs.assign(unaltcom,j,i-j+1);
							unaltcom=unaltcom.substr(0,j-1);
							unaltcom.erase(remove(unaltcom.begin(),unaltcom.end(),' '),unaltcom.end());
							funname.assign(unaltcom,4,unaltcom.length()-4);
							i=-1;
							break;
						}
					}
				}
			}			
			asmb<<"//"<<pr.curcom<<endl;
			asmb<<"@"<<pr.curfun<<"$ret."<<pr.curfuncalls<<" \nD=A \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@LCL \n";
			asmb<<"D=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@ARG \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@THIS \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
			asmb<<"@THAT \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n@5 \nD=A \n@"<<nargs<<" \nD=D+A \n@SP \nD=M-D \n@ARG \nM=D \n@SP \nD=M \n@LCL \nM=D \n";
			asmb<<"@"<<funname<<" \n0;JMP \n";
			asmb<<"("<<pr.curfun<<"$ret."<<pr.curfuncalls<<") \n";
		}
		//codewriter function
		void func(parser &pr,string unaltcom)
		{
			string funcname,nvars;
			unaltcom=unaltcom.substr(0,unaltcom.find("/",0));
			for(int i=unaltcom.length()-1;i>=0;i--)
			{
				if(unaltcom.at(i)!=' ')
				{
					for(int j=i;j>=0;j--)
					{
						if(unaltcom.at(j)==' ')
						{
							j++;
							nvars.assign(unaltcom,j,i-j+1);
							unaltcom=unaltcom.substr(0,j-1);
							unaltcom.erase(remove(unaltcom.begin(),unaltcom.end(),' '),unaltcom.end());
							funcname.assign(unaltcom,8,unaltcom.length()-8);
							pr.curfun=funcname;
							i=-1;
							break;
						}
					}
				}
			}
			asmb<<"//"<<pr.curcom<<endl;
	        asmb<<"("<<pr.curfun<<") \n";
			asmb<<"@R13 \nM=0 \n("<<pr.curfun<<"LCL) \n@R13 \nD=M \n@"<<nvars<<" \nD=D-A \n@"<<pr.curfun<<"LCLEND \nD;JGE \n@R13 \nMD=M+1 \n";
			asmb<<"D=D-1 \n@LCL \nA=M+D \nM=0 \n@"<<pr.curfun<<"LCL \n0;JMP \n("<<pr.curfun<<"LCLEND) \n@LCL \nD=M \n@"<<nvars<<" \nD=D+A \n";
			asmb<<"@SP \nM=D \n";
		}		
		//codewriter return
		void ret(parser pr)
		{
			asmb<<"//"<<pr.curcom<<endl;
			asmb<<"@LCL \nD=M \n@endframe \nM=D \n@5 \nD=A \n@endframe \nA=M-D \nD=M \n@retaddr \nM=D \n@SP \nA=M-1 \nD=M \n@ARG \nA=M \nM=D \n";
			asmb<<"@ARG \nD=M+1 \n@SP \nM=D \n@1 \nD=A \n@endframe \nA=M-D \nD=M \n@THAT \nM=D \n@2 \nD=A \n@endframe \nA=M-D \nD=M \n@THIS \nM=D \n";
			asmb<<"@3 \nD=A \n@endframe \nA=M-D \nD=M \n@ARG \nM=D \n@4 \nD=A \n@endframe \nA=M-D \nD=M \n@LCL \nM=D \n@retaddr \nA=M \n0;JMP \n";
		}  
		//close asm file
		void closeasm()
		{
			asmb.close();
		}				          
};

void drive(string file,codewriter &cw,parser &pr,string classname)
{
	ifstream vm(file.c_str());
	string stg,unaltcom;
	while(!vm.eof())
	{
		getline(vm,stg);
		unaltcom=stg;
		stg.erase(remove(stg.begin(),stg.end(),' '),stg.end());
		stg=stg.substr(0,stg.find("/",0));
		if(stg.length()<=0)
		continue;
		else
		{
			pr.curcom=stg;
			pr.parse();
			if((pr.cmdtype).compare("C_ARITHMETIC")==0)
			{
				cw.arithmetic(pr);
			}
			else if((pr.cmdtype).compare("C_PUSH")==0)
			{
				cw.push(pr,classname);
			}
			else if((pr.cmdtype).compare("C_POP")==0)
			{
				cw.pop(pr,classname);
			}
			else if((pr.cmdtype).compare("C_IFGOTO")==0)
			{
				cw.ifgoto(pr);
			}
			else if((pr.cmdtype).compare("C_GOTO")==0)
			{
				cw.gotoo(pr);
			}
			else if((pr.cmdtype).compare("C_FUNCTION")==0)
			{
				cw.func(pr,unaltcom);
			}
			else if((pr.cmdtype).compare("C_CALL")==0)
			{
				cw.call(pr,unaltcom);
			}
			else if((pr.cmdtype).compare("C_RETURN")==0)
			{
				cw.ret(pr);
			}
			else if((pr.cmdtype).compare("C_LABEL")==0)
			{
				cw.label(pr);
			}								
		}
	}
	vm.close();	
}
int main(int argc, char **argv) 
{
	string givarg,opfilename,str,classname;
	int n,l;
	givarg=argv[1];
	n=givarg.length();
	parser pr;
	if(givarg.compare(n-3,3,".vm")==0)
	{
		/*l=-1;
		for(int i=0;i<givarg.length();i++)
		{
			if(givarg.at(i)=='\\' ||  givarg.at(i)=='/')
			{
				l=i;
			}
		}
		classname.assign(givarg,l+1,givarg.length()-l-3);
		opfilename=givarg.substr(0,l);
		for(int i=0;i<opfilename.length();i++)
		{
			if(opfilename.at(i)=='\\'||opfilename.at(i)=='/')
			{
				l=i;
			}
		}
		str.assign(opfilename,l+1,opfilename.length()-l-1);
		opfilename=opfilename+opfilename.at(l);
		opfilename=opfilename+str; 
		opfilename=opfilename+".asm";*/
		classname.assign(givarg,0,n-2);
		opfilename=classname+"asm";
		codewriter cw(opfilename);
		drive(givarg,cw,pr,classname);
		cw.closeasm();
	}
	
	else
	{
		/*for(int i=0;i<givarg.length();i++)
		{
			if(givarg.at(i)=='\\'||givarg.at(i)=='/')
			{
				l=i;
			}
		}
		str.assign(givarg,l+1,n-l-1);
		opfilename=givarg+givarg.at(l);
		opfilename=opfilename+str; 
		opfilename=opfilename+".asm";
		
		codewriter cw(opfilename);
		opfilename=givarg.at(l);
		cw.writinit(pr,"call Sys.init 0");*/
		opfilename=givarg;
		opfilename=opfilename+"\\";         // opfilename=opfilename+"/";
		opfilename=opfilename+givarg;
		opfilename=opfilename+".asm";
		codewriter cw(opfilename);
		cw.writinit(pr,"call Sys.init 0");
		DIR *d;
		struct dirent *dir;
		d=opendir(givarg.c_str());
		if(d)
		{
			while((dir=readdir(d))!=NULL)
			{
				str=dir->d_name;
				if(str.length()>=3)
				{
					if(str.compare((str.length())-3,3,".vm")==0)
					{
						classname.assign(str,0,str.length()-2);
						givarg=argv[1];
						givarg=givarg+"\\";            //givarg=givarg+"/";
						givarg=givarg+str;
						drive(givarg,cw,pr,classname);
					}					
				}			
			}
		}
		closedir(d);
		cw.closeasm();
	}
	return 0;                       
}

