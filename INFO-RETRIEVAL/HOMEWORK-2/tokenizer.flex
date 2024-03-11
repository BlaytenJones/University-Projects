/*Tokenizer lex: tokenizes keywords from an HTML file*/

%{
#include <iostream>
#include <ctype.h>
#include <string.h>
extern int yylex(void);
using namespace std;
using std::tolower;

char Ch;

//used for non-general downcasing
char* downcase(const char* input){
        char* result = (char*)malloc(strlen(input) + 1);
	if(result == NULL){
		return NULL;
	}
	int i = 0;
	while(input[i] != '\0'){
		if (isalpha(input[i])) {
            		result[i] = tolower(input[i]);
        	} else {
            		result[i] = input[i]; // Copy non-alphabetic characters as is
        	}
        	i++;
    	}
 	result[i] = '\0'; // Null-terminate the result string
    	return result;
}

%}

WHITESPACE [ \n\t\r\v\f]+
CAPITALS [A-Z]
EMAIL [a-zA-Z~!$%^&*_=+}{'?\-.0-9]+@[a-zA-Z\~\!\$\%\^\&\*\_\=\+\}\{\'\?\-0-9]+(\.[a-z]+)+
PUNCTUATION [./\\#;:\'\"|`~!$\->@<^?}{\[\]%&*)(+=,_]+
URL ([hH]ttps?:\/\/|[wW][wW][wW]\.)[^ \n\t\r\v\f\"]*
FLOAT [0-9]*\.[0-9]+
SPIDER (Date:[ ](.|\n)*Content-length:[ ][0-9]*)
GENERALHTMLTAG <[^>]*>

%%
{SPIDER} ;

{WHITESPACE} {fprintf(yyout, "%s", "\n");}

{EMAIL}|{URL} {char* lowered = downcase(yytext); if (lowered != NULL){ fprintf(yyout, "%s", lowered); free(lowered); }}

alt=[^{PUNCTUATION}]+ {char* lowered = downcase(yytext+4); if (lowered != NULL){ fprintf(yyout, "%s", lowered); free(lowered); }} 

contents=[^{PUNCTUATION}]+ {char* lowered = downcase(yytext+9); if (lowered != NULL){ fprintf(yyout, "%s", lowered); free(lowered); }}

{GENERALHTMLTAG} ;

{FLOAT}|{PUNCTUATION} ;

{CAPITALS} {Ch = yytext[0] - 'A' + 'a'; fprintf(yyout, "%c", Ch);}

. {fprintf(yyout, "%s", yytext);}
%%

int main(int argc, char **argv){
	if(argc != 3){
		cout << "\n Incorrect number of arguments. Make sure to have input and output file. \n";
	}
	yyout = fopen(argv[2], "w");
	if((yyin = fopen(argv[1],"r")) == NULL){
		cout << "\n Error opening input file. \n";
	}
	yylex();
}
