
#ifndef HATTRICKBENCH_GLOBALS_H
#define HATTRICKBENCH_GLOBALS_H
#include <atomic>
#include <vector>
#include "Barrier.h"
#include "UserInput.h"
#include "LinkedList.h"
using namespace std;

enum runType{none, warmup, testing};

class Globals{
public:
    Globals();
    int freshnessPeriod = 0;
    time_t epoch_time;    // global epoch time use by T and A. 
    atomic<int> loOrderKey{0};
    //atomic<int> txnNum{0};
    runType typeOfRun;
    Barrier* barrierW;
    Barrier* barrierT;
    vector<LinkedList*> containers;  // containers in which each tran. clients saves the mapping of the commit_time and txn_num
    //int IncrementTxnNum();
    //int GetTxnNum();
    int getLoOrderKey();
    time_t GetEpochTime();
};

#endif //HATTRICKBENCH_GLOBALS_H
