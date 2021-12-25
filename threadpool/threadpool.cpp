#include "threadpool.h"
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;

threadpool::threadpool(int thread_s, int max_queue_s):thread_size(thread_s), max_queue_size(max_queue_s)
{
    if(thread_s < 0 || thread_s > MAX_THREAD_SIZE){
        thread_size = 4;
    }
    if(max_queue_s < 0 || max_queue_s > MAX_QUEUE_SIZE){
        max_queue_size = MAX_QUEUE_SIZE;
    }

    threads.resize(thread_s);

    for(int i = 0; i<thread_s; ++i){
        if(pthread_create(&threads[i], NULL, work, this) != 0){
            std::cout << "thread start failed" << std::endl;
            throw std::exception();
        }
    }
}

threadpool::~threadpool(){

}

bool threadpool::append(httpconn* arg, int mode){
    m_queuelocker.lock();
    if(request_queue.size() >= max_queue_size){
        m_queuelocker.unlock();
        return false;
    }
    arg->m_state = mode;
    request_queue.push_back(arg);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

void* threadpool::work(void* arg){
    threadpool* pool = (threadpool* ) arg;
    pool->run();
    return pool;
}

void threadpool::run(){
    while(true){
        std::cout << "thread run" << std::endl;
        m_queuestat.wait();
        m_queuelocker.lock();
        if(request_queue.empty()){
            m_queuelocker.unlock();
            continue;
        }
        httpconn* request = request_queue.front();
        request_queue.pop_front();
        m_queuelocker.unlock();
        //std::cout << "working mode: " << request->m_state << std::endl;
        //std::cout << "working thread id: " << syscall(SYS_gettid) << std::endl;
        if(request->m_state == 0){
            if(request->read_once()){
                request->improv = 1;
                request->process();
            }
            else{
                request->improv = 1;
                request->time_flager = 1;
            }
        }
        else{
            if(request->write()){
                request->improv = 1;
            }
            else{
                request->improv = 1;
                request->time_flager = 1;
            }
        }
    }
}

