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
   string query1 = "SELECT * from JOBS;";
   //builder.append("<br><br><br> JOBS table before:" + myDB.query(query1) +"<br>");

   // Parse input string to get job information
   string jobCompany;
   string jobTitle;
   string jobSalary;
   string jobMajor;

   // Read command line arguments
   // First arg, arg[0] is the name of the program
   // Next args are the parameters
   jobCompany = argv[1];
   jobTitle = argv[2];
   jobSalary = argv[3];
   jobMajor = argv[4];

   if(jobCompany.compare("") == 0 || jobTitle.compare("") == 0 || jobSalary.compare("") == 0 || jobMajor.compare("") == 0){
	   cout << "INVALID INPUT" << endl;
	   return 0;
   }

   // Get the next usable id
   string q = "select IFNULL(max(JOBID), 1000) as max_id from JOBS";
   sql::ResultSet *result = myDB.rawQuery(q);
   int next_id = 1;
   if (result->next()) // get first row of result set
      next_id += result->getInt("max_id");

   // Insert the new student
   string input = "'" + to_string(next_id) + "','" + jobCompany + "','" + jobTitle + "','" + jobSalary + "','" + jobMajor + "'";

   // DEBUG:
   // printf("%s", input.c_str());
   myDB.insert("JOBS", input);    // insert new student

   //For debugging purposes: Show the database after insert
   builder.append("<br><br><br> Table JOBS after:" + myDB.query(query1));
   cout << builder;

   myDB.disConnect();//disconect Database

   return 0;
}
