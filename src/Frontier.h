#ifndef HATTRICKBENCH_FRONTIER_H
#define HATTRICKBENCH_FRONTIER_H
#include "Driver.h"
#include "Barrier.h"
#include "Workload.h"
#include "GetFromDB.h"
#include "Globals.h"
#include "Results.h"
#include "UserInput.h"
#include "SQLDialect.h"
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class Frontier{
private:
    int max_tc = 0;
    int max_ac = 0;
public:
    void deleteTuples();
    void createFreshnessTable(int& tc);
    double runBenchmark(int& peak, int& choice);
    void findMaxTCAC();
    void setMaxTC(int& tc);
    void setMaxAC(int& ac);
    int getMaxTC();
    int getMaxAC();
    void findFrontier();
    /*void addTransactionalThroughput(double tt);
    void addAnalyticalThroughput(double at);
    vector<double>& getTransactionalThroughputs();
    vector<double>& getAnalyticalThroughputs();*/
};
#endif //HATTRICKBENCH_FRONTIER_H
