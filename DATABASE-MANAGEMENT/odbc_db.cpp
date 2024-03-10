#include "odbc_db.h"
#include <string>
using namespace std;

// Connect to the database
void odbc_db::Connect(string Username, string mysqlPassword, string SchemaName)
{
   try
   {
      driver = get_driver_instance();
      connection = driver->connect("tcp://127.0.0.1:3306", Username, mysqlPassword);
      connection->setSchema(SchemaName);
   }
  catch (sql::SQLException &e) 
  {
      cout << "ERROR: SQLException in " << __FILE__;
      cout << " (" << __func__<< ") on line " << __LINE__ << endl;
      cout << "ERROR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
      cout << ", SQLState: " << e.getSQLState() << ")" << endl;
   }
}

// Disconnect from the database
void odbc_db::disConnect()  
{
   delete resultSet;
   delete statement;
   connection -> close();
   delete connection;
}

// Execute an SQL query passed in as a String parameter
// and print the resulting relation
string odbc_db::query(string q) 
{
string builder = ""; 
   try 
   {
      resultSet = statement->executeQuery(q);
      builder.append("<br>---------------------------------<br>");
      //builder.append("Query: <br>" + q + "<br><br>Result: ");
      builder.append(print(resultSet));
   }
   catch (sql::SQLException e) 
   {
      cout << "ERROR: SQLException in " << __FILE__;
      cout << " (" << __func__<< ") on line " << __LINE__ << endl;
      cout << "ERROR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
      cout << ", SQLState: " << e.getSQLState() << ")" << endl;
   }
   return builder;
}

// Execute an SQL query passed in as a String parameter
// and print the resulting relation
sql::ResultSet* odbc_db::rawQuery(string q) 
{
string builder = ""; 
   try 
   {
      resultSet = statement->executeQuery(q);
   }
   catch (sql::SQLException e) 
   {
      cout << "ERROR: SQLException in " << __FILE__;
      cout << " (" << __func__<< ") on line " << __LINE__ << endl;
      cout << "ERROR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
cout << ", SQLState: " << e.getSQLState() << ")" << endl;
}
return resultSet;
}

// Print the results of a query with attribute names on the first line
// Followed by the tuples, one per line
string odbc_db::print (sql::ResultSet *resultSet) 
{
string builder = ""; 
try
{
if (resultSet -> rowsCount() != 0)
{
 sql::ResultSetMetaData *metaData = resultSet->getMetaData();
 int numColumns = metaData->getColumnCount();
 builder.append(printHeader( metaData, numColumns));
 builder.append(printRecords( resultSet, numColumns));
}
else
 throw runtime_error("ResultSetMetaData FAILURE - no records in the result set");
}
catch (std::runtime_error &e) 
{
}
return builder;
}

// Print the attribute names
string odbc_db::printHeader(sql::ResultSetMetaData *metaData, int numColumns)
{ 
string builder = "";

try 
{
builder.append("<table border=\"2\" bordercolor=\"151973\"> <thead> <tr>");
//Printing Column names
for (int i = 1; i <= numColumns; i++) 
{
 builder.append("<th style=\"text-align: center\">");
 builder.append(metaData->getColumnName(i)); //ColumnName	 
 builder.append("</th>");
}
builder.append("</thead>");
}
catch (sql::SQLException &e)
{
}
return builder;
}

// Print the attribute values for all tuples in the result
string odbc_db::printRecords(sql::ResultSet *resultSet, int numColumns)   
{ 
string builder = "";

//Printing Records
try
{
builder.append("<tbody>");
while (resultSet->next()) 
      {
	 builder.append("<tr>");
         for (int i = 1; i <= numColumns; i++) 
         {
            builder.append("<td style=\"text-align: center\">");
            builder.append(resultSet->getString(i));
	    builder.append("</td>"); 
         }
         builder.append("<tr>");
      }
      builder.append("</tbody></table>");
   }
   catch (sql::SQLException &e)
   {
   }
   return builder;
}

// Insert into any table, any values from data passed in as String parameters
void odbc_db::insert(string table, string values) 
{
string query = "INSERT into " + table + " values (" + values + ")";

   statement->executeUpdate(query);
}

// Delete from any table, any values from data passed in as String parameters
void odbc_db::deleteVal(string query)
{
   statement->executeUpdate(query);
}


// Remove all records and fill them with values for testing
// Assumes that the tables are already created
// Assumes that connection to database is already made
void odbc_db::initDatabase()
{
   try 
   {
      // Drop records from existing tables here
      statement = connection->createStatement();




   }
   catch (sql::SQLException &e)
   {
      cout << "ERROR: SQLException in " << __FILE__;
      cout << " (" << __func__<< ") on line " << __LINE__ << endl;
      cout << "ERROR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
      cout << ", SQLState: " << e.getSQLState() << ")" << endl;
   }
}
