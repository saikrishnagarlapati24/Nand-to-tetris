#include<iostream>
#include<cstring>
#include<string>
#include<fstream>
#include<algorithm>
#include<dirent.h>
#include<climits>
using namespace std;

class tokenizer{
	private:
		ifstream jack;
		ofstream tfile;
		string token, token_type, keyword[21];
		char c;
		bool got_token;
		char sym[19];
		
	public:
		tokenizer(string path){
			
			jack.open(path.c_str());
			tfile.open((path.substr(0, path.length()-5)+"T.xml").c_str());
			got_token=false;
			sym[0]= '{'; 
			sym[1]= '}'; sym[10]='-';
			sym[2]= '('; sym[11]='*';
			sym[3]= ')'; sym[12]='/';
			sym[4]= '['; sym[13]='&';
			sym[5]= ']'; sym[14]='|';
			sym[6]= '.'; sym[15]='<';
			sym[7]= ','; sym[16]='>';
			sym[8]= ';'; sym[17]='=';
			sym[9]= '+'; sym[18]='~';
			
			keyword[0]="class"; keyword[1]="constructor";
			keyword[2]="function"; keyword[3]="method";
			keyword[4]="field"; keyword[5]="static";
			keyword[6]="var"; keyword[7]="int";
			keyword[8]="char"; keyword[9]="boolean";
			keyword[10]="void"; keyword[11]="true";
			keyword[12]="false"; keyword[13]="null";
			keyword[14]="this"; keyword[15]="let";
			keyword[16]="do"; keyword[17]="if";
			keyword[18]="else"; keyword[19]="while";
			keyword[20]="return";
			
			tfile<<"<tokens>"<<endl;
		}
		
		bool isdigit(char a){
			for(int i=48; i<58; i++){
				if(int(a)==i){
					return true;
				}
			}
			return false;
		}
		
		bool isalpha(char a){
			for(int i=65; i<91; i++){
				if(int(a)==i){
					return true;
				}
			}
			
			for(int i=97; i<123; i++){
				if(int(a)==i){
					return true;
				}
			}
			return false;
		}
		
		bool issymbol(char a){
			for(int i=0; i<19; i++){
				if(a==sym[i]){
					return true;
				}
			}
			return false;
		}
		
		bool iskeyword(string s){
			for(int i=0; i<21; i++){
				if(s.compare(keyword[i])==0){
					return true;
				}
			}
			return false;
		}
		
		void printToken(){
			if(token_type.compare("KEYWORD")==0){
				tfile<<"	"<<"<keyword> "<<token<<" </keyword>"<<endl;
			}
			
			else if(token_type.compare("SYMBOL")==0){
				tfile<<"	"<<"<symbol> ";
				if(token.compare("<")==0){
					tfile<<"&lt;"<<" </symbol>"<<endl;
				}
				else if(token.compare(">")==0){
					tfile<<"&gt;"<<" </symbol>"<<endl;
				}
				else if(token.compare("\"")==0){
					tfile<<"&quot;"<<" </symbol>"<<endl;
				}
				else if(token.compare("&")==0){
					tfile<<"&amp;"<<" </symbol>"<<endl;
				}
				else{
					tfile<<token<<" </symbol>"<<endl;
				}
			}
			
			else if(token_type.compare("INT_CONST")==0){
				tfile<<"	"<<"<integerConstant> "<<token<<" </integerConstant>"<<endl;
			}
			else if(token_type.compare("STRING_CONST")==0){
				tfile<<"	"<<"<stringConstant> "<<token<<" </stringConstant>"<<endl;
			}
			else if(token_type.compare("IDENTIFIER")==0){
				tfile<<"	"<<"<identifier> "<<token<<" </identifier>"<<endl;
			}
			return;
		}
		
		bool eof(){
			if(jack.eof()){
				return true;
			}
			else{
				return false;
			}
		}
		
