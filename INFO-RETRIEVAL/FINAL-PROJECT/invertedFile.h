#ifndef INVERTEDFILE_H
#define INVERTEDFILE_H

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class InvertedFile {
	private:
		fstream mapFile;
		fstream dictFile;
		fstream postFile;
		const int mapRecordSize;
		const int dictRecordSize;
		const int postRecordSize;
		int numMapRecords;
		int numDictRecords;
		int numPostRecords;
		const string configFilename;
		//If needed, add more variables here
	public:
		InvertedFile();
		~InvertedFile();
		void openForWrite();
		void openForRead();
		void closeAfterWriting();
		void closeAfterReading();
		bool writeMapRecord(int docID, const string& filename);
		bool writePostRecord(int docID, float weight);
		bool writeDictRecord(const string& term, int numdocs, int start);
		bool readMapRecord(int recordNum, int& docID, string& filename);
		bool readPostRecord(int recordNum, int& docID, float& weight);
		bool readDictRecord(int recordNum, string& term, int& numdocs, int& start);
		bool findDictRecord(const string Key, const int size, int& numdocs, int& start);
};

#endif
