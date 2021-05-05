#ifndef DRIVER_H
#define DRIVER_H
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>

class Driver{

    public:
        static void extract_error(const char* fn, SQLHANDLE& handle, SQLSMALLINT type);
        static void setEnv(SQLHENV& env);
        static void connectDB(SQLHENV& env, SQLHDBC& dbc);
        static void connectDB2(SQLHENV& env, SQLHDBC& dbc);

        static int executeStmtDiar(SQLHSTMT& stmt, const char* query);
        static int executeStmt(SQLHSTMT& stmt);
        static void bindCharColumn(SQLHSTMT& stmt, char* colBuf, int size,  int colNum);
        static void bindIntColumn(SQLHSTMT& stmt, int& colBuf, int colNum);
        static void bindDecColumn(SQLHSTMT& stmt, double& colBuf, int colNum);
        static void bindCharParam(SQLHSTMT& stmt, char* param, int size, int pos);
        static void bindIntParam(SQLHSTMT& stmt, int& param, int pos);
        static void bindDecParam(SQLHSTMT& stmt, double& param, int pos);
        static void resetStmt(SQLHSTMT& stmt);
        static void prepareStmt(SQLHDBC& dbc, SQLHSTMT& stmt, const char* query);
        static void allocHandle(SQLHDBC& dbc, SQLHSTMT& stmt);
        static void freeStmtHandle(SQLHSTMT& stmt);
        static void freeEnvHandle(SQLHENV &env);
        static void disconnectDB(SQLHDBC& dbc);
        static void fetchData(SQLHSTMT& stmt);
        static void getCharData(SQLHSTMT& stmt, int col, SQLCHAR* dataBuf);
        static void getIntData(SQLHSTMT& stmt, int col, int& dataBuf);
        static void getLongData(SQLHSTMT& stmt, int col, int& dataBuf);
        static void getDoubleData(SQLHSTMT& stmt, int col, double& dataBuf);
        static void autoCommitOff(SQLHDBC& dbc);
        static void endOfTransaction(SQLHDBC& dbc);
};
#endif