		void advance(){
			token="";
			while(!got_token && !jack.eof()){  //got_token and jack.eof() can not be true at the same time.
				jack.get(c);
				if( (c!=' ')&&(c!='\n') ){
					//String constant
					if(c=='"'){
						jack.get(c);
						while(c!='"'){
							token=token+c;
							jack.get(c);
						}
						token_type="STRING_CONST";
						got_token=true;
					}
					//integer constant
					else if( isdigit(c) ){
						token=token+c;
						jack.get(c);
						while(isdigit(c)){
							token=token+c;
							jack.get(c);
						}
						jack.unget();
						token_type="INT_CONST";
						got_token=true;
					}
					//symbol
					else if( issymbol(c) ){
						if(c=='/'){
							jack.get(c);
							if(c=='/'){
								jack.get(c);
								while((c!='\n') && !jack.eof()){
									jack.get(c);
								}
							}
							else if(c=='*'){
								bool endofcomm=false;
								while(!endofcomm && !jack.eof()){
									jack.get(c);
									if(c=='*'){
										jack.get(c);
										if(c=='/'){
											endofcomm=true;
										}
									}
								}
							}
							else{
								jack.unget();
								token='/';
								token_type="SYMBOL";
								got_token=true;
							}
						}
						else{
							token=token+c;
							token_type="SYMBOL";
							got_token=true;
						}
					}
					//Keyword or identifier
					else if(isalpha(c)){
						token=token+c;
						jack.get(c);
						while(isalpha(c)){
							token=token+c;
							jack.get(c);
						}
						if(issymbol(c)||(c==' ')){
							if(iskeyword(token)){
								jack.unget();
								token_type="KEYWORD";
								got_token=true;
							}
							else{
								jack.unget();
								token_type="IDENTIFIER";
								got_token=true;
							}
						}
						else if(isdigit(c)||(c=='_')){
							token=token+c;
							jack.get(c);
							while(isalpha(c)||isdigit(c)||(c=='_')){
								token=token+c;
								jack.get(c);
							}
							jack.unget();
							token_type="IDENTIFIER";
							got_token=true;
						}
					}
					//only identifier
					else if(c=='_'){
						token=token+c;
						jack.get(c);
						while(isalpha(c)||isdigit(c)||(c=='_')){
							token=token+c;
							jack.get(c);
						}
						jack.unget();
						token_type="IDENTIFIER";
						got_token=true;
					}
				}
			}
			if(got_token){
				printToken();
			}
			else if(jack.eof()){
				tfile<<"</tokens>";
				token_type="no_token";
			}
			got_token=false;
			return;
		}
		
		string tokenType(){
			return token_type;
		}
		
		string keyWord(){
			return token;
		}
		
		string symbol(){
			if(token.compare("<")==0){
				return "&lt;";
			}
			else if(token.compare(">")==0){
				return "&gt;";
			}
			else if(token.compare("\"")==0){
				return "&quot;";
			}
			else if(token.compare("&")==0){
				return "&amp;";
			}
			else{
				return token;
			}
		}
		
		string identifier(){
			return token;
		}
		
		int intval(){
			return atoi(token.c_str());
		}
		
		string stringval(){
			return token;
		}
};

struct element{
	string name, type, kind;
	int index; 
	element *nxt;
};

// 'kind' arguments for the symbol table would be static, field, arg, var; return of 'kind' would be static, field, argument, local.
class symbolTable{
	private :
		element *clas, *sr, *temp;
		int st_varcount, fi_varcount, arg_varcount, loc_varcount;
	public :
		symbolTable(){
			clas=NULL;
			sr=NULL;
			temp=NULL;
			st_varcount=-1;
			fi_varcount=-1;
			arg_varcount=-1;
			loc_varcount=-1;
		}
		
		void startSubroutine(){
			sr=NULL;
			arg_varcount=-1;
			loc_varcount=-1;
		}
		
		/*void insert(element* e, element *head){
			if(head==NULL){
				//head=e; Because this is a terrible mistake. this makes 'head=e' but not sr=e........
				
				//cout<<sr->name<<" "<<sr->type<<" "<<sr->kind<<" "<<sr->index<<endl;////////////sr->xxx is not being recognised.
				//if(sr==NULL)cout<<"sr is null"<<endl;
				//if(sr!=NULL)cout<<"sr is not NULL"<<endl;
			}
			else{
				temp=head;
				while(temp->nxt!=NULL){
					temp=temp->nxt;
				}
				temp->nxt=e;
			}
		}*/
		
		void define(string name, string type, string kind){
			element* e;
			e=new element;
			e->name=name;
			e->type=type;
			e->nxt=NULL;
			if(kind.compare("field")==0){
				e->kind="this";
			}
			else if(kind.compare("var")==0){
				e->kind="local";
			}
			else if(kind.compare("arg")==0){
				e->kind="argument";
			}
			else{
				e->kind=kind;
			}
			if(e->kind.compare("static")==0){
				st_varcount++;
				e->index=st_varcount;
			}
			else if(e->kind.compare("this")==0){
				fi_varcount++;
				e->index=fi_varcount;
			}
			else if(e->kind.compare("argument")==0){
				arg_varcount++;
				e->index=arg_varcount;
			}
			else if(e->kind.compare("local")==0){
				loc_varcount++;
				e->index=loc_varcount;
			}
			if(e->kind.compare("static")==0 || e->kind.compare("this")==0){
				//insert(e, clas);
				if(clas==NULL){
					clas=e;
				}
				else{
					temp=clas;
					while(temp->nxt!=NULL){
						temp=temp->nxt;
					}
					temp->nxt=e;
				}
			}
			else if(e->kind.compare("argument")==0 || e->kind.compare("local")==0){
				//insert(e, sr);
				if(sr==NULL){
					sr=e;
				}
				else {
					temp=sr;
					while(temp->nxt!=NULL){
						temp=temp->nxt;
					}
					temp->nxt=e;
				}
			}
		}
		
