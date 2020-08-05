#include<iostream>
#include<cstring>
#include<string>
#include<fstream>
#include<algorithm>
#include<dirent.h>
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


class CompilationEngine{
	private:
		ofstream xml;
		tokenizer t;
	public:
		CompilationEngine(string path): t(path){
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
			eatidentifier(n+1);
			eatsymbol(n+1);
			while( (t.tokenType()).compare("KEYWORD")==0 && ( (t.keyWord()).compare("static")==0 || (t.keyWord().compare("field"))==0 ) ){
				compileClassVarDec(n+1);
			}
			while( (t.tokenType()).compare("KEYWORD")==0 && ( (t.keyWord()).compare("constructor")==0 || (t.keyWord()).compare("method")==0 || (t.keyWord().compare("function"))==0 ) ){
				compileSubroutineDec(n+1);
			}
			eatsymbol(n+1);
			if( (t.tokenType()).compare("no_token")==0 ){
				cout<<"Parsing completed"<<endl;
				printntabs(n);
				xml<<"</class>"<<endl;
				return;
			}
			else{
				cout<<"Parsing incomplete"<<endl;
				cout<<t.tokenType()<<"   "<<endl;
				return;
			}
		}
		
		void compileClassVarDec(int n){
			printntabs(n);
			xml<<"<classVarDec>"<<endl;
			eatkeyWord(n+1);
			if( (t.tokenType()).compare("KEYWORD")==0 ){               /////check with the type rule at other points
				eatkeyWord(n+1);
			}
			else if( (t.tokenType()).compare("IDENTIFIER")==0){
				eatidentifier(n+1);
			}
			eatidentifier(n+1);
			while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0 ){
				eatsymbol(n+1);
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
			eatkeyWord(n+1);
			if( (t.tokenType()).compare("KEYWORD")==0 ){
				eatkeyWord(n+1);
			}
			else if( (t.tokenType()).compare("IDENTIFIER")==0){
				eatidentifier(n+1);
			}
			eatidentifier(n+1);
			eatsymbol(n+1);
			compileParameterList(n+1);
			eatsymbol(n+1);
			compileSubroutineBody(n+1);
			printntabs(n);
			xml<<"</subroutineDec>"<<endl;
			return;
		}
		
