#ifndef SERVER_H
#define SERVER_H

#include <sys/epoll.h>
#include <memory>
#include <unordered_map>
#include <string>
#include "threadpool.h"
#include "timequeue.h"
#include "timer.h"

const int MAX_FD = 65536;           //最大文件描述符
const int MAX_EVENT_NUMBER = 10000; //最大事件数
const int TIMESLOT = 5 * 1000;      //最小超时单位

class Server
{
public:
    typedef std::shared_ptr<httpconn> httpconn_ptr;
    typedef std::shared_ptr<Client_data> clientdata_ptr;

public:
    Server();
    ~Server();

    //初始化Server参数
    void init(int port, int opt_linger, int thread_num);
    //初始化日志系统
    //void log();
    //初始化线程池
    void thread_pool();
    //配置listen socket
    void listenfd();
    //Server主循环
    void eventloop();

public:
    //listen socket接受连接
    void dealclientdata();
    //处理信号
    void dealwithsignal(bool& stop_server);
    //处理写事件
    void dealwithwrite(int sockfd);
    //处理读时间
    void dealwithread(int sockfd);
    //删除定时器（由于挂起/错误等时间导致的关闭处理）
    //void deal_timer(std::shared_ptr<timer_node> timer);
    //为新连接的socket设置定时器
    void timer(int connfd, struct sockaddr_in client_address);
    //调整timer
    //void adjust_timer(std::shared_ptr<timer_node> timer);

public:
    //通用参数
    int m_port;
    char* m_root;

    int m_epollfd;
    int m_pipe[2];
    //std::unordered_map<int, httpconn_ptr> users;
    std::unordered_map<int, httpconn> users;

    //线程池参数
    int m_thread_size;
    int m_request_queue_size = 1000;
    threadpool* m_threadpool;

    //数据库参数
    std::string m_name;
    std::string m_password;
    std::string m_databaseName;
    
    //IO复用参数
    epoll_event events[MAX_EVENT_NUMBER];
    int m_listenfd;
    int m_OPT_LINGER;
    
    //定时器相关
    //修改为heaptimer，可以灵活adjust和delete
    //std::unordered_map<int, clientdata_ptr> users_timer;
    Util util;
    TimeQueue timerheap;
    int timeoutMS = TIMESLOT;
};

#endif