		int varCount(string kind){
			if(kind.compare("static")==0){
				return st_varcount+1;
			}
			else if(kind.compare("field")==0){
				return fi_varcount+1;
			}
			else if(kind.compare("arg")==0){
				return arg_varcount+1;
			}
			else if(kind.compare("var")==0){
				return loc_varcount+1;
			}
		}
		
		element* search(string name){
			temp=sr;
			while(temp!=NULL){
				if(temp->name.compare(name)==0){
					return temp;
				}
				else{
					temp=temp->nxt;
				}
			}
			if(temp==NULL){
				temp=clas;
				while(temp!=NULL){
					if(temp->name.compare(name)==0){
						return temp;
					}
					else{
						temp=temp->nxt;
					}
				}
				if(temp==NULL){
					return NULL;
				}
			}
		}
		
		string type_of(string name){
			element *l;
			l=search(name);
			if(l==NULL){
				return "NONE";
			}
			else{
				return l->type;
			}
		}
		
		string kindof(string name){
			element *l;
			l=search(name);
			if(l==NULL){
				return "NONE";
			}
			else{
				return l->kind;
			}
		}
		
		int indexof(string name){
			element *l;
			l=search(name);
			if(l==NULL){
				return INT_MAX;
			}
			else{
				return l->index;
			}
		}
		
		void printclas(){
			temp=clas;
			while(temp!=NULL){
				cout<<temp->name<<" "<<temp->type<<" "<<temp->kind<<" "<<temp->index<<endl;
				temp=temp->nxt;
			}
			if(temp==NULL){
				cout<<"List is empty"<<endl;
			}
		}
		
		void printsr(){
			temp=sr;
			while(temp!=NULL){
				cout<<temp->name<<" "<<temp->type<<" "<<temp->kind<<" "<<temp->index<<endl;
				temp=temp->nxt;
			}
			if(sr==NULL){
				cout<<"List is empty"<<endl;
			}
		}	
};

class vmWriter{
	private:
		ofstream vm;
	public:
		vmWriter(string path){
			vm.open((path.substr(0, path.length()-4)+"vm").c_str());
		}
		
		void writePush(string seg, int index){
			vm<<"push "<<seg<<" "<<index<<endl;
		}
		
		void writePop(string seg, int index){
			vm<<"pop "<<seg<<" "<<index<<endl;
		}
		
		void writeArithmetic(string arith){
			vm<<arith<<endl;
		}
		
		void writeLabel(string label, int i){
			vm<<"label "<<label<<"_"<<i<<endl;
		}
		
		void writeGoto(string label, int i){
			vm<<"goto "<<label<<"_"<<i<<endl;
		}
		
		void writeIf(string label, int i){
			vm<<"if-goto "<<label<<"_"<<i<<endl;
		}
		
		void writeCall(string cl, string sr, int nargs){
			vm<<"call "<<cl<<"."<<sr<<" "<<nargs<<endl;
		}
		
		void writeFunction(string cl, string sr, int nlocals){
			vm<<"function "<<cl<<"."<<sr<<" "<<nlocals<<endl;
		}
		
		void writeReturn(){
			vm<<"return"<<endl;
		}
};

//symboltable and vmwriter class variables in the compilationengine class and missed/forgotten calls to appropriate eat/write/compile methods
//write close functions in all classes.
class CompilationEngine{
	private:
		ofstream xml;
		tokenizer t;
		symbolTable s;
		vmWriter v;
		string classname, subroutine, subrouttype, subroutname;
		int nfields, subroutnlocals, whilecount, ifcount;
		bool flag;
	public:
		CompilationEngine(string path): t(path), v(path){
			xml.open((path.substr(0, path.length()-4)+"xml").c_str());
			t.advance();
		}
		
		void printntabs(int n){
			for(int i=0; i<n; i++){
				xml<<"	";
			}
			return;
		}
		
