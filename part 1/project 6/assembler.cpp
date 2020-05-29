#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<climits>
#include<math.h>
using namespace std;
struct symbol
{
	symbol *ptr;
	string sym;
	int addr;
}; symbol *head=NULL,*temp;

void addsym(symbol *ns)
{
	if(head==NULL)
	{
		head=ns;
	}
	else
	{
		temp=head;
		while(temp->ptr!=NULL)
		{
			temp=temp->ptr;
		}
		temp->ptr=ns;
	}
}
void print()
{
	if(head==NULL)
	{
		cout<<"The table is empty"<<endl;
	}
	else
	{
		temp=head;
		while(temp!=NULL)
		{
			cout<<temp->sym<<" "<<temp->addr<<endl;
			temp=temp->ptr;
		}
	}
}
int search(string stg)
{
	temp=head;
	while(temp!=NULL)
    {
		if(stg.compare(temp->sym)==0)
		{
			return temp->addr;
		}
		temp=temp->ptr;		
	}
	if(temp==NULL)
	return INT_MAX;
}
bool isNumber(string stg)
{
	for(int i=0;i<stg.length();i++)
	{
		if(isdigit(stg.at(i))==false)
		return false;
	}
	return true;
}
int main(int argc, char **argv)
{
	//cout<<"The arguments passed are "<<argv[0]<<endl;
	string stg,jump,jumpfield,dest,destfield,comp,compfield; 
	int n,address,i;
	//ifstream asmb("Add.asm");
	//ofstream mach("Add.hack");
	ifstream asmb(argv[1]);
	stg=argv[1];
	stg=stg.substr(0,stg.find(".",0));
	stg=stg+".hack";
	ofstream mach(stg.c_str());
	string predef[16]={"R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","R13","R14","R15"};
	
	for(i=0;i<16;i++)
	{
		symbol *ns;
		ns=new symbol;
		ns->ptr=NULL;
		ns->sym=predef[i];
		ns->addr=i;
		addsym(ns);
	}
	symbol *ns;
	ns=new symbol;
	ns->ptr=NULL;
	ns->sym="SCREEN";
	ns->addr=16384;
	addsym(ns);

	ns=new symbol;
	ns->ptr=NULL;
	ns->sym="KBD";
	ns->addr=24576;
	addsym(ns);
	
	ns=new symbol;
	ns->ptr=NULL;
	ns->sym="SP";
	ns->addr=0;
	addsym(ns);
	
	ns=new symbol;
	ns->ptr=NULL;
	ns->sym="LCL";
	ns->addr=1;
	addsym(ns);
	
	ns=new symbol;
	ns->ptr=NULL;
	ns->sym="ARG";
	ns->addr=2;
	addsym(ns);
	
	ns=new symbol;
	ns->ptr=NULL;
	ns->sym="THIS";
	ns->addr=3;
	addsym(ns);
    
	ns=new symbol;
	ns->ptr=NULL;
	ns->sym="THAT";
	ns->addr=4;
	addsym(ns);
	
	//firstpass
	int line=0;	
	while(!asmb.eof())
	{
		getline(asmb,stg);
		stg.erase(remove(stg.begin(),stg.end(),' '),stg.end());
		stg=stg.substr(0,stg.find("/",0));
		if(stg.length()<=0)
		{
			continue;
		}
		else
		{
			if(stg.at(0)=='(')     
			{
				stg.erase(remove(stg.begin(),stg.end(),'('),stg.end());
				stg.erase(remove(stg.begin(),stg.end(),')'),stg.end());
				symbol *ns;
				ns=new symbol;
				ns->ptr=NULL;
				ns->sym=stg;
				ns->addr=line;
				addsym(ns);
			}
			else
			line++;
		}
	}
	
	//second pass
	line=0;
	asmb.clear();
	asmb.seekg(0,ios::beg);
	int varadd=16;
	while(!asmb.eof())
	{
		getline(asmb,stg);
		stg.erase(remove(stg.begin(),stg.end(),' '),stg.end());
		stg=stg.substr(0,stg.find("/",0));	
		stg=stg.substr(0,stg.find("(",0));
		if(stg.length()<=0)	
		{
			continue;
		}
		else
		{
			//Start of A instruction.
			if(stg.at(0)=='@')
			{
				mach<<"0";
				cout<<"0";       ////////
				stg.erase(0,1);
				if(!isNumber(stg))
				{
					address=search(stg);
					if(address!=INT_MAX)
					{
						for(i=14;i>=0;i--)
						{
							if(pow(2,i)<=address)
							{
								address=address-pow(2,i);
								mach<<"1";
								cout<<"1";          /////////
							}
							else
							{
								mach<<"0";
								cout<<"0";          
							}
				        }
					}
					else //if address is not found
					{
						ns=new symbol;
						ns->sym=stg;
						ns->ptr=NULL;
						ns->addr=varadd;
						addsym(ns);
						address=varadd;
						varadd++;
						for(i=14;i>=0;i--)
						{
							if(pow(2,i)<=address)
							{
								address=address-pow(2,i);
								mach<<"1";
								cout<<"1";          /////////
							}
							else
							{
								mach<<"0";
								cout<<"0";       
							}
						}						
					}
				}
				else //if value is a number           
				{
					address=atoi(stg.c_str());  // c_str() is  used to convert string datatype into c-style strings when a function needs c-style strings as arguments.
					for(i=14;i>=0;i--)
					{
						if(pow(2,i)<=address)
						{
							address=address-pow(2,i);
							mach<<"1";
							cout<<"1";          /////////
						}
						else
						{
							mach<<"0";
							cout<<"0";  
						}
					}					
				}
			    mach<<endl;
			    cout<<endl;        ////////
			    line++;
			}
			else //start of C instrucion
			{
				n=stg.length();
				for(i=0;i<n;i++)
				{
					if(stg.at(i)==';')
					{
						jump.assign(stg,i+1,n-i-1);
						stg=stg.substr(0,stg.find(";",0));
						break;
					}
				}
				if(n>stg.length())
				{
					if(jump.compare("JMP")==0)
					{
						jumpfield.assign("111");
					}
					else if(jump.compare("JGT")==0)
					{
						jumpfield.assign("001");
					}
					else if(jump.compare("JEQ")==0)
					{
						jumpfield.assign("010");
					}
					else if(jump.compare("JGE")==0)
					{
						jumpfield.assign("011");
					}
					else if(jump.compare("JLT")==0)
					{
						jumpfield.assign("100");
					}
					else if(jump.compare("JNE")==0)
					{
						jumpfield.assign("101");
					}
					else if(jump.compare("JLE")==0)
					{
						jumpfield.assign("110");
					}																									
				}
				else
				{
					jumpfield.assign("000");
				}
				n=stg.length();
				for(i=0;i<n;i++)
				{
					if(stg.at(i)=='=')
					{
						comp.assign(stg,i+1,n-i-1);
						stg=stg.substr(0,stg.find("=",0));                         //modify to delete comments if "//" is detected
						if(stg.compare("M")==0)
						{
							destfield.assign("001");
							break;
						}
						else if(stg.compare("D")==0)
						{
							destfield.assign("010");
							break;
						}
						
						else if(stg.compare("MD")==0)
						{
							destfield.assign("011");
							break;
						}
						
						else if(stg.compare("A")==0)
						{
							destfield.assign("100");
							break;
						}
						
						else if(stg.compare("AM")==0)
						{
							destfield.assign("101");
							break;
						}
						
						else if(stg.compare("AD")==0)
						{
							destfield.assign("110");
							break;
						}
						
						else if(stg.compare("AMD")==0)
						{
							destfield.assign("111");
							break;
						}	
					}
				}
				if(i==n)
				{
					destfield.assign("000");
					comp.assign(stg);
			    }
				if(comp.compare("0")==0)
				{
					compfield.assign("0101010");
				}
				else if(comp.compare("1")==0)
				{
					compfield.assign("0111111");
				}
				else if(comp.compare("-1")==0)
				{
					compfield.assign("0111010");
				}
				else if(comp.compare("D")==0)
				{
					compfield.assign("0001100");
				}
				else if(comp.compare("A")==0)
				{
					compfield.assign("0110000");
				}
				else if(comp.compare("M")==0)
				{
					compfield.assign("1110000");
				}
				else if(comp.compare("!D")==0)
				{
					compfield.assign("0001101");
				}
				else if(comp.compare("!A")==0)
				{
					compfield.assign("0110001");
				}
				else if(comp.compare("!M")==0)
				{
					compfield.assign("1110001");
				}
				else if(comp.compare("-D")==0)
				{
					compfield.assign("0001111");
				}
				else if(comp.compare("-A")==0)
				{
					compfield.assign("0110011");
				}
				else if(comp.compare("-M")==0)
				{
					compfield.assign("1110011");
				}
				else if(comp.compare("D+1")==0)
				{
					compfield.assign("0011111");
				}
				else if(comp.compare("A+1")==0)
				{
					compfield.assign("0110111");
				}
				else if(comp.compare("M+1")==0)
				{
					compfield.assign("1110111");
				}
				else if(comp.compare("D-1")==0)
				{
					compfield.assign("0001110");
				}
				else if(comp.compare("A-1")==0)
				{
					compfield.assign("0110010");
				}
				else if(comp.compare("M-1")==0)
				{
					compfield.assign("1110010");
				}
				else if(comp.compare("D+A")==0)          // or A+D
				{
					compfield.assign("0000010");
				}
				else if(comp.compare("D+M")==0)          // or M+D
				{
					compfield.assign("1000010");
				}
				else if(comp.compare("D-A")==0)
				{
					compfield.assign("0010011");
				}
				else if(comp.compare("D-M")==0)
				{
					compfield.assign("1010011");
				}						
				else if(comp.compare("A-D")==0)
				{
					compfield.assign("0000111");
				}
				else if(comp.compare("M-D")==0)
				{
					compfield.assign("1000111");
				}
				else if(comp.compare("D&A")==0)
				{
					compfield.assign("0000000");
				}
				else if(comp.compare("D&M")==0)
				{
					compfield.assign("1000000");
				}
				else if(comp.compare("D|A")==0)
				{
					compfield.assign("0010101");
				}
				else if(comp.compare("D|M")==0)
				{
					compfield.assign("1010101");
				}				
				mach<<"111"<<compfield<<destfield<<jumpfield<<endl;
				cout<<"111"<<compfield<<destfield<<jumpfield<<endl;	
				line++;			
			}
		}
	}
	asmb.close();
	mach.close();
	cout<<line<<" number of instructions"<<endl;
	return 0;
}








