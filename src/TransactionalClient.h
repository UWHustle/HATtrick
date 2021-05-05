//
// Created by elena on 17/10/20.
//
#ifndef HATTRICKBENCH_TRANSACTIONALCLIENT_H
#define HATTRICKBENCH_TRANSACTIONALCLIENT_H
#include "Driver.h"
#include "SQLDialect.h"
#include "DataSrc.h"
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
    int transactionsNum = 0;        // counter of transactions executed in testing period in total
    int clientNum = 0;              // number of the current client [1, 40]
    vector<vector<double>> latencyVector = vector<vector<double>>(3);    // history of response time for the 3 transactions
public:
    void PrepareTransactionStmt(SQLHDBC& dbc);
    int NewOrderTransactionPS(SQLHDBC& dbc);   // NewOrder txn w/ store procedures for PostgreSQL
    int  NewOrderTransactionSS(SQLHDBC& dbc);   // NewOrder txn w/ stored procedures for SQL Server
    void NewOrderTransaction(SQLHDBC& dbc);     // NewOrder txn w/ prepared statements, same for all DBs
    int PaymentTransactionSP(SQLHDBC& dbc);    // Payment txn w/ stored procedures, same for PostgreSQL and SQL Server
    void PaymentTransaction(SQLHDBC& dbc);      // Payment txn w/ prepared statements, same for all DBs
    int CountOrdersTransactionSP(SQLHDBC& dbc); // CountOrders txn w/ stored procedures, same for PostgreSQL and SQL Server
    void CountOrdersTransaction(SQLHDBC& dbc);   // CountOrders txn w/ prepared statements, same for all DBs
    void FreshnessTransactionSP(SQLHDBC& dbc);
    void FreshnessTransaction(SQLHDBC& dbc);
    SQLHSTMT& GetTransactionStmt();
    SQLHSTMT& GetTransactionPrepStmt(int idx);  
    void FreeTransactionStmt();
    void SetLoOrderKey(int& num);
    int& GetLoOrderKey();
    void SetClientNum(int& num);
    int GetClientNum();
    void IncrementTransactionNum();  // Increment the counter of the total transactions executed on the current client during testing 
    int GetTransactionNum();
    void SetThreadNum(thread::id num);
    thread::id GetThreadNum();
    void SetLatency(double latency, int tType);
    double GetLatencySum(int tType);
    int GetLatencySize(int tType);
};
#endif //HATTRICKBENCH_TRANSACTIONALCLIENT_H