#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <memory>
#include <vector>
#include <list>

#include "httpconn.h"
#include "lock.h"

const int MAX_THREAD_SIZE = 1024;
const int MAX_QUEUE_SIZE = 10000;

class threadpool
{
public:
    threadpool(int thread_s, int max_queue_s);

    ~threadpool();

    bool append(httpconn* arg, int mode);

private:
    static void* work(void* arg);
    
    void run();

public:
    Mutexlock m_queuelocker;
    Sem m_queuestat;

    int thread_size;
    int max_queue_size;
    std::vector<pthread_t> threads;
    std::list<httpconn*> request_queue;
};

#endif