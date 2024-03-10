#!/bin/bash
set -e -v

g++ -c addStudents.cpp
g++ -c addJobs.cpp
g++ -c addApplications.cpp
g++ -c viewStudents.cpp
g++ -c viewJobs.cpp
g++ -c viewApplications.cpp
g++ -c addHires.cpp
g++ -c odbc_db.cpp
g++ -Wall -I/usr/include/cppconn -o addStudents.exe addStudents.o odbc_db.o -L/usr/lib -lmysqlcppconn
g++ -Wall -I/usr/include/cppconn -o addJobs.exe addJobs.o odbc_db.o -L/usr/lib -lmysqlcppconn
g++ -Wall -I/usr/include/cppconn -o addApplications.exe addApplications.o odbc_db.o -L/usr/lib -lmysqlcppconn
g++ -Wall -I/usr/include/cppconn -o viewStudents.exe viewStudents.o odbc_db.o -L/usr/lib -lmysqlcppconn
g++ -Wall -I/usr/include/cppconn -o viewJobs.exe viewJobs.o odbc_db.o -L/usr/lib -lmysqlcppconn
g++ -Wall -I/usr/include/cppconn -o viewApplications.exe viewApplications.o odbc_db.o -L/usr/lib -lmysqlcppconn
g++ -Wall -I/usr/include/cppconn -o addHires.exe addHires.o odbc_db.o -L/usr/lib -lmysqlcppconn
chmod 755 *.cpp
chmod 755 *.exe
chmod 755 *.php
