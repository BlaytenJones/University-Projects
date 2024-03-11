#include <iostream>
#include "invertedFile.h"
#include "hashtable.h"
#include "linkedList.h"

using namespace std;

void writeMapRecords(InvertedFile &invFile){
	invFile.writeMapRecord(0, "Document0");
	invFile.writeMapRecord(1, "Document1");
	invFile.writeMapRecord(2, "Document2");
	invFile.writeMapRecord(3, "Document3");
}

void writeDictRecords(InvertedFile &invFile){
	invFile.writeDictRecord("ate", 4, 0);
	invFile.writeDictRecord("cat", 1, 4);
	invFile.writeDictRecord("doctor", 1, 5);
	invFile.writeDictRecord("dog", 2, 6);
	invFile.writeDictRecord("dry", 1, 8);
	invFile.writeDictRecord("duck", 1, 9);
	invFile.writeDictRecord("quickly", 1, 10);
}

void writePostRecords(InvertedFile &invFile){
	invFile.writePostRecord(0, 1.0);
	invFile.writePostRecord(1, 2.0);
	invFile.writePostRecord(2, 1.0);
	invFile.writePostRecord(3, 1.0);
	invFile.writePostRecord(3, 1.0);
	invFile.writePostRecord(2, 2.0);
	invFile.writePostRecord(0, 1.0);
	invFile.writePostRecord(1, 1.0);
	invFile.writePostRecord(2, 1.0);
	invFile.writePostRecord(2, 1.0);
	invFile.writePostRecord(0, 1.0);
}

void printMapRecord(InvertedFile &invFile, int recordNum){
	int docID; string filename;
	if(invFile.readMapRecord(recordNum, docID, filename)){
		cout << recordNum << " " << docID << " " << filename << "\n";
	}
}

void printDictRecord(InvertedFile &invFile, int recordNum){
	int numdocs, start; string term;
	if(invFile.readDictRecord(recordNum, term, numdocs, start)){
        	cout << recordNum << " " << term << " " << numdocs << " " << start << "\n";
	}
}

void printPostRecord(InvertedFile &invFile, int recordNum){
	int docID; float weight;
	if(invFile.readPostRecord(recordNum, docID, weight)){
        	cout << recordNum << " " << docID << " " << weight << "\n";
	}
}

int main(){
	
}
