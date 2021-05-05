//
// Created by elena on 14/10/20.
//
#ifndef HATTRICKBENCH_RESULTS_H
#define HATTRICKBENCH_RESULTS_H
#include "AnalyticalClient.h"
#include "TransactionalClient.h"
#include <iostream>
using namespace std;

class Results{
private:
    ofstream resultsStream;
    int totalQueries = 0;
    int totalTxns = 0;
    double txnLatency[3] = {0.0};
    double queryExecTime[13] = {0.0};
    double freshness = 0.0;
    int testDuration = 0;
    double t_throughput = 0.0;
    double a_throughput = 0.0;
public:
    void setTotalQueries(vector<AnalyticalClient*>& a);
    void setTotalTxns(vector<TransactionalClient*>& t);
    void getQueryExecTime(vector<AnalyticalClient*>& a);
    void getTxnLatency(vector<TransactionalClient*>& t);
    void getFreshness(vector<AnalyticalClient*>& a);
    void getTestDuration(vector<AnalyticalClient*>& a);
    double getTransactionalThroughput();
    double getAnalyticalThroughput();
    void setTransactionalThroughput(double& tt);
    void setAnalyticalThroughput(double& at);
    void saveResults();
    void computeResults(vector<TransactionalClient*>& t, vector<AnalyticalClient*>& a);
};
#endif //HATTRICKBENCH_RESULTS_H
