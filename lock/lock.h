#ifndef LOCK_H
#define LOCK_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

class Mutexlock
{
public:
    Mutexlock(){
        pthread_mutex_init(&mutex_, NULL);
    }

    ~Mutexlock(){
        pthread_mutex_destroy(&mutex_);
    }

    void lock(){
        pthread_mutex_lock(&mutex_);
    }

    void unlock(){
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getMutex(){
        return &mutex_;
    }
public:
    pthread_mutex_t mutex_;
};

class Sem
{
public:
    Sem(){
        sem_init(&sem_, 0, 0);
    }

    Sem(int init_num){
        sem_init(&sem_, 0, init_num);
    }

    ~Sem(){
        sem_destroy(&sem_);
    }

    void wait(){
        sem_wait(&sem_);
    }

    void post(){
        sem_post(&sem_);
    }

public:
    sem_t sem_;
};

class Condition
{
public:
    Condition(){
        pthread_cond_init(&cond_, NULL);
    }

    ~Condition(){
        pthread_cond_destroy(&cond_);
    }

    void signal(){
        pthread_cond_signal(&cond_);
    }

    bool wait(pthread_mutex_t* lock_){
        int ret = 0;
        ret = pthread_cond_wait(&cond_, lock_);
        return ret == 0;
    }
    
public:
    pthread_cond_t cond_;
};

#endif