		void compileParameterList(int n){
			printntabs(n);
			xml<<"<parameterList>"<<endl;
			if( t.tokenType().compare("KEYWORD")==0 || t.tokenType().compare("IDENTIFIER")==0 ){
				if(t.tokenType().compare("KEYWORD")==0){
					eatkeyWord(n+1);
				}
				else if(t.tokenType().compare("IDENTIFIER")==0){
					eatidentifier(n+1);
				}
				eatidentifier(n+1);
				while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0 ){
					eatsymbol(n+1);
					if( (t.tokenType()).compare("KEYWORD")==0){
						eatkeyWord(n+1);
					}
					else if( (t.tokenType()).compare("IDENTIFIER")==0){
						eatidentifier(n+1);
					}
					eatidentifier(n+1);	
				}
			}
			printntabs(n);
			xml<<"</parameterList>"<<endl;
			return;
		}
		
		void compileSubroutineBody(int n){
			printntabs(n);
			xml<<"<subroutineBody>"<<endl;
			eatsymbol(n+1);
			while( (t.tokenType()).compare("KEYWORD")==0 && (t.keyWord()).compare("var")==0 ){
				compileVarDec(n+1);
			}
			compileStatements(n+1);
			eatsymbol(n+1);
			printntabs(n);
			xml<<"</subroutineBody>"<<endl;
			return;
		}
		
		void compileVarDec(int n){
			printntabs(n);
			xml<<"<varDec>"<<endl;
			eatkeyWord(n+1);
			if( (t.tokenType()).compare("KEYWORD")==0){
				eatkeyWord(n+1);
			}
			else if(t.tokenType().compare("IDENTIFIER")==0){
				eatidentifier(n+1);
			}
			eatidentifier(n+1);	
			while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0 ){
				eatsymbol(n+1);
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
					compileIf(n+1);
				}
				else if( (t.keyWord()).compare("while")==0 ){
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
			printntabs(n);
			xml<<"<letStatement>"<<endl;
			eatkeyWord(n+1);
			eatidentifier(n+1);
			if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("[")==0 ){
				eatsymbol(n+1);
				compileExpression(n+1);
				eatsymbol(n+1);
			}
			eatsymbol(n+1);
			compileExpression(n+1);
			eatsymbol(n+1);
			printntabs(n);
			xml<<"</letStatement>"<<endl;
			return;
		}
		
		void compileIf(int n){
			printntabs(n);
			xml<<"<ifStatement>"<<endl;
			eatkeyWord(n+1);
			eatsymbol(n+1);
			compileExpression(n+1);
			eatsymbol(n+1);
			eatsymbol(n+1);
			compileStatements(n+1);
			eatsymbol(n+1);
			if( (t.tokenType()).compare("KEYWORD")==0 && (t.keyWord()).compare("else")==0 ){
				eatkeyWord(n+1);
				eatsymbol(n+1);
				compileStatements(n+1);
				eatsymbol(n+1);
			}
			printntabs(n);
			xml<<"</ifStatement>"<<endl;
			return;
		}
		
		void compileWhile(int n){
			printntabs(n);
			xml<<"<whileStatement>"<<endl;
			eatkeyWord(n+1);
			eatsymbol(n+1);
			compileExpression(n+1);
			eatsymbol(n+1);
			eatsymbol(n+1);
			compileStatements(n+1);
			eatsymbol(n+1);
			printntabs(n);
			xml<<"</whileStatement>"<<endl;
			return;
		}
		
		void compileDo(int n){
			printntabs(n);
			xml<<"<doStatement>"<<endl;
			eatkeyWord(n+1);
			eatidentifier(n+1);
			if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("(")==0 ){
				eatsymbol(n+1);
				compileExpressionList(n+1);
				eatsymbol(n+1);
				eatsymbol(n+1);
			}
			else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(".")==0 ){
				eatsymbol(n+1);
				eatidentifier(n+1);
				eatsymbol(n+1);
				compileExpressionList(n+1);
				eatsymbol(n+1);
				eatsymbol(n+1);
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
				eatsymbol(n+1);
				compileTerm(n+1);
			}
			printntabs(n);
			xml<<"</expression>"<<endl;
			return;
		}
		
		void compileTerm(int n){
			printntabs(n);
			xml<<"<term>"<<endl;
			if( (t.tokenType()).compare("INT_CONST")==0 ){
				eatintval(n+1);
			}
			else if( (t.tokenType()).compare("STRING_CONST")==0 ){
				eatstringval(n+1);
			}
			else if( (t.tokenType()).compare("KEYWORD")==0 ){      /////////
				eatkeyWord(n+1);
			}
			else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("(")==0 ){
				eatsymbol(n+1);
				compileExpression(n+1);
				eatsymbol(n+1);
			}
			else if( (t.tokenType()).compare("SYMBOL")==0 && ( (t.symbol()).compare("-")==0 || (t.symbol()).compare("~")==0 ) ){
				eatsymbol(n+1);
				compileTerm(n+1);
			}
			else if( (t.tokenType()).compare("IDENTIFIER")==0 ){
				eatidentifier(n+1);
				//varname[expression]
				if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("[")==0 ){
					eatsymbol(n+1);
					compileExpression(n+1);
					eatsymbol(n+1);
				}
				//subroutinecall
				else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare("(")==0 ){
					eatsymbol(n+1);
					compileExpressionList(n+1);
					eatsymbol(n+1);
				}
				else if( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(".")==0 ){
					eatsymbol(n+1);
					eatidentifier(n+1);
					eatsymbol(n+1);
					compileExpressionList(n+1);
					eatsymbol(n+1);
				}
				//varname
				else if( (t.tokenType()).compare("SYMBOL")==0 && ( (t.symbol()).compare(";")==0 || (t.symbol()).compare("]")==0 || (t.symbol()).compare(")")==0 || (t.symbol()).compare(",")==0 || (t.symbol()).compare("+")==0 || (t.symbol()).compare("-")==0 || (t.symbol()).compare("*")==0 || (t.symbol()).compare("/")==0 || (t.symbol()).compare("&amp;")==0 || (t.symbol()).compare("|")==0 || (t.symbol()).compare("&lt;")==0 || (t.symbol()).compare("&gt;")==0 || (t.symbol()).compare("=")==0 ) ){
					printntabs(n);
					xml<<"</term>"<<endl;
					return;
				}
			}
			printntabs(n);
			xml<<"</term>"<<endl;
			return;
		}
		
		void compileExpressionList(int n){
			printntabs(n);
			xml<<"<expressionList>"<<endl;
			if( (t.tokenType()).compare("SYMBOL")!=0 || (t.symbol()).compare(")")!=0 ){
				compileExpression(n+1);
				while( (t.tokenType()).compare("SYMBOL")==0 && (t.symbol()).compare(",")==0  ){
					eatsymbol(n+1);
					compileExpression(n+1);
				}
			}
			printntabs(n);
			xml<<"</expressionList>"<<endl;
			return;
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
