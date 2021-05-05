#ifndef DBINIT_H
#define DBINIT_H
#include "Driver.h"
#include "UserInput.h"
#include "SQLDialect.h"
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <vector>
using namespace std;

class DBInit{
    private:
        static vector<const char*> createSchemaStmt;
        static vector<const char*> dropSchemaStmt;
        static vector<const char*> bulkLoadStmt;
    public:
        static int createSchema(SQLHSTMT& stmt);
        static int dropSchema(SQLHSTMT& stmt);
        static int bulkLoad(SQLHSTMT& stmt);
        static int indexCreation(SQLHSTMT& stmt);
};
#endif