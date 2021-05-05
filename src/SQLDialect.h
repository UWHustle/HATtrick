//
// Created by elena on 17/10/20.
//
#ifndef HATTRICKBENCH_SQLDIALECT_H
#define HATTRICKBENCH_SQLDIALECT_H
#include <vector>
#include <string.h>
#include <string>
#include "UserInput.h"
using namespace std;

class SQLDialect{
	public:
        static vector<vector<string>> bulkLoadStmt;
        static vector<vector<string>> createIndexStmt;
        static vector<vector<string>> dropSchemaStmt;
        static vector<vector<string>> createSchemaStmt;
        static vector<string> init;
        static vector<vector<string>> deleteTuplesStmt;
        static vector<string> analyticalQueries; 
        static vector<vector<string>> transactionalQueries;          // for stored procedures
        static vector<string> transactionalCommands;     // for prepared statements
};
#endif //HATTRICKBENCH_SQLDIALECT_H