/* Compile using:
g++ -Wall -I/usr/include/cppconn -o odbc viewApplications.cpp -L/usr/lib -lmysqlcppconn
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

   // Parse input string to get student information
   string choice;
   string attribute;
   string builder = "";

   // Read command line arguments
   // First arg, arg[0] is the name of the program
   // Next args are the parameters
   choice = argv[1];

	string startString = "SELECT STUDENTNAME, COMPANYNAME, JOBTITLE, SALARY, MAJOR, DESIREDMAJOR FROM APPLICATIONS NATURAL JOIN STUDENTS NATURAL JOIN JOBS";

   if(choice.compare("\\*") != 0){
	   attribute = argv[2];
	   switch(attribute[0]){
		   case '1':
			   builder.append(myDB.query(startString + " WHERE JOBID = '" + choice + "'"));
			   break;
		   case '2':
			   builder.append(myDB.query(startString + " WHERE DESIREDMAJOR = '" + choice + "'"));
			   break;
		   case '3':
			   builder.append(myDB.query(startString + " WHERE MAJOR = '" + choice + "'"));
			   break;
		   case '4':
			   builder.append(myDB.query(startString + " WHERE STUDENTID = '" + choice + "'"));
			   break;
	   }
   }else{
	   builder.append(myDB.query(startString));
   }

   cout << builder;

   myDB.disConnect();//disconect Database

   return 0;
}
