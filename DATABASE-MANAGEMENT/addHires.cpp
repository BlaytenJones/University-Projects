/* Compile using:
g++ -Wall -I/usr/include/cppconn -o odbc addHires.cpp -L/usr/lib -lmysqlcppconn
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
   string query1 = "SELECT * from HIRES;";
   //builder.append("<br><br><br> HIRES table before:" + myDB.query(query1) +"<br>");

   // Parse input string to get job information
   string employeeID;
   string jobID;
   string location;
   string startDate;

   // Read command line arguments
   // First arg, arg[0] is the name of the program
   // Next args are the parameters
   employeeID = argv[1];
   jobID = argv[2];
   location = argv[3];
   startDate = argv[4];

   if((employeeID.compare("") == 0) || (jobID.compare("") == 0) || (location.compare("") == 0) || (startDate.compare("") == 0)){
	   cout << "INPUTS ARE INVALID" << endl;
	   return 0;
   }
   
   //Delete application
   myDB.deleteVal("DELETE FROM APPLICATIONS WHERE STUDENTID = " + employeeID + " AND JOBID = " + jobID + ";");
   cout << endl << "DELETED FROM APPLICATIONS; IF THERE IS NO FURTHER OUTPUT, THIS PERSON HAS ALREADY BEEN HIRED..." << endl;

   // Insert the new hire
   string input = "'" + employeeID + "','" + jobID + "','" + location + "','" + startDate + "'";

   // DEBUG:
   // printf("%s", input.c_str());
   myDB.insert("HIRES", input);    // insert new hire

   //For debugging purposes: Show the database after insert
   builder.append("<br><br><br> Table HIRES after:" + myDB.query(query1));
   cout << builder;

   myDB.disConnect();//disconect Database

   return 0;
}
