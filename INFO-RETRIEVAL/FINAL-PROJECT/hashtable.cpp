/* Filename:  hashtable.cpp
 * Author:    Susan Gauch
string  * Date:      2/11/2010
 * Purpose:   The implementation file for a hash table of words and numbers.
*/

#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "hashtable.h"

using namespace std;
/*-------------------------- Constructors/Destructors ----------------------*/

/* : HashTable
 * Author: seg
 * Parameters:  ht: the hashtable to copy
 * Purpose:     copy a hashtable 
 *              NOTE:  this is crucial to include since it is invoked
 *              by call-by-value parameter passing
 * Returns:     nothing
*/
HashTable::HashTable( const HashTable &ht )
{
   size = ht.size;                    // set the size of the array
   used = ht.used;                    // set the size of the array
   if((hashtable = new StringIntTuple[size]) == NULL)
       cout << "Out of memory at HashTable::HashTable(const HashTable)" << endl;
   assert( hashtable != 0 );

   for (unsigned long i=0; i < size; i++)     // make a _copy_ of the array elements
   {
      hashtable[i].key = ht.hashtable[i].key;
      hashtable[i].data = ht.hashtable[i].data;
   }
   
}
           
/* Name:  HashTable
 * Author: seg
 * Parameters:  none
 * Purpose:     allocate a hashtable for an expected number of keys
 *              initializes all values to null (0)
 * Returns:     pointer to the created HashTable or 0 if out of memory
*/
HashTable::HashTable(const unsigned long NumKeys)
{
   // allocate space for the table, init to null key
   size = NumKeys * 3;   // we want the hash table to be 2/3 empty
   used = 0;
   collisions = 0;
   lookups = 0;
   if((hashtable = new StringIntTuple[size]) == NULL)
      cout << "Out of memory at HashTable::HashTable(unsigned long)" << endl;
   assert( hashtable != 0 );

   // initialize the hashtable
   for (unsigned long i=0; i < size; i++)
   {
      hashtable[i].key = "";
      hashtable[i].data = 0;
   }
}


/* Name:  ~HashTable
 * Author: seg
 * Parameters:  none
 * Purpose:     deallocate a hash table
 * Returns:     nothing
*/
HashTable::~HashTable()
{
   delete [] hashtable;
}

/*-------------------------- Accessors ------------------------------------*/


/* Name:  Print
 * Author: seg
 * Parameters:  none
 * Purpose:     print the contents of the hash table
 *              currently, only prints non-null entries
 * Returns:     nothing
*/
void HashTable::Print(const char *filename) const
{
   ofstream fpout(filename); 

   // Print out the non-zero contents of the hashtable
   for ( unsigned long i=0; i < size; i++ )
   {  
      if ( !(hashtable[i].key == ""))
          fpout << hashtable[i].key << " "
                << hashtable[i].data << endl;
   }
   fpout.close();
   cout << "Collisions: " << collisions << ", Used: " << used
        <<  ", Lookups: " << lookups << endl;
}

/* Name: Insert
 * Author: sgauch
 * Parameter:
 * 		key : The target of context words to be stored
 * 		frequency: Total frequency count
 * Purpose: 	insert or add a word with its frequency count in hashtable
 * Return:	nothing
*/
void HashTable::Insert (const string Key, const float Data, const int Start, const int numDocs){
unsigned long Index;

 if (used >= size)
    cerr << "The hashtable is full; cannot insert. Size is " + to_string(size) + "\n";
 else
 {
    string newKey = Key.substr(0,34);
    Index = Find(newKey);

    // If not already in the table, insert it
    if (hashtable[Index].key == "")
    {
       hashtable[Index].key = newKey;
       hashtable[Index].data = Data;
       hashtable[Index].numDocs = numDocs;
       hashtable[Index].start = Start;
       used++;
    }else{
       hashtable[Index].data += Data;
       hashtable[Index].numDocs += numDocs;
    }
    // else do nothing
 }
}

/* Name: GetData
 * Author: sgauch
 * Parameters:	key: the string
 * Purpose:	return the data or -1 if Key is not found
 * Return:	return an int 
*/
float HashTable::GetData(const string Key)
{
unsigned long Index;

 lookups++; 
 Index = Find(Key);
 if (hashtable[Index].key == "")
    return -1;
 else   
    return (hashtable[Index].data);
}

/* Name: GetUsage
 * Author: S. Gauch
 * Parameters:	None
 * Purpose:	return the number of collisions
 * Return:	return a char *
*/
void HashTable::GetUsage(int &Used, int &Collisions, int &Lookups) const
{
   Used = used;
   Collisions = collisions; 
   Lookups = lookups;
}


/*-------------------------- Private Functions ----------------------------*/
/* Name:  Find
 * Author: seg
 * Parameters:  key: the word to be located
 * Purpose:     return the index of the word in the table, or
 *              the index of the free space in which to store the word
 * Returns:     index of the word's actual or desired location
*/
unsigned long HashTable::Find (const string Key) 
{
unsigned long Sum = 0;
unsigned long Index;

   // add all the characters of the key together
   for (long unsigned i=0; i < Key.length(); i++)
      Sum = (Sum * 19) + Key[i];  // Mult sum by 19, add byte value of char
  
   Index = Sum % size;

   // Check to see if word is in that location
   // If not there, do linear probing until word found
   // or empty location found.
   while (((hashtable[Index].key) != Key) &&
          ((hashtable[Index].key) != "") ) 
   {
      Index = (Index+1) % size;
      collisions++;
   }
   
   return Index;
}

string HashTable::Print(int docID){
	std::vector<StringIntTuple> copyHashtable;
    	for (size_t i = 0; i < size; i++) {
        	if (hashtable[i].data != 0) {
            		copyHashtable.push_back(hashtable[i]);
        	}
    	}	

    	// Sort the copyHashtable based on the key field
    	std::sort(copyHashtable.begin(), copyHashtable.end(), [](const StringIntTuple& a, const StringIntTuple& b) {
        	return a.key < b.key;
    	});
	string output;
	for (long unsigned i = 0; i < copyHashtable.size(); i++){
		if(copyHashtable[i].data != 0){
			output += copyHashtable[i].key + " " + to_string(docID) + " " + to_string((int)copyHashtable[i].data) + "\n";
		}
	}
	return output;
}

void HashTable::Clear(){
	for (long unsigned i = 0; i < size; i++){
		hashtable[i].key = "";
      		hashtable[i].data = 0;
	}
}

bool HashTable::UpdateStart(const string Key, const int Start){
	if(!hashtable[Find(Key)].edited){
		hashtable[Find(Key)].start = Start;
		return true;
	}else{
		return false;
	}
}

void HashTable::PrintDict(InvertedFile& IV, bool postEmpty){
	for (unsigned long i=0; i < size; i++){
		if(hashtable[i].key != ""){
			IV.writeDictRecord(hashtable[i].key, hashtable[i].numDocs, hashtable[i].start);
		}else if(postEmpty){
			IV.writeDictRecord("[]", hashtable[i].data, hashtable[i].start);
		}
	}
}

float HashTable::CalculateTotalLog(){
	float totalLog = 0;
	for (unsigned long i=0; i < size; i++){
		int freq = hashtable[i].data;
                if(freq != 0){
                        totalLog += 1 + log(freq);
                }
        }
	return totalLog;
}

float HashTable::Walk(int index, bool mode){
	return (mode ? hashtable[index].start : hashtable[index].data);
}
