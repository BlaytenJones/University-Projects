/* Compile using:
g++ -Wall -I/usr/include/cppconn -o odbc addStudents.cpp -L/usr/lib -lmysqlcppconn
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

   // For debugging purposes:  Show the database before insert
   string builder = "";
   string query1 = "SELECT * from APPLICATIONS;";
   //builder.append("<br><br><br> APPLICATIONS table before:" + myDB.query(query1) +"<br>");

   // Parse input string to get job information
   string studentID;
   string jobID;

   // Read command line arguments
   // First arg, arg[0] is the name of the program
   // Next args are the parameters
   studentID = argv[1];
   jobID = argv[2];
   
   if(studentID.compare("") == 0 || jobID.compare("") == 0){
	   cout << "INVALID INPUT" << endl;
	   return 0;
   }

   // Insert the new student
   string input = "'" + studentID + "','" + jobID + "'";

   // DEBUG:
   // printf("%s", input.c_str());
   myDB.insert("APPLICATIONS", input);    // insert new student

   //For debugging purposes: Show the database after insert
   builder.append("<br><br><br> Table APPLICATIONS after:" + myDB.query(query1));
   cout << builder;

   myDB.disConnect();//disconect Database

   return 0;
}
