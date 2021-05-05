//
// Created by elena on 18/10/20.
//
#ifndef HATTRICKBENCH_WORKLOAD_H
#define HATTRICKBENCH_WORKLOAD_H
#include "Driver.h"
#include "SQLDialect.h"
#include "UserInput.h"
#include "AnalyticalClient.h"
#include "TransactionalClient.h"
#include "GetFromDB.h"
#include "Barrier.h"
#include "DataSrc.h"
#include "Globals.h"
#include "Results.h"
#include <thread>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <atomic>

using namespace std;

class Workload{
private:
    vector<thread> aThreads;
    vector<thread> tThreads;
    vector<AnalyticalClient*> aClients;
    vector<TransactionalClient*> tClients;

public:
    Workload();
    static bool runTime(chrono::steady_clock::time_point& startTime, int duration);
    static void AnalyticalStream(AnalyticalClient* aClient, runType& typeOfRun, std::atomic<int>& freshness);
    static void TransactionalStreamPS(TransactionalClient* tClient, Globals* g, SQLHDBC& dbc);  // for Prepared Statements
    static void TransactionalStreamSP(TransactionalClient* tClient, Globals* g, SQLHDBC& dbc);  // for Stored Procedures
    static void AnalyticalWorkload(AnalyticalClient* aClient, SQLHENV& env, Globals* g);
    static void TransactionalWorkload(TransactionalClient* tClient, SQLHENV& env, Globals* g, int t);
    void ExecuteWorkloads(SQLHENV& env, Globals* g);
    void ReturnResults(Results* r);
};

#endif //HATTRICKBENCH_WORKLOAD_H