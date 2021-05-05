#ifndef HATTRICKBENCH_FRONTIER_H
#define HATTRICKBENCH_FR0NTIER_H
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
    int num_of_ratios = 0;
    vector<double> t_throughputs;
    vector<double> a_throughputs;
public:
    void deleteTuples();
    double runBenchmark(int& peak, int& choice);
    void findMaxTCAC();
    void setMaxTC(int& tc);
    void setMaxAC(int& ac);
    int getMaxTC();
    int getMaxAC();
    void findFrontier();
};
#endif //HATTRICKBENCH_FRONTIER_H
