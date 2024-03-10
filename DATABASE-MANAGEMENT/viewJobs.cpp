/* Compile using:
g++ -Wall -I/usr/include/cppconn -o odbc viewJobs.cpp -L/usr/lib -lmysqlcppconn
run:
./odbc */
#include "odbc_db.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
string Username = "bkj011";   // Change to your own username
string mysqlPassword = "oa8ahYoo";  // Change to your mysql password
string SchemaName = "bkj011"; // Change to your username

   odbc_db myDB;
   myDB.Connect(Username, mysqlPassword, SchemaName);
   myDB.initDatabase();

   // Parse input string to get job information
   string major;
   string builder = "";

   // Read command line arguments
   // First arg, arg[0] is the name of the program
   // Next args are the parameters
   major = argv[1];

   if(major.compare("\\*") == 0){
	   builder.append(myDB.query("SELECT * FROM JOBS;"));
   }else{
	   builder.append(myDB.query("SELECT * FROM JOBS WHERE DESIREDMAJOR = '" + major + "';"));
   }

   cout << builder;

   myDB.disConnect();//disconect Database

   return 0;
}
