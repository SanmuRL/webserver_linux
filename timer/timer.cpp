#include "timer.h"

int *Util::u_pipefd = 0;
int Util::u_epollfd = 0;

void Util::addfd(int epollfd, int fd, int mode){
    epoll_event event;
    event.data.fd = fd;

    if(mode == 0)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
    else if(mode == 1)
        event.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnoblocking(fd);
}

int Util::setnoblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// void Util::addtimer(shared_timer_node timer_node){
//     util_locker.lock();
//     timer_queue.push(timer_node);
//     util_locker.unlock();
// }

// std::vector<int> Util::handle_expired_event(){
//     util_locker.lock();
//     timer_node::current_time();
//     std::vector<int> expired_fd;
//     while(!timer_queue.empty()){
//         shared_timer_node cur = timer_queue.top();
//         if(cur->isexpired() && (!cur->isdeleted())){
//             timer_queue.pop();
//             expired_fd.push_back(cur->user_data->sockfd);
//             cur->cb_timer(cur->user_data);
//             cur->user_data.reset();
//         }
//         else if(cur->isdeleted()){
//             timer_queue.pop();
//         }
//         else break;
//     }
//     util_locker.unlock();
//     return;
// }

void Util::addsig(int sig, void(handler)(int), bool restart){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if(restart){
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void Util::sig_handler(int sig){
    int save_errno = errno;
    int msg = sig;
    send(u_pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}


void cb_timer(httpconn conn)
{
    //避免超时处理和客户端关闭导致的重复删除
    if(conn.m_deleted) return;
    //epoll_ctl(Util::u_epollfd, EPOLL_CTL_DEL, conn.m_sockfd, 0);
    std::cout << "I AM HERE" << std::endl;
    conn.close_conn();
}