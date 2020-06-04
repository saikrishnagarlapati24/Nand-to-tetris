#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<algorithm>
using namespace std;

class parser
{
	public :
		string curcom,arg1,arg2,arg3,cmdtype;
		int n;
		void parse()
		{
			n=curcom.length();
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
			else if((curcom.compare("add")==0)||(curcom.compare("sub")==0)||(curcom.compare("neg")==0)||(curcom.compare("eq")==0)||(curcom.compare("gt")==0)||(curcom.compare("lt")==0)||
			(curcom.compare("and")==0)||(curcom.compare("or")==0)||(curcom.compare("not")==0))                     
			{
				cmdtype="C_ARITHMETIC";
				arg1.assign(curcom);
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
		string outputfile;
	public :
		codewriter(string outputfil)
		{
			eqlabctr=1;
			gtlabctr=1;
			ltlabctr=1;
			asmb.open(outputfil.c_str());
			l=-1;
			for(int i=0;i<outputfil.length();i++)
			{
				if(outputfil.at(i)=='\\' ||  outputfil.at(i)=='/')
				{
					l=i;
				}
			}
			outputfile.assign(outputfil,l+1,outputfil.length()-l-4);
			cout<<outputfile<<endl;
		}
		void push(parser pr)
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
				asmb<<"@"<<outputfile<<pr.arg3<<" \nD=M \n@SP \nAM=M+1 \nA=A-1 \nM=D \n";
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
		void pop(parser pr)
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
				asmb<<"@SP \nAM=M-1 \nD=M \n@"<<outputfile<<pr.arg3<<" \nM=D \n";
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
		void close()
		{
			asmb.close();
		}
};
int main(int argc, char **argv) 
{
	ifstream vm(argv[1]);
	string outputfile;
	outputfile=argv[1];
	outputfile=outputfile.substr(0,outputfile.find(".",0));
	outputfile=outputfile+".asm";
	cout<<outputfile<<endl;
	parser pr;
	codewriter cw(outputfile);
	string stg;
	while(!vm.eof())
	{
		getline(vm,stg);
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
				cw.push(pr);
			}
			else if((pr.cmdtype).compare("C_POP")==0)
			{
				cw.pop(pr);
			}
		}
	}
	cw.close();
	vm.close();
	return 0;                       
}

