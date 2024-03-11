/*Tokenizer lex: tokenizes keywords from an HTML file*/

%{
#include <iostream>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include "hashtable.h"
#include "invertedFile.h"
#include <time.h>
#include <bits/stdc++.h>
extern int yylex(void);
using namespace std;
using std::tolower;
#undef yywrap

char Ch;
const int SIZE = 60000;
HashTable ht(3*SIZE);
string Prevname = "";
int numOfTotalTokens = 0;
bool currNewLine = false;
HashTable ght(SIZE);
int docID = 0;
float numOfLogDocTokens[SIZE];
int numOfDocsIn[1000];
bool stopListInit = true;
const int STOPLISTSIZE = 150;
HashTable stopList(3*STOPLISTSIZE);

const int MAXFILESOPEN = 1000;
const int MAXFILESTOTAL = 100000;
//Note that MAXFILESTOTAL is much larger than MAXFILESOPEN. The current distro of Linux that Turing runs on only allows ~1,000 files
//open at the same time. Since the buffer relies on the alphabetical order of the tokens, it is necessary to switch between the first
//1000ish files of the filePtrs, then switch to the next 1000, and so on.
struct FILEANDNAME{
        FILE* fileptr = nullptr;
        string filename = "";
};
FILEANDNAME filePtrs[MAXFILESTOTAL];
struct fileInfo{
        string term = "";
        float weight = 0;
        int fileID = -1;
};
fileInfo buffer[MAXFILESOPEN][MAXFILESTOTAL/MAXFILESOPEN + 1];

//used for non-general downcasing
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

void tokenize(string text){
	int data = ht.GetData(text);
	if(data != -1){
		currNewLine = true;
	}
	if(text == "\n"){
		currNewLine = true;
	}else{
		if(text.length() > 1){
			if(stopListInit && (stopList.GetData(text) != -1)){
				return;
			}
			bool unique = false;
			data = ht.GetData(text);
        		if(data == -1){
				unique = true;
        		}
			numOfTotalTokens++;
			currNewLine = false;
			ht.Insert(text, 1, 0, (unique ? 1 : 0));
			ght.Insert(text, 1, 0, (unique ? 1 : 0));
		}
	}
}

void ProcessHashtable(){
	fprintf(yyout, "%s", (ht.Print(docID)).c_str());
	numOfLogDocTokens[docID] = ht.CalculateTotalLog();
	ht.Clear();
	docID++;
}

void openClusterFiles(size_t index){
        const char* currFilename;
        for(size_t i = index*MAXFILESOPEN; i < (index+1)*MAXFILESOPEN; i++){
                currFilename = filePtrs[i].filename.c_str();
                if(strcmp(currFilename, "") != 0){
                        filePtrs[i].fileptr = fopen(currFilename, "r");
                }
        }
}

void closeClusterFiles(size_t index){
	cout << "D" << endl;
        const char* currFilename;
        for(size_t i = index*MAXFILESOPEN; i < (index+1)*MAXFILESOPEN; i++){
		cout << i << " " << filePtrs[i].filename << " " << filePtrs[i].fileptr << endl;
		currFilename = filePtrs[i].filename.c_str();
		cout << "A" << endl;
                if(strcmp(currFilename, "") != 0){
                        cout << "B: " << filePtrs[i].fileptr << " " << (filePtrs[i].fileptr != nullptr) << endl;
			if (ferror(filePtrs[i].fileptr)){
				fprintf(stderr, "Error occurred while reading file: %s\n", strerror(errno));
			}
			fclose(filePtrs[i].fileptr);
			cout << "C" << endl;
                }
        }
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
{SPIDER} ;

{EMAIL}|{URL} {char* lowered = Downcase(yytext); if (lowered != NULL){ tokenize(lowered); free(lowered); }}

{PHONENUM} {tokenize(yytext);}

{FLOAT} ;

{NUM}|{TIME}|{VER} {tokenize(yytext);}

{OPENTAG}|{ENDTAG} ;

{WHITESPACE} {tokenize("\n");}

{WORD} {tokenize(yytext);}

{UPPERWORD} {char* lowered = Downcase(yytext); if (lowered != NULL){ tokenize(lowered); free(lowered); }}

. ;
%%

//global variables for the input directory pointer
//and output directory name for use in yywrap
DIR *InputDirPtr = NULL;
string InDirname;
string OutDirname;
string OutFilename;
string InFilename;
bool OutputFileIsOpen = false;
InvertedFile IV;

// This is called once per file.
int yywrap(){
	struct dirent* InputDirEntryPtr;

	//if there was an input file open 
	//close it to get ready for the next file 
	if (yyin != NULL){
      		fclose(yyin);
      		yyin = NULL;
      		//Just closed the file
		ProcessHashtable();
   	}

   	//if there was an output file open
   	//close it to get ready for the next file 
   	if (OutputFileIsOpen){
		fclose(yyout);
      		OutputFileIsOpen = false;
   	}

   	//skip over the hidden filenames that begin with dot
   	do{
      		InputDirEntryPtr = readdir(InputDirPtr);
   	} while ((InputDirEntryPtr != NULL) && (InputDirEntryPtr->d_name[0] == '.'));

   	//if there are still files to process
   	if(InputDirEntryPtr != NULL){
		//open the next file in the list as yyin
      		InFilename = InDirname + '/'+ InputDirEntryPtr->d_name;
		if(docID <= MAXFILESTOTAL - 1){
			yyin = fopen(InFilename.c_str(), "r");
		
      			//if file open failed, print an error
      			if(yyin == NULL){ 
         			perror(InFilename.c_str());
			}

			//Only needed if doing Multiway Merge (creates temporary token files)

      			//open a matching output file and set it to yyout
      			OutFilename = OutDirname + '/' + InputDirEntryPtr->d_name;
			yyout = fopen(OutFilename.c_str(), "w");	
      			// if file open failed, print an error
      			if(yyout == NULL){ 
         			perror(OutFilename.c_str());
      			}else{
          			OutputFileIsOpen = true;
   			}
		}
	}

	//if yyin is NULL, return 1, else return 0
   	return (yyin == NULL);
}

int main(int argc, char **argv){
	clock_t start, end;
	start = clock();
        FILE* stopFile = fopen("stopList.txt", "r");
        if (stopFile == nullptr) {
        	perror("Error opening stopList file! Stop words will not be considered!");
        	stopListInit = false;
    	}
	if(stopListInit){
		char buffer[125];
		while (fgets(buffer, sizeof(buffer), stopFile) != nullptr) {
			size_t length = strcspn(buffer, "\n");
        		buffer[length] = '\0';
			stopList.Insert(buffer, 1);
    		}
		fclose(stopFile);
	}
	DIR *OutputDirPtr = NULL;
	if(argc != 3){
		cout << "\n Incorrect number of arguments. Make sure to have input and output file. \n";
		return(1);
	}
	
	InDirname = argv[1];
	OutDirname = argv[2];
	InputDirPtr = opendir(InDirname.c_str());
	
	if (!InputDirPtr){
		cerr << "Unable to open input directory: " << InDirname << "\n";
	}else{
		//open or create output dir
		OutputDirPtr = opendir(OutDirname.c_str());
		if(!OutputDirPtr){
			string cmd = "mkdir -p " + OutDirname;
			system(cmd.c_str());
			OutputDirPtr = opendir(OutDirname.c_str());
		}
		
		//init input and output file ptrs
		yyin = NULL;
		yyout = NULL;

		//process files
		yywrap();
		yylex();

		//close input and output dir
		(void) closedir(InputDirPtr);

		struct dirent* OutputEntryPtr = readdir(OutputDirPtr);
		IV.openForWrite();

		unsigned int numFiles = 0;
		while(OutputEntryPtr != NULL && numFiles < MAXFILESTOTAL){
			if(OutputEntryPtr->d_name[0] != '.'){
				string filename = OutDirname + '/' + OutputEntryPtr->d_name;
				filePtrs[numFiles].filename = filename.c_str();
				IV.writeMapRecord(numFiles, OutputEntryPtr->d_name);
				numFiles = (numFiles >= MAXFILESTOTAL) ? MAXFILESTOTAL : numFiles + 1;
			}
			OutputEntryPtr = readdir(OutputDirPtr);
		}
		openClusterFiles(0);
		bool finished = false;

		string currTerm = "";
                string minTerm = "";
                int currFreq = 0;
                for(size_t i = 0; i < numFiles/MAXFILESOPEN + 1; i++){
			if(i != 0){
				closeClusterFiles(i-1);
				openClusterFiles(i);
			}
			for(size_t j = 0; j < MAXFILESOPEN; j++){
				int currID = 0;
                        	char tmp[256];
                        	if(buffer[j][i].term == ""){
					if(i*MAXFILESOPEN+j < numFiles && !feof(filePtrs[i*MAXFILESOPEN+j].fileptr)){
						fscanf(filePtrs[i*MAXFILESOPEN+j].fileptr, "%s %d %d", tmp, &currID, &currFreq);
                                		int cluster = currID/MAXFILESOPEN; int position = currID%MAXFILESOPEN;
						currTerm = tmp;
						//cout << "buffer[" << position << "][" << cluster << "]: " << currTerm << endl;
                                		buffer[position][cluster].term = currTerm;
						buffer[position][cluster].weight = (1 + log(currFreq))/numOfLogDocTokens[currID];
						buffer[position][cluster].fileID = i;
					}else{
                                		buffer[j][i].term = ""; buffer[j][i].weight = 0;
                                	}
                        	}else{
					int cluster = currID/MAXFILESOPEN; int position = currID%MAXFILESOPEN;
					currTerm = buffer[position][cluster].term;
                        	}
			}
                }	
		while (!finished) {
			cout << "C" << endl;
			minTerm = "";

		    	for(size_t i = 0; i < numFiles/MAXFILESOPEN + 1; i++){
				for(size_t j = 0; j < MAXFILESOPEN; j++){
					//cout << "buffer[" << j << "][" << i << "].term " << buffer[j][i].term << endl;
					if (buffer[j][i].term != "" && (minTerm == "" || buffer[j][i].term < minTerm)) {
			    			cout << j << " " << i << " " << buffer[j][i].term;
						minTerm = buffer[j][i].term;
					}
		    		}
			}

		    	if (minTerm == ""){
				finished = true;
		    	}else{
				int totalFreq = 0;
				bool emptyFlag = true;
				for(size_t i = 0; i < numFiles/MAXFILESOPEN + 1; i++){
					for(size_t j = 0; j < MAXFILESOPEN; j++){
                                        	if((minTerm != "") && (buffer[j][i].term == minTerm) && (!feof(filePtrs[buffer[j][i].fileID].fileptr))){
							totalFreq += 1;
						}
					}
				}

				for(size_t i = 0; i < numFiles/MAXFILESOPEN + 1; i++){
					if(i != 0){
                                		cout << "A: " << i << endl;
						closeClusterFiles(i-1);
						cout << "B" << endl;
                                		openClusterFiles(i);
						cout << "C" << endl;
                        		}
					for(size_t j = 0; j < MAXFILESOPEN; j++){
						fileInfo currFile = buffer[j][i];
						cout << i << " " << j << " " << currFile.term << endl;
						if ((minTerm != "") && (buffer[j][i].term == minTerm) && (!feof(filePtrs[currFile.fileID].fileptr))) {
							cout << currFile.term << " " << currFile.weight << endl;
							IV.writePostRecord(i*MAXFILESOPEN + j, (1 + log(numFiles/totalFreq))*currFile.weight);
							char tmp[256];
							int currID;

							if (i*MAXFILESOPEN + j < numFiles && !feof(filePtrs[currFile.fileID].fileptr)) {
								fscanf(filePtrs[currFile.fileID].fileptr, "%s %d %d", tmp, &currID, &currFreq);
				    				currTerm = tmp;
				    				buffer[j][i].term = currTerm; buffer[j][i].weight = (1 + log(currFreq))/numOfLogDocTokens[currID];
								//cout << tmp << ": " << setw(12) << numOfLogDocTokens[currID] << "; " << setw(15) << buffer[currID].weight << endl;
								emptyFlag = false;
							}else{
				    				buffer[j][i].term = "";
				    				buffer[j][i].weight = 0;
							}
			    			}else if((i*MAXFILESOPEN + j < numFiles && feof(filePtrs[currFile.fileID].fileptr))){
							buffer[j][i].term = "";
                                		}
					}
				}
				if(!emptyFlag){
					if(ght.UpdateStart(minTerm, start)){
						start += totalFreq;
					}
				}
			}
		}	

		ght.PrintDict(IV, true);
		IV.closeAfterWriting();
		(void) closedir(OutputDirPtr);
	}

	end = clock();
	double totalTime = double(end - start)/double(CLOCKS_PER_SEC);
	cout << "TOOK " << totalTime << " SECONDS";
	//FILE* config = fopen("config.txt", "w");
	//fprintf(config, "%d\n", 3*SIZE);
	//fclose(config);
	//stopList.Print("TESTINGFILE.txt");
	
	cout << "\nTOTAL TOKENS: " << numOfTotalTokens << "\n";
	cout << "Done tokenizing. Good place to write the dict and post files.\n";
}
