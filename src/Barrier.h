#ifndef BARRIER_H
#define BARRIER_H
#include <condition_variable>
#include <thread>
#include <iostream>
using namespace std;
class Barrier
{
 private:
      mutex m;
      condition_variable cv;
      int counter;
      int thread_count;
 public:
    Barrier(int count);
    void wait();
};
#endif