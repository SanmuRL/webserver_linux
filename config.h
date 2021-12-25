#ifndef CONFIG_H
#define CONFIG_H

#include<unistd.h>
#include<stdlib.h>

class Config
{
public:
    Config();
    ~Config(){};

    void parse_init(int argc, char** argv);

    //是否适用守护线程
    bool use_deamon = false;

    //线程池中线程数
    int max_thread;
    
    //服务器端口号
    int port;

    //是否关闭日志
    bool close_log;

    //优雅关闭socket
    int OPT_LINGER;
};

#endif