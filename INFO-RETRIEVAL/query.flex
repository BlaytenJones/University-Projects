%{
#include <iostream>
#include <sstream>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "hashtable.h"
#include "invertedFile.h"
#include <time.h>
extern int yylex(void);
using namespace std;
using std::tolower;

string tokenizedQuery[1000];
struct numDocsAndStart {
	int numDocs = 0;
	int start = 0;
};
numDocsAndStart queryInfo[1000]; 
int currToken = 0;
int RESULTSIZE = 10;

char* Downcase(const char* input){
        char* result = (char*)malloc(strlen(input) + 1);
        if(result == NULL){
                return NULL;
        }
        int i = 0;
        while(input[i] != '\0'){
                if (isalpha(input[i])) {
                        result[i] = tolower(input[i]);
                } else {
                        result[i] = input[i]; //Copy non-alphabetic characters as is
                }
                i++;
        }
        result[i] = '\0'; // Null-terminate the result string
        return result;
}

%}

WHITESPACE [\t\r\n\v ]+
WORD [a-z]+
UPPERWORD ([0-9]*[a-zA-Z]+[0-9]*)+
PHONENUM ((\+)?([0-9]-))?[0-9]{3}-[0-9]{3}-[0-9]{4}
NUM [0-9]+(,[0-9]+)*
TIME [0-9]{1,2}(:[0-9]{2})+
VER [0-9]+\.[0-9]+(\.[0-9]+)+
EMAIL [a-zA-Z~!$%^&*_=+}{'?\-\.0-9]+@[a-zA-Z\~\!\$\%\^\&\*\_\=\+\}\{\'\?\-0-9]+(\.[a-zA-Z]+)+
PUNCTUATION [./\\#;:\'\"|`~!$\->@<^?}{\[\]%&*)(+=,_]+
URL (([hH][tT][tT][pP])|([fF][tT][pP]))s?:\/\/[A-Za-z0-9]+([\-\.]{1}[A-Za-z0-9]+)*\.[A-Za-z0-9]{2,}(:[0-9]{1,})?(\/[A-Za-z0-9_~\.\-]*)*
FLOAT [0-9]*\.[0-9]+
SPIDER (Date:[ ](.|\n)*Content-length:[ ][0-9]*)
ATTR <!?[A-Za-z0-9]+([ \n\t]+(([A-Za-z\-_]+)?[ \n\t]*=?[ \n\t]*((\"[^\"]*\")|([A-Za-z0-9]+)|({URL}))[ \n\t]*)+[ \n\t]*)*[\/]?>
OPENTAG <!?[a-zA-Z0-9]+{ATTR}*[\/]?>
ENDTAG <[\/][a-zA-Z0-9]+>

%%
\{\*[0-9]+\} {
	string numOnly = "";
	for(unsigned int i = 0; i < strlen(yytext) + 1; i++){
		if(isdigit(yytext[i])){
			numOnly += yytext[i];
		}
	}
	RESULTSIZE = stoi(numOnly);
}

{SPIDER} ;

{EMAIL}|{URL} {char* lowered = Downcase(yytext); if (lowered != NULL){ tokenizedQuery[currToken] = lowered; currToken++; free(lowered);}}

{PHONENUM} {tokenizedQuery[currToken] = yytext; currToken++;}

{FLOAT} ;

{NUM}|{TIME}|{VER} {tokenizedQuery[currToken] = yytext; currToken++;}

{OPENTAG}|{ENDTAG} ;

{WHITESPACE} ;

{WORD} {tokenizedQuery[currToken] = yytext; currToken++;}

{UPPERWORD} {char* lowered = Downcase(yytext); if (lowered != NULL){ tokenizedQuery[currToken] = lowered; currToken++; free(lowered);}}

. ;

%%

int main(int argc, char **argv){
	clock_t start, end;
        start = clock();
	string query = "";
	for(int i = 1; i < argc; i++){
		string tmpToken = argv[i];
		query += tmpToken.substr(0, 34);
		query += " ";
	}
	yyin = fmemopen((void*)query.c_str(), strlen(query.c_str()), "r");
	int token;
    	while ((token = yylex()) != 0) {
        	// Handle the tokens as needed
        	printf("Token: %d\n", token);
    	}
	int configValue = 0;
	FILE* file = fopen("config.txt", "r");
    	if (file == NULL) {
        	perror("Error opening file");
        	return 1;
    	}
	//get first line out of the way
	fscanf(file, "%d", &configValue);
	if (fscanf(file, "%d", &configValue) != 1) {
        	perror("Error reading integer from file");
        	fclose(file);
        	return 1;
    	}
	fclose(file);
	InvertedFile IV;
	IV.openForRead();
	int currNumDocs; int currStart; int htSize = 0;
	for(int i = 0; i < currToken; i++){
		cout << i << ") " << tokenizedQuery[i] << endl;
		IV.findDictRecord(tokenizedQuery[i], configValue, currNumDocs, currStart);
		queryInfo[i].numDocs = currNumDocs; queryInfo[i].start = currStart;
		htSize += currNumDocs;
	}
	HashTable ht(htSize);
	for(int i = 0; i < currToken; i++){
		for(int j = 0; j < queryInfo[i].numDocs; j++){
			int docID; float weight;
			IV.readPostRecord(queryInfo[i].start + j, docID, weight);
			ht.Insert(to_string(docID), weight, docID);
		}
	}
	struct docInfo{
        	string name = "[]";
        	float val = 0;
		int ID = 0;
	};
	docInfo results[RESULTSIZE];
	int accessed[RESULTSIZE];
	for(int i = 0; i < RESULTSIZE; i++){
		accessed[i] = -1;
	}
	bool resultsFound = false;
	for(int i = 0; i < RESULTSIZE; i++){	
		float currMax = 0;
		int maxIndex = 0;
		for(int j = 0; j < 3*htSize; j++){
			float currVal = ht.Walk(j);
			if((currVal != 0) && (currVal > currMax)){
				bool hasBeenAccessed = false;
				for(int k = 0; k < i; k++){
					hasBeenAccessed |= (accessed[k]	== j);
				}
				if(!hasBeenAccessed){
					currMax = currVal;
					maxIndex = j;
				}
			}
		}
		results[i].val = currMax;
		accessed[i] = maxIndex;
		//sets mode to return docID from hashtable
                results[i].ID = ht.Walk(maxIndex, 1);
		string fileName; int throwaway;
		IV.readMapRecord(results[i].ID, throwaway, fileName);
		results[i].name = fileName;
		resultsFound |= (currMax != 0);
	}
	int numResults = 0;
	if(resultsFound){	
		cout << "\nRANK~~~DOCID~~~DOC_NAME~~~~~~~~~~~~~WEIGHT~~~\n\n";
		for(int i = 0; i < RESULTSIZE; i++){
			if(results[i].val != 0){
				numResults++;
				cout << setw(6) << left << (to_string(i+1) + ")") << " " << setw(7) << results[i].ID << " " << setw(20) << results[i].name << " " << setw(24) << 20*results[i].val << endl << endl;
			}
		}
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	}else{
		cout << "\nUNFORTUNATELY NO RESULTS COULD BE FOUND! TRY A DIFFERENT QUERY\n";
	}
	end = clock();
        double totalTime = double(end - start)/double(CLOCKS_PER_SEC);
	cout << "RETURNED " << numResults << " DIFFERENT RESULTS IN " << totalTime << " SECONDS!\n";
	return 0;
}
