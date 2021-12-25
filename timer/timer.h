#ifndef TIMER_H
#define TIMER_H

#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <queue>
#include <memory>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <vector>
#include <memory>

#include "lock.h"
#include "httpconn.h"

//class timer_node;

class Client_data{
public:
    sockaddr_in address;
    int sockfd;
    //std::shared_ptr<timer_node> timer;
};

// class timer_node{
// public:
//     typedef std::shared_ptr<Client_data> clientdata_ptr;

//     timer_node(size_t timeout){
//         current_time();
//         expiredTime = current_msec + timeout;
//     }
//     ~timer_node(){}

//     static void current_time(){
//         struct timeval cur;
//         gettimeofday(&cur, NULL);
//         current_msec = cur.tv_sec * 1000 + cur.tv_usec / 1000;
//     }

//     static size_t current_msec;

//     bool isexpired(){
//         return expiredTime < current_msec;
//     }

//     bool isdeleted(){
//         return deleted_;
//     }

//     void deleted(){
//         cb_timer(user_data);
//         deleted_ = true;
//     }

// public:
//     void (*cb_timer)(clientdata_ptr);
//     size_t expiredTime;
//     clientdata_ptr user_data;
//     bool deleted_ = false;
// };

// class timercmp{
//     bool operator()(std::shared_ptr<timer_node> a, std::shared_ptr<timer_node> b){
//         return a->expiredTime > b->expiredTime;
//     }
// };

class Util{
public:
    //typedef std::shared_ptr<timer_node> shared_timer_node;

public:
    void addfd(int epollfd, int fd, int mode);

    int setnoblocking(int fd);

    //void addtimer(shared_timer_node timer_node);

    //std::vector<int> handle_expired_event();

    void addsig(int sig, void(handler)(int), bool restart = true);

    static void sig_handler(int sig);


public:
    static int u_epollfd;
    static int *u_pipefd;
    //std::priority_queue<shared_timer_node, std::vector<shared_timer_node>, timercmp> timer_queue;
    Mutexlock util_locker; 
};

void cb_timer(httpconn);

#endif