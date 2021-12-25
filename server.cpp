#include "server.h"
#include <iostream>

Server::Server()
{
    //root文件夹路径
    char server_path[200];
    getcwd(server_path, 200);
    char root[6] = "/root";
    m_root = (char *)malloc(strlen(server_path) + strlen(root) + 1);
    strcpy(m_root, server_path);
    strcat(m_root, root);
}

Server::~Server()
{
    close(m_epollfd);
    close(m_listenfd);
    close(m_pipe[1]);
    close(m_pipe[0]);
    delete m_threadpool;
}

void Server::init(int port, int opt_linger, int thread_num)
{
    m_port = port;
    m_thread_size = thread_num;
    m_OPT_LINGER = opt_linger;
}

void Server::thread_pool()
{
    m_threadpool = new threadpool(m_thread_size, m_request_queue_size);
}

void Server::listenfd()
{
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);

    if(m_OPT_LINGER == 0)
    {
        struct linger tmp = {0, 1};
        setsockopt(m_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }
    else
    {
        struct linger tmp = {1, 1};
        setsockopt(m_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }
    
    ret = bind(m_listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(m_listenfd, 5);
    assert(ret >= 0);

    m_epollfd = epoll_create(5);
    assert(m_epollfd != -1);

    util.addfd(m_epollfd, m_listenfd, 1);
    httpconn::m_epollfd = m_epollfd;

    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, m_pipe);
    assert(ret != -1);
    util.setnoblocking(m_pipe[1]);
    util.addfd(m_epollfd, m_pipe[0], 1);

    util.addsig(SIGPIPE, SIG_IGN);
    util.addsig(SIGTERM, util.sig_handler);

    Util::u_epollfd = m_epollfd;
    Util::u_pipefd = m_pipe;
}

void Server::dealclientdata()
{
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);

    while(1)
    {
        int connfd = accept(m_listenfd, (struct sockaddr *)&client_address, &client_addrlength);
        //std::cout << "connfd: " << connfd << std::endl;
        if(connfd < 0)
        {
            break;
        }
        if(httpconn::m_user_count >= MAX_FD)
        {
            std::cout << "too many users" << std::endl;
            break;
        }
        timer(connfd, client_address);
    }
}

void Server::timer(int connfd, struct sockaddr_in client_address)
{
    //为新建立的连接构造httpconn实例，并且记载在user中
    //std::cout << "will add" << std::endl;
    //users[connfd] = std::make_shared<httpconn>();
    users[connfd].init(connfd, client_address, m_root);
    //初始化client_data数据
    //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
    // users_timer[connfd] = std::make_shared<Client_data>();
    // users_timer[connfd]->address = client_address;
    // users_timer[connfd]->sockfd = connfd;
    // std::shared_ptr<timer_node> timer(new timer_node(TIMESLOT));
    // timer->user_data = users_timer[connfd];
    // timer->cb_timer = cb_timer;
    // users_timer[connfd]->timer = timer;
    timerheap.add(connfd, timeoutMS, std::bind(cb_timer, users[connfd]));
}

// void Server::deal_timer(std::shared_ptr<timer_node> timer)
// {
//     timer->deleted();
//     timer->user_data.reset();
//     users_timer.erase(timer->user_data->sockfd);
//     users.erase(timer->user_data->sockfd);
// }

void Server::dealwithsignal(bool& stop_server)
{
    int ret = 0;
    int sig;
    char signals[1024];
    ret = recv(m_pipe[0], signals, sizeof(signals), 0);
    if(ret == -1){
        return;
    }
    else if(ret == 0){
        return;
    }
    else{
        for(int i = 0; i<ret; ++i){
            switch(signals[i])
            {
                case SIGTERM:
                {
                    stop_server = true;
                    break;
                }
            }
        }
    }
}

void Server::dealwithread(int sockfd)
{
    // auto timer = users_timer[sockfd]->timer;
    // adjust_timer(timer);
    timerheap.adjust(sockfd, timeoutMS);
    m_threadpool->append(&users[sockfd], 0);

    while(true)
    {
        if(users[sockfd].improv == 1)
        {
            if(users[sockfd].time_flager == 1)
            {
                //deal_timer(timer);
                timerheap.dowork(sockfd);
                break;
            }
            users[sockfd].improv = 0;
            break;
        }
    }
}

void Server::dealwithwrite(int sockfd)
{
    //auto timer = users_timer[sockfd]->timer;
    //adjust_timer(timer);
    timerheap.adjust(sockfd, timeoutMS);
    m_threadpool->append(&users[sockfd], 1);

    while(true)
    {
        if(users[sockfd].improv == 1)
        {
            if(users[sockfd].time_flager == 1)
            {
                //deal_timer(timer);
                timerheap.dowork(sockfd);
                break;
            }
            users[sockfd].improv = 0;
            break;
        }
    } 
}

// void Server::adjust_timer(std::shared_ptr<timer_node> timer)
// {
//     int connfd = timer->user_data->sockfd;
    
//     std::shared_ptr<timer_node> timer(new timer_node(TIMESLOT));
//     timer->user_data = users_timer[connfd];
//     timer->cb_timer = cb_timer;
//     users_timer[connfd]->timer->deleted_ = true;
//     users_timer[connfd]->timer->user_data.reset();
//     users_timer[connfd]->timer.reset();
//     users_timer[connfd]->timer = timer;
//     util.addtimer(timer);
// }

void Server::eventloop()
{
    int timems = -1;
    bool stop_server = false;

    while(!stop_server)
    {
        timems = timerheap.nextTick();
        //std::cout << "waiting time: " << timems << std::endl;
        int number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, timems);
        if(number < 0 && errno != EINTR)
        {
            //std::cout << "epoll wait failed" << std::endl;
            break;
        }

        for(int i = 0; i < number; ++i)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == m_listenfd)
            {
                dealclientdata();
            }
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                //std::shared_ptr<timer_node> timer = users_timer[sockfd]->timer;
                //deal_timer(timer);
                //std::cout << "客户端中断" << std::endl;
                timerheap.dowork(sockfd);
            }
            else if((sockfd == m_pipe[0]) && (events[i].events & EPOLLIN))
            {
                dealwithsignal(stop_server);
            }
            else if(events[i].events & EPOLLIN)
            {
                //std::cout << "in" << sockfd << std::endl;
                dealwithread(sockfd);
            }
            else if(events[i].events & EPOLLOUT)
            {
                //std::cout << "out" << sockfd << std::endl;
                dealwithwrite(sockfd);
            }
        }
        //std::vector<int> expired_fd = util.handle_expired_event();
        // for(int expiredfd : expired_fd){
        //     users_timer.erase(expiredfd);
        //     users.erase(expiredfd);
        // }
    }
}
