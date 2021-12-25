#include "config.h"
#include "server.h"
#include <stdio.h>
#include <fcntl.h>

using namespace std;

void daemon_run()
{
    int pid;

    pid = fork();

    if(pid < 0)
    {
        printf("fork failed");
        exit(-1);
    }
    if(pid > 0)
    {
        exit(-1);
    }

    setsid();
    int fd;
    fd = open("/dev/null", O_RDWR, 0);
    if(fd != -1)
    {
        close(STDIN_FILENO);
        dup2(fd, STDIN_FILENO);
        close(STDOUT_FILENO);
        dup2(fd, STDOUT_FILENO);
        close(STDERR_FILENO);
        dup2(fd, STDERR_FILENO);
    }
    if(fd > 2) close(fd);
}

int main(int argc, char **argv)
{
    Config config;
    
    config.parse_init(argc, argv);

    if(config.use_deamon)
    {
        daemon_run();
    }

    Server server;

    server.init(config.port, config.OPT_LINGER, config.max_thread);

    //server.log();
    
    server.thread_pool();

    server.listenfd();

    server.eventloop();

    return 0;
}