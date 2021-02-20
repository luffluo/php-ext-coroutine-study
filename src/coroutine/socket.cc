#include "socket.h"
#include "coroutine.h"
#include "coroutine_socket.h"

using study::Coroutine;
using study::coroutine::Socket;

Socket::Socket(int domain, int type, int protocol)
{
    int fd = st_socket_create(domain, type, protocol);
    if (fd < 0) {
        return;
    }

    st_socket_set_nonblock(fd);
}

int Socket::bind(int type, char *host, int port)
{
    return st_socket_bind(sockfd, type, host, port);
}

int Socket::listen()
{
    return st_socket_listen(sockfd);
}

int Socket::accept()
{
    int connfd = st_socket_accept(sockfd);

    if (connfd < 0 && errno == EAGAIN) {
        wait_event(ST_EVENT_READ);
        connfd = st_socket_accept(sockfd);
    }

    return connfd;
}

bool Socket::wait_event(int event)
{
    long cid;
    Coroutine *co;
    epoll_event *ev;

    co = Coroutine::get_current();
    cid = co->get_cid();

    ev = study_g.poll->events;
    
    ev->events = event == ST_EVENT_READ ? EPOLLIN : EPOLLOUT;
    ev->data.u64 = touint64(sockfd, cid);
    epoll_ctl(study_g.poll->epollfd, EPOLL_CTL_ADD, sockfd, ev);

    co->yield();

    return true;
}