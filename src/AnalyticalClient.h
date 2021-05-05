#ifndef HATTRICKBENCH_ANALYTICALCLIENT_H
#define HATTRICKBENCH_ANALYTICALCLIENT_H
#include "Driver.h"
#include "SQLDialect.h"
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

class AnalyticalClient{
private:
    thread::id threadNum;     // id of thread
    vector<SQLHSTMT> qStmt;  
    int queriesNum; //= 0;       // counter of queries executed in testing period in total
    vector<int> freshnessVector;   // freshness score for each analytical query executed in the current thread
    vector<vector<double>> executionTime;   // execution time of each query
    int testDuration;
public:
    AnalyticalClient();
    void SetThreadNum(thread::id num);
    void IncrementQueriesNum();
    int GetQueriesNum() const;
    thread::id GetThreadNum();
    void PrepareAnalyticalStmt(SQLHDBC& dbc);
    int ExecuteQuery(int& q);
    void SetFreshness(int& freshness);
    void FreeQueryStmt();
    void SetExecutionTime(double execTime, int qType);
    double GetExecutionTimeSum(int qType);
    int GetExecutionTimeSize(int qType);
    double GetFreshness();
    void SetTestDuration(int duration);
    int GetTestDuration();
};
#endif //HATTRICKBENCH_ANALYTICALCLIENT_H
