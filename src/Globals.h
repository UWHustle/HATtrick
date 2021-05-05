#ifndef HATTRICKBENCH_GLOBALS_H
#define HATTRICKBENCH_GLOBALS_H
#include <atomic>
#include "Barrier.h"
using namespace std;

enum runType{none, warmup, testing};

class Globals{
public:
    Globals();
    atomic<int> loOrderKey{0};
    atomic<int> batch{0};
    atomic<int> freshness{0};
    runType typeOfRun;
    Barrier* barrierW;
    Barrier* barrierT;
    int getLoOrderKey();
    int getBatch();
};

#endif //HATTRICKBENCH_GLOBALS_H