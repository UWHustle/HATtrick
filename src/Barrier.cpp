#include "Barrier.h"

Barrier::Barrier(int count){
    thread_count = count;
    counter = 0;
}

void Barrier::wait() {
    //fence mechanism
    unique_lock<mutex> lk(m);
    cout << "Lock is taken from Thread: " << this_thread::get_id() <<  endl;
    cout << "Number of threads: " << thread_count << endl;
    ++counter;
    cout << "Thread " << this_thread::get_id() << " increased the counter to: " << counter <<  endl;
    cv.wait(lk, [&]{return counter == thread_count;});
    cv.notify_all();
    cout << "Unlock the mutex by Thread: " << this_thread::get_id() <<  endl;
    lk.unlock();
}