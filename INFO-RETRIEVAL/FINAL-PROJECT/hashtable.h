/* Filename:  hashtable.h
 * Author:    Susan Gauch
 * Date:      2/25/10
 * Purpose:   The header file for a hash table of strings and ints. 
*/

#include "invertedFile.h"
#include <bits/stdc++.h>

using namespace std;

class HashTable {
public:
   HashTable (const HashTable& ht );       // constructor for a copy
   HashTable(const unsigned long NumKeys);          // constructor of hashtable 
   ~HashTable();                           // destructor
   void Print (const char *filename) const;       
   void Insert (const string Key, const float Data, const int Start = -1, const int numDocs = 0); 
   float GetData (const string Key); 
   void GetUsage (int &Used, int &Collisions, int &Lookups) const;
   string Print(int docID);
   void Clear();
   bool UpdateStart(const string Key, const int Start);
   void PrintDict(InvertedFile& IV, bool postEmpty = false);
   float CalculateTotalLog();
   float Walk(int index, bool mode = 0);
protected:
   struct StringIntTuple // the datatype stored in the hashtable`
   {
      string key;
      float data;
      int start;
      int numDocs;
      bool edited = false;
   };
   unsigned long Find (const string Key); // the index of the ddr in the hashtable
private:
   StringIntTuple *hashtable;        // the hashtable array itself
   unsigned long size;              // the hashtable size
   unsigned long used;
   unsigned long collisions;
   unsigned long lookups;
};

