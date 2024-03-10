#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/statement.h>
#include <string>

#ifndef odbc_db_H
#define odbc_db_H
using namespace std;

// This class has some useful methods that can be used by other programs
class odbc_db 
{
   private:
      string Username;
      string mysqlPassword;
      string SchemaName ;
      sql::ResultSet *resultSet;
      sql::ResultSetMetaData *metaData;
      sql::Driver *driver;
      sql::Connection *connection;
      sql::Statement *statement;

   public:
      // Connect to the database
      void Connect(string Username, string mysqlPassword, string SchemaName);
    
      // Disconnect from the database
      void disConnect();  
    
      // Execute an SQL query passed in as a String parameter, print the resulting relation
      string query(string q);

      // Execute an SQL query passed in as a String parameter, print the resulting relation
      sql::ResultSet* rawQuery(string q);

      // Print the results of a query with attribute names on the first line, then tuples
      string print (sql::ResultSet *resultSet); 

      // Print the attribute names
      string printHeader(sql::ResultSetMetaData *metaData, int numColumns);
    
      // Print the attribute values for all tuples in the result
      string printRecords(sql::ResultSet *resultSet, int numColumns);

      // Insert into any table, any values from data passed in as String parameters
      void insert(string table, string values);
     
      //Delete any value from the table
      void deleteVal(string query);

      // Remove all records and fill them with values for testing
      // Assumes that the tables are already created
      // Assumes that connection to database is already made
      void initDatabase();
};
#endif