		void eatkeyWord(int n){
			if( (t.tokenType()).compare("KEYWORD")==0 ){
				printntabs(n);
				xml<<"<keyword> "<<t.keyWord()<<" </keyword>"<<endl;
				t.advance();
			}
			else{
				if( (t.tokenType()).compare("SYMBOL")==0 ){
					cout<<t.tokenType()<<" "<<t.symbol()<<" occured but keyword expected"<<endl;
				}
				else if( (t.tokenType()).compare("INT_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.intval()<<" occured but keyword expected"<<endl;
				}
				else if( (t.tokenType()).compare("STRING_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.stringval()<<" occured but keyword expected"<<endl;
				}
				else if( (t.tokenType()).compare("IDENTIFIER")==0 ){
					cout<<t.tokenType()<<" "<<t.identifier()<<" occured but keyword expected"<<endl;
				}
				t.advance();
			}
			return;
		}
		
		void eatsymbol(int n){
			if( (t.tokenType()).compare("SYMBOL")==0 ){
				printntabs(n);
				xml<<"<symbol> "<<t.symbol()<<" </symbol>"<<endl;
				t.advance();
			}
			else{
				if( (t.tokenType()).compare("KEYWORD")==0 ){
					cout<<t.tokenType()<<" "<<t.keyWord()<<" occured but symbol expected"<<endl;
				}
				else if( (t.tokenType()).compare("INT_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.intval()<<" occured but symbol expected"<<endl;
				}
				else if( (t.tokenType()).compare("STRING_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.stringval()<<" occured but symbol expected"<<endl;
				}
				else if( (t.tokenType()).compare("IDENTIFIER")==0 ){
					cout<<t.tokenType()<<" "<<t.identifier()<<" occured but symbol expected"<<endl;
				}
				t.advance();
			}
			return;
		}
		
		void eatintval(int n){
			if( (t.tokenType()).compare("INT_CONST")==0 ){
				printntabs(n);
				xml<<"<integerConstant> "<<t.intval()<<" </integerConstant>"<<endl;
				t.advance();
			}
			else{
				if( (t.tokenType()).compare("SYMBOL")==0 ){
					cout<<t.tokenType()<<" "<<t.symbol()<<" occured but integerConstant expected"<<endl;
				}
				else if( (t.tokenType()).compare("KEYWORD")==0 ){
					cout<<t.tokenType()<<" "<<t.keyWord()<<" occured but integerConstant expected"<<endl;
				}
				else if( (t.tokenType()).compare("STRING_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.stringval()<<" occured but integerConstant expected"<<endl;
				}
				else if( (t.tokenType()).compare("IDENTIFIER")==0 ){
					cout<<t.tokenType()<<" "<<t.identifier()<<" occured but integerConstant expected"<<endl;
				}
				t.advance();
			}
			return;
		}
		
		void eatstringval(int n){
			if( (t.tokenType()).compare("STRING_CONST")==0 ){
				printntabs(n);
				xml<<"<stringConstant> "<<t.stringval()<<" </stringConstant>"<<endl;
				t.advance();
			}
			else{
				if( (t.tokenType()).compare("SYMBOL")==0 ){
					cout<<t.tokenType()<<" "<<t.symbol()<<" occured but stringConstant expected"<<endl;
				}
				else if( (t.tokenType()).compare("KEYWORD")==0 ){
					cout<<t.tokenType()<<" "<<t.keyWord()<<" occured but stringConstant expected"<<endl;
				}
				else if( (t.tokenType()).compare("INT_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.intval()<<" occured but stringConstant expected"<<endl;
				}
				else if( (t.tokenType()).compare("IDENTIFIER")==0 ){
					cout<<t.tokenType()<<" "<<t.identifier()<<" occured but stringConstant expected"<<endl;
				}
				t.advance();
			}
			return;
		}
		
		void eatidentifier(int n){
			if( (t.tokenType()).compare("IDENTIFIER")==0 ){
				printntabs(n);
				xml<<"<identifier> "<<t.identifier()<<" </identifier>"<<endl;
				t.advance();
			}
			else{
				if( (t.tokenType()).compare("SYMBOL")==0 ){
					cout<<t.tokenType()<<" "<<t.symbol()<<" occured but identifier expected"<<endl;
				}
				else if( (t.tokenType()).compare("INT_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.intval()<<" occured but identifier expected"<<endl;
				}
				else if( (t.tokenType()).compare("STRING_CONST")==0 ){
					cout<<t.tokenType()<<" "<<t.stringval()<<" occured but identifier expected"<<endl;
				}
				else if( (t.tokenType()).compare("KEYWORD")==0 ){
					cout<<t.tokenType()<<" "<<t.keyWord()<<" occured but identifier expected"<<endl;
				}
				t.advance();
			}
			return;
		}
			
		void compileClass(int n){
			printntabs(n);
			xml<<"<class>"<<endl;
			eatkeyWord(n+1);
			classname=t.identifier();
			eatidentifier(n+1);
			eatsymbol(n+1);
			nfields=0;
			while( (t.tokenType()).compare("KEYWORD")==0 && ( (t.keyWord()).compare("static")==0 || (t.keyWord().compare("field"))==0 ) ){
				compileClassVarDec(n+1);
			}
			while( (t.tokenType()).compare("KEYWORD")==0 && ( (t.keyWord()).compare("constructor")==0 || (t.keyWord()).compare("method")==0 || (t.keyWord().compare("function"))==0 ) ){
				s.startSubroutine();
				compileSubroutineDec(n+1);
			}
			eatsymbol(n+1);
			if( (t.tokenType()).compare("no_token")==0 ){
				cout<<"Compilation completed"<<endl;
				printntabs(n);
				xml<<"</class>"<<endl;
				return;
			}
			else{
				cout<<"Compilation incomplete"<<endl;
				cout<<t.tokenType()<<"   "<<endl;
				return;
			}
		}
		
		void compileClassVarDec(int n){
			string classVartype, classVarkind;
			printntabs(n);
			xml<<"<classVarDec>"<<endl;
			classVarkind=t.keyWord();
			eatkeyWord(n+1);
			if( (t.tokenType()).compare("KEYWORD")==0 ){
				classVartype=t.keyWord();
				eatkeyWord(n+1);
			}
			else if( (t.tokenType()).compare("IDENTIFIER")==0){
				classVartype=t.identifier();
				eatidentifier(n+1);
			}
			s.define(t.identifier(), classVartype, classVarkind);
			if(classVarkind.compare("field")==0 ){
				nfields++;
			}
			eatidentifier(n+1);
			while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0 ){
				eatsymbol(n+1);
				s.define(t.identifier(), classVartype, classVarkind);
				if(classVarkind.compare("field")==0 ){
					nfields++;
				}
				eatidentifier(n+1);
			}
			eatsymbol(n+1);
			printntabs(n);
			xml<<"</classVarDec>"<<endl;
			return;
		}
		
		void compileSubroutineDec(int n){
			printntabs(n);
			xml<<"<subroutineDec>"<<endl;
			subroutine=t.keyWord();
			eatkeyWord(n+1);
			if( (t.tokenType()).compare("KEYWORD")==0 ){
				subrouttype=t.keyWord();
				eatkeyWord(n+1);
			}
			else if( (t.tokenType()).compare("IDENTIFIER")==0){
				subrouttype=t.identifier();
				eatidentifier(n+1);
			}
			subroutname=t.identifier();
			eatidentifier(n+1);
			eatsymbol(n+1);
			compileParameterList(n+1);//this will update the arg variables of the function.
			eatsymbol(n+1);
			flag=false;
			subroutnlocals=0;
			compileSubroutineBody(n+1); //for compiling variable declarations only and thus knowing subroutnlocals.
			v.writeFunction(classname, subroutname, subroutnlocals);
			if(subroutine.compare("constructor")==0){
				v.writePush("constant" ,nfields);
				v.writeCall("Memory", "alloc", 1);
				v.writePop("pointer", 0);
			}
			else if(subroutine.compare("method")==0){
				v.writePush("argument", 0);
				v.writePop("pointer", 0);
			}
			else if(subroutine.compare("function")==0){
				//nothing to do
			}
			if(flag==true){
				compileSubroutineBody(n+1);//This should not generate same labels again in the parse tree but continue from where it has left.
			}
			printntabs(n);
			xml<<"</subroutineDec>"<<endl;
			return;
		}
		
		void compileParameterList(int n){
			string argtype;
			printntabs(n);
			xml<<"<parameterList>"<<endl;
			if(subroutine.compare("method")==0){
				s.define("this", classname, "arg");
			}
			if( t.tokenType().compare("KEYWORD")==0 || t.tokenType().compare("IDENTIFIER")==0 ){
				if(t.tokenType().compare("KEYWORD")==0){
					argtype=t.keyWord();
					eatkeyWord(n+1);
				}
				else if(t.tokenType().compare("IDENTIFIER")==0){
					argtype=t.identifier();
					eatidentifier(n+1);
				}
				s.define(t.identifier(), argtype, "arg");
				eatidentifier(n+1);
				while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0 ){
					eatsymbol(n+1);
					if( (t.tokenType()).compare("KEYWORD")==0){
						argtype=t.keyWord();
						eatkeyWord(n+1);
					}
					else if( (t.tokenType()).compare("IDENTIFIER")==0){
						argtype=t.identifier();
						eatidentifier(n+1);
					}
					s.define(t.identifier(), argtype, "arg");
					eatidentifier(n+1);	
				}
			}
			printntabs(n);
			xml<<"</parameterList>"<<endl;
			return;
		}
		
		void compileSubroutineBody(int n){
			if(flag==false){
				printntabs(n);
				xml<<"<subroutineBody>"<<endl;
				eatsymbol(n+1);
				while( (t.tokenType()).compare("KEYWORD")==0 && (t.keyWord()).compare("var")==0 ){
					compileVarDec(n+1);
				}
				flag=true;
				return;
			}
			else{
				whilecount=0;
				ifcount=0;
				compileStatements(n+1);
				eatsymbol(n+1);
				printntabs(n);
				xml<<"</subroutineBody>"<<endl;
				return;
			}
		}
		
		void compileVarDec(int n){
			string vartype;
			printntabs(n);
			xml<<"<varDec>"<<endl;
			eatkeyWord(n+1);
			if( (t.tokenType()).compare("KEYWORD")==0){
				vartype=t.keyWord();
				eatkeyWord(n+1);
			}
			else if(t.tokenType().compare("IDENTIFIER")==0){
				vartype=t.identifier();
				eatidentifier(n+1);
			}
			s.define(t.identifier(), vartype, "var");
			subroutnlocals++;
			eatidentifier(n+1);	
			while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0 ){
				eatsymbol(n+1);
				s.define(t.identifier(), vartype, "var");
				subroutnlocals++;
				eatidentifier(n+1);
			}
			eatsymbol(n+1);
			printntabs(n);
			xml<<"</varDec>"<<endl;
			return;
		}
		
		void compileStatements(int n){
			printntabs(n);
			xml<<"<statements>"<<endl;
			while( (t.tokenType()).compare("KEYWORD")==0 && ( (t.keyWord()).compare("let")==0 || (t.keyWord()).compare("if")==0 || (t.keyWord()).compare("while")==0 || (t.keyWord()).compare("do")==0 || (t.keyWord()).compare("return")==0 ) ){
				if( (t.keyWord()).compare("let")==0 ){
					compileLet(n+1);
				}
				else if( (t.keyWord()).compare("if")==0 ){
					ifcount++;
					compileIf(n+1);
				}
				else if( (t.keyWord()).compare("while")==0 ){
					whilecount++;
					compileWhile(n+1);
				}
				else if( (t.keyWord()).compare("do")==0 ){
					compileDo(n+1);
				}
				else if( (t.keyWord()).compare("return")==0 ){
					compileReturn(n+1);
				}
			}
			printntabs(n);
			xml<<"</statements>"<<endl;
			return;
		}
		
		void compileLet(int n){
			string varname, varkind;
			int varindex;
			printntabs(n);
			xml<<"<letStatement>"<<endl;
			eatkeyWord(n+1);
			varname=t.identifier();
			varkind=s.kindof(varname);
			varindex=s.indexof(varname);
			eatidentifier(n+1);
			if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("[")==0 ){
				eatsymbol(n+1);
				v.writePush(varkind, varindex);
				compileExpression(n+1);
				v.writeArithmetic("add");
				eatsymbol(n+1);
				eatsymbol(n+1);
				compileExpression(n+1);
				v.writePop("temp", 0); //Whenever temp variable is used it should be kept track of until it is no longer necessary. So better use it and throw it immediately and not store values in it.
				v.writePop("pointer", 1);
				v.writePush("temp", 0);
				v.writePop("that", 0);
				eatsymbol(n+1);
			}
			else{
				eatsymbol(n+1);
				compileExpression(n+1);
				v.writePop(varkind, varindex);
				eatsymbol(n+1);
			}
			printntabs(n);
			xml<<"</letStatement>"<<endl;
			return;
		}
		
		void compileIf(int n){
			int if_count=ifcount;
			printntabs(n);
			xml<<"<ifStatement>"<<endl;
			eatkeyWord(n+1);
			eatsymbol(n+1);
			compileExpression(n+1);
			v.writeArithmetic("not");
			v.writeIf(classname + "." + subroutname + "_iffalse", if_count);
			eatsymbol(n+1);
			eatsymbol(n+1);
			compileStatements(n+1);
			eatsymbol(n+1);
			v.writeGoto(classname + "." + subroutname + "_iftrue", if_count);
			v.writeLabel(classname + "." + subroutname + "_iffalse", if_count);
			if( (t.tokenType()).compare("KEYWORD")==0 && (t.keyWord()).compare("else")==0 ){
				eatkeyWord(n+1);
				eatsymbol(n+1);
				compileStatements(n+1);
				eatsymbol(n+1);
			}
			v.writeLabel(classname + "." + subroutname + "_iftrue", if_count);
			printntabs(n);
			xml<<"</ifStatement>"<<endl;
			return;
		}
		
		void compileWhile(int n){
			int while_count=whilecount;
			printntabs(n);
			xml<<"<whileStatement>"<<endl;
			eatkeyWord(n+1);
			eatsymbol(n+1);
			v.writeLabel(classname + "." + subroutname + "_whiletrue", while_count);
			compileExpression(n+1);
			v.writeArithmetic("not");
			eatsymbol(n+1);
			v.writeIf(classname + "." + subroutname + "_whilefalse", while_count);
			eatsymbol(n+1);
			compileStatements(n+1);
			v.writeGoto(classname + "." + subroutname + "_whiletrue", while_count);
			eatsymbol(n+1);
			v.writeLabel(classname + "." + subroutname + "_whilefalse", while_count);
			printntabs(n);
			xml<<"</whileStatement>"<<endl;
			return;
		}
		
		void compileDo(int n){
			int nargs;
			string cl_sr_var;
			string cl_var_kind;
			string func_cons_meth;
			printntabs(n);
			xml<<"<doStatement>"<<endl;
			eatkeyWord(n+1);
			cl_sr_var=t.identifier();
			eatidentifier(n+1);
			if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("(")==0 ){//Current class' method.
				eatsymbol(n+1);
				v.writePush("pointer", 0);
				nargs=compileExpressionList(n+1);
				v.writeCall(classname, cl_sr_var, nargs+1);
				v.writePop("temp", 0);/////////////////////
				eatsymbol(n+1);
				eatsymbol(n+1);
			}
			else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(".")==0 ){
				eatsymbol(n+1);
				cl_var_kind=s.kindof(cl_sr_var);
				if(cl_var_kind.compare("NONE")==0){//it is a class' constructor or a function.
					func_cons_meth=t.identifier();
					eatidentifier(n+1);
					eatsymbol(n+1);
					nargs=compileExpressionList(n+1);
					v.writeCall(cl_sr_var, func_cons_meth, nargs);
					v.writePop("temp", 0);
					eatsymbol(n+1);
					eatsymbol(n+1);
				}
				else{//Another class' method.
					v.writePush(s.kindof(cl_sr_var), s.indexof(cl_sr_var));
					func_cons_meth=t.identifier();
					eatidentifier(n+1);
					eatsymbol(n+1);
					nargs=compileExpressionList(n+1);
					eatsymbol(n+1);
					v.writeCall(s.type_of(cl_sr_var), func_cons_meth, nargs+1);
					eatsymbol(n+1);
				}
			}
			printntabs(n);
			xml<<"</doStatement>"<<endl;
			return;
		}
		
		void compileReturn(int n){
			printntabs(n);
			xml<<"<returnStatement>"<<endl;
			eatkeyWord(n+1);
			if( (t.tokenType()).compare("SYMBOL")!=0 || (t.symbol()).compare(";")!=0 ){
				compileExpression(n+1);
				v.writeReturn();
			}
			else{
				v.writePush("constant", 0);
				v.writeReturn();
			}
			eatsymbol(n+1);
			printntabs(n);
			xml<<"</returnStatement>"<<endl;
			return;
		}
		
		void compileExpression(int n){
			printntabs(n);
			xml<<"<expression>"<<endl;
			compileTerm(n+1);
			while( (t.tokenType()).compare("SYMBOL")==0 && ( (t.symbol()).compare("+")==0 || (t.symbol()).compare("-")==0 || (t.symbol()).compare("*")==0 || (t.symbol()).compare("/")==0 || (t.symbol()).compare("&amp;")==0 || (t.symbol()).compare("|")==0 || (t.symbol()).compare("&lt;")==0 || (t.symbol()).compare("&gt;")==0 || (t.symbol()).compare("=")==0 ) ){
				if((t.symbol()).compare("+")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("add");
				}
				else if((t.symbol()).compare("-")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("sub");
				}
				else if((t.symbol()).compare("*")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeCall("Math", "multiply", 2);
				}
				else if((t.symbol()).compare("/")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeCall("Math", "divide", 2);
				}
				else if((t.symbol()).compare("&amp;")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("and");
				}
				else if((t.symbol()).compare("|")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("or");
				}
				else if((t.symbol()).compare("&lt;")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("lt");
				}
				else if((t.symbol()).compare("&gt;")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("gt");
				}
				else if((t.symbol()).compare("=")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("eq");
				}
			}
			printntabs(n);
			xml<<"</expression>"<<endl;
			return;
		}
		
		void compileTerm(int n){
			string cl_sr_var, cons_meth_func;
			int nargs;
			printntabs(n);
			xml<<"<term>"<<endl;
			if( (t.tokenType()).compare("INT_CONST")==0 ){
				v.writePush("constant", t.intval());
				eatintval(n+1);
			}
			else if( (t.tokenType()).compare("STRING_CONST")==0 ){
				v.writePush("constant", t.stringval().length() ); /////////////////////
				v.writeCall("String", "new", 1);
				int i=0;
				while(i<(t.stringval()).length()){
					v.writePush("constant", int( t.stringval().at(i)) );
					v.writeCall("String", "appendChar", 2);
					i++;
				}
				eatstringval(n+1);
			}
			else if( (t.tokenType()).compare("KEYWORD")==0 ){
				if(t.keyWord().compare("true")==0){
					v.writePush("constant", 0);
					v.writeArithmetic("not");
				}
				else if(t.keyWord().compare("false")==0){
					v.writePush("constant", 0);
				}
				else if(t.keyWord().compare("null")==0){
					v.writePush("constant", 0);
				}
				else if(t.keyWord().compare("this")==0){
					v.writePush("pointer", 0);
				}
				eatkeyWord(n+1);
			}
			else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("(")==0 ){
				eatsymbol(n+1);
				compileExpression(n+1);
				eatsymbol(n+1);
			}
			else if( (t.tokenType()).compare("SYMBOL")==0 && ( (t.symbol()).compare("-")==0 || (t.symbol()).compare("~")==0 ) ){
				if(t.symbol().compare("-")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("neg");
				}
				else if(t.symbol().compare("~")==0){
					eatsymbol(n+1);
					compileTerm(n+1);
					v.writeArithmetic("not");
				}
			}
			else if( (t.tokenType()).compare("IDENTIFIER")==0 ){
				cl_sr_var=t.identifier();
				eatidentifier(n+1);
				//varname[expression]
				if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("[")==0 ){
					eatsymbol(n+1);
					v.writePush(s.kindof(cl_sr_var), s.indexof(cl_sr_var));
					compileExpression(n+1);
					v.writeArithmetic("add");
					v.writePop("pointer", 1);
					v.writePush("that", 0);
					eatsymbol(n+1);
				}
				//subroutinecall
				else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("(")==0 ){//this class method
					eatsymbol(n+1);
					v.writePush("pointer", 0);
					nargs=compileExpressionList(n+1);
					v.writeCall(classname, cl_sr_var, nargs+1);
					eatsymbol(n+1);
				}
				else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(".")==0 ){
					if(s.kindof(cl_sr_var).compare("NONE")==0){//some class function or constructor
						eatsymbol(n+1);
						cons_meth_func=t.identifier();
						eatidentifier(n+1);
						eatsymbol(n+1);
						nargs=compileExpressionList(n+1);
						v.writeCall(cl_sr_var, cons_meth_func, nargs);
						eatsymbol(n+1);
					}
					else{//some object's method
						v.writePush(s.kindof(cl_sr_var), s.indexof(cl_sr_var));
						eatsymbol(n+1);
						cons_meth_func=t.identifier();
						eatidentifier(n+1);
						eatsymbol(n+1);
						nargs=compileExpressionList(n+1);
						v.writeCall(s.type_of(cl_sr_var), cons_meth_func, nargs+1);
						eatsymbol(n+1);
					}
				}
				//varname
				else if( (t.tokenType()).compare("SYMBOL")==0 && ( (t.symbol()).compare(";")==0 || (t.symbol()).compare("]")==0 || (t.symbol()).compare(")")==0 || (t.symbol()).compare(",")==0 || (t.symbol()).compare("+")==0 || (t.symbol()).compare("-")==0 || (t.symbol()).compare("*")==0 || (t.symbol()).compare("/")==0 || (t.symbol()).compare("&amp;")==0 || (t.symbol()).compare("|")==0 || (t.symbol()).compare("&lt;")==0 || (t.symbol()).compare("&gt;")==0 || (t.symbol()).compare("=")==0 ) ){
					v.writePush(s.kindof(cl_sr_var), s.indexof(cl_sr_var));
					printntabs(n);
					xml<<"</term>"<<endl;
					return;
				}
			}
			printntabs(n);
			xml<<"</term>"<<endl;
			return;
		}
		
		int compileExpressionList(int n){
			int nexps=0;
			printntabs(n);
			xml<<"<expressionList>"<<endl;
			if( (t.tokenType()).compare("SYMBOL")!=0 || (t.symbol()).compare(")")!=0 ){
				compileExpression(n+1);
				nexps++;
				while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0  ){
					eatsymbol(n+1);
					compileExpression(n+1);
					nexps++;
				}
			}
			printntabs(n);
			xml<<"</expressionList>"<<endl;
			return nexps;
		}
		
};

int main(int argc, char **argv){
	string path,s;
	path=argv[1];
	s=path;
	if(s.length()>5){
		if(s.compare(s.length()-5, 5, ".jack")==0){
			CompilationEngine c(s);
			c.compileClass(0);
			return 0;
		}
	}
	//else{
		DIR *d;
		struct dirent *dir;
		d=opendir(path.c_str());
		if(d){
			while((dir=readdir(d))!=NULL)
			{
				s=dir->d_name;
				if(s.length()>5)
				{
					if(s.compare((s.length())-5,5,".jack")==0)
					{
						s=path+"/"+s;      //or s=path+"\\"+s;
						CompilationEngine c(s);
						c.compileClass(0);
					}
				}
			}
		}
	//}
	return 0;
}
