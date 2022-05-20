
#ifndef HATTRICKBENCH_TRANSACTIONALCLIENT_H
#define HATTRICKBENCH_TRANSACTIONALCLIENT_H
#include "Driver.h"
#include "SQLDialect.h"
#include "Globals.h"
#include "DataSrc.h"
#include "LinkedList.h"
#include <thread>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

class TransactionalClient{
private:
    thread::id threadNum;           // id of thread
    int loOrderKey;                 // current lo_orderkey
    SQLHSTMT tStmt = 0;             // for stored procedures
    vector<SQLHSTMT> ptStmt = {vector<SQLHSTMT>(SQLDialect::transactionalCommands.size(), 0)}; // for prepared statements 
    SQLHSTMT freshStmt = 0;
    int clientNum = 0;              
    vector<vector<double>> latencyVector = vector<vector<double>>(3);    // history of response time for the 3 transactions
    int txn_num[3] = {0};           // number of txns executed from each category
    int localCounter = 0;           // local counter of the transactions that the current client is running 
public:
    TransactionalClient();
    void PrepareTransactionStmt(SQLHDBC& dbc);
    int NewOrderTransactionPS(SQLHDBC& dbc);   // NewOrder txn w/ store procedures for PostgreSQL
    int  NewOrderTransactionSS(SQLHDBC& dbc);   // NewOrder txn w/ stored procedures for SQL Server
    void NewOrderTransaction(SQLHDBC& dbc);     // NewOrder txn w/ prepared statements, same for all DBs
    int PaymentTransactionSP(SQLHDBC& dbc);    // Payment txn w/ stored procedures, same for PostgreSQL and SQL Server
    void PaymentTransaction(SQLHDBC& dbc);      // Payment txn w/ prepared statements, same for all DBs
    int CountOrdersTransactionSP(SQLHDBC& dbc); // CountOrders txn w/ stored procedures, same for PostgreSQL and SQL Server
    void CountOrdersTransaction(SQLHDBC& dbc);   // CountOrders txn w/ prepared statements, same for all DBs
    SQLHSTMT& GetTransactionStmt();
    SQLHSTMT& GetTransactionPrepStmt(int idx);  
    void FreeTransactionStmt();
    void SetLoOrderKey(int& num);
    int& GetLoOrderKey();
    void SetClientNum(int& num);     // Set the number of the client
    int& GetClientNum();             // Get the number of the client
    void SetThreadNum(thread::id num);
    thread::id GetThreadNum();
    void SetLatency(double latency, int tType);
    double GetLatencySum(int tType);
    int GetLatencySize(int tType);
    void IncrementLocalCounter();
    void DecrementLocalCounter();
    int& GetLocalCounter();
    void PrepareFreshnessStmt(SQLHDBC &dbc);
    SQLHSTMT& GetFreshnessStmt();
};
#endif //HATTRICKBENCH_TRANSACTIONALCLIENT_H
