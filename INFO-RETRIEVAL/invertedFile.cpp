#include "invertedFile.h"

InvertedFile::InvertedFile():mapRecordSize(29), dictRecordSize(51), postRecordSize(16), numMapRecords(-1), numDictRecords(-1), numPostRecords(-1), configFilename("config.txt"){
}

InvertedFile::~InvertedFile() {
}

void InvertedFile::openForWrite() {
	mapFile.open("mapFile.txt", fstream::out | fstream::app);
	dictFile.open("dictFile.txt", fstream::out | fstream::app);
	postFile.open("postFile.txt", fstream::out | fstream::app);
	numMapRecords = numDictRecords = numPostRecords = 0;
}

void InvertedFile::openForRead() {
	mapFile.open("mapFile.txt", fstream::in);
        dictFile.open("dictFile.txt", fstream::in);
        postFile.open("postFile.txt", fstream::in);
	fstream tmpConfig;
	tmpConfig.open(configFilename, fstream::in);
	string numMapTmp, numDictTmp, numPostTmp;
	tmpConfig >> numMapTmp >> numDictTmp >> numPostTmp;
	numMapRecords = stoi(numMapTmp);
	numDictRecords = stoi(numDictTmp);
	numPostRecords = stoi(numPostTmp);
	tmpConfig.close();
}

void InvertedFile::closeAfterWriting() {
	mapFile.close();
	dictFile.close();
	postFile.close();
	fstream tmpConfig;
	tmpConfig.open(configFilename, fstream::out);
	tmpConfig << numMapRecords << " " << numDictRecords << " " << numPostRecords;
	tmpConfig.close();
	numMapRecords = numDictRecords = numPostRecords = -1;
}

void InvertedFile::closeAfterReading() {
	mapFile.close();
        dictFile.close();
        postFile.close();
	numMapRecords = numDictRecords = numPostRecords = -1;
}

bool InvertedFile::writeMapRecord(int docID, const string& filename) {
	if(!mapFile.is_open()){
		cout << "\nERROR! \"mapFile.txt\" IS NOT OPEN! PLEASE TRY AGAIN.\n";
		return false;
	}else if(docID > 9999999 || filename.length() > 20){
		cout << "\nERROR! EITHER docID AND/OR filename ARE TOO LONG! PLEASE TRY AGAIN.\n";
		return false;
	}	
	mapFile.seekp(0, fstream::end);
	mapFile << left << setw(7) << docID << " " << left << setw(20) << filename << "\n";
	numMapRecords++;
	return true;
}

bool InvertedFile::writePostRecord(int docID, float weight) {
	if(!postFile.is_open()){
                cout << "\nERROR! \"postFile.txt\" IS NOT OPEN! PLEASE TRY AGAIN.\n";
        	return false;
	}else if(docID > 9999999 || weight > 9999999){
                cout << "\nERROR! EITHER docID AND/OR weight ARE TOO LONG! PLEASE TRY AGAIN.\n";
        	cout << docID << weight << endl;
		return false;
	}
        postFile.seekp(0, fstream::end);
        postFile << left << setfill(' ') << setw(7) << docID << " " << left << setfill('0') << setw(7) << to_string(weight).substr(0,7) << "\n";
	numPostRecords++;
	return true;
}

bool InvertedFile::writeDictRecord(const string& term, int numdocs, int start) {
	if(!dictFile.is_open()){
                cout << "\nERROR! \"dictFile.txt\" IS NOT OPEN! PLEASE TRY AGAIN.\n";
		return false;
        }else if(numdocs > 9999999 || start > 9999999){
                cout << "\nERROR! EITHER numdocs AND/OR start ARE TOO LONG! PLEASE TRY AGAIN.\n";
        	return false;
	}
        dictFile.seekp(0, fstream::end);
        dictFile << left << setw(34) << term.substr(0, (((int)term.length() < 34) ? (int)term.length() : 34)) << " " << left << setw(7) << numdocs << " " << left << setw(7) << start << "\n";
	numDictRecords++;
	return true;
}

bool InvertedFile::readMapRecord(int recordNum, int& docID, string& filename) {
	if(!mapFile.is_open()){
		cout << "\nERROR! \"mapFile.txt\" IS NOT OPEN! PLEASE TRY AGAIN.\n";
		return false;
	}	
	if ((recordNum >= 0) && (recordNum < numMapRecords)){
      		mapFile.seekg(recordNum * mapRecordSize, ios::beg);
		mapFile >> docID >> filename;
		return true;
   	}
   	else{
      		cout << "Map Record " << recordNum << " out of range.\n";
		return false;
	}
}

bool InvertedFile::readPostRecord(int recordNum, int& docID, float& weight){
	if(!postFile.is_open()){
                cout << "\nERROR! \"postFile.txt\" IS NOT OPEN! PLEASE TRY AGAIN.\n";
                return false;
        }
	if ((recordNum >= 0) && (recordNum < numPostRecords)){
                postFile.seekg(recordNum * postRecordSize, ios::beg);
                postFile >> docID >> weight;
        	return true;
	}
        else{
                cout << "Post Record " << recordNum << " out of range.\n";
        	return false;
	}
}

bool InvertedFile::readDictRecord(int recordNum, string& term, int& numdocs, int& start) {
	if(!dictFile.is_open()){
                cout << "\nERROR! \"dictFile.txt\" IS NOT OPEN! PLEASE TRY AGAIN.\n";
                return false;
        }
	if ((recordNum >= 0) && (recordNum < numPostRecords)){
                dictFile.seekg(recordNum * dictRecordSize, ios::beg);
                dictFile >> term >> numdocs >> start;
        	return true;
	}
        else{
                cout << "Dict Record " << recordNum << " out of range.\n";
        	return false;
	}
}

bool InvertedFile::findDictRecord(const string Key, const int size, int& numdocs, int& start){
	if(!dictFile.is_open()){
                cout << "\nERROR! \"dictFile.txt\" IS NOT OPEN! PLEASE TRY AGAIN.\n";
                return -1;
        }
	unsigned long Sum = 0;
	unsigned long Index;

	// add all the characters of the key together
   	for (long unsigned i=0; i < Key.length(); i++){
		Sum = (Sum * 19) + Key[i];  // Mult sum by 19, add byte value of char
	}
   	Index = Sum % size;
	//cout << endl << Index << " = " << Sum << " % " << size << endl;

   	// Check to see if word is in that location
   	// If not there, do linear probing until word found
   	// or empty location found.
	string term;
	do{
   		dictFile.seekg(Index * dictRecordSize, ios::beg);
		dictFile >> term >> numdocs >> start;
		//cout << endl << "TERM: " << term << ";" << endl << "NUMDOCS: " << numdocs << endl << "START: " << start << endl << endl;
		Index++;
	}while((Key != term) && (term != "[]"));
	Index--;

   	return (term != "[]");
}
