#include "log.h"
#include "socket.h"
#include "coroutine.h"
#include "coroutine_socket.h"

using namespace std;
using study::Coroutine;
using study::coroutine::Socket;

char *Socket::read_buffer = nullptr;
size_t Socket::read_buffer_len = 0;

char *Socket::write_buffer = nullptr;
size_t Socket::write_buffer_len = 0;

Socket::Socket(int domain, int type, int protocol)
{
    sockfd = st_socket_create(domain, type, protocol);
    if (sockfd < 0) {
        return;
    }

    st_socket_set_nonblock(sockfd);
}

Socket::Socket(int fd)
{
    sockfd = fd;
    st_socket_set_nonblock(sockfd);
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
    int connfd;

    do {
        connfd = st_socket_accept(sockfd);
    } while (connfd < 0 && errno == EAGAIN && wait_event(ST_EVENT_READ));

    return connfd;
}

ssize_t Socket::recv(void *buf, size_t len)
{
    int ret;

    do {
        ret = st_socket_recv(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(ST_EVENT_READ));

    return ret;
}

ssize_t Socket::send(const void *buf, size_t len)
{
    int ret;

    do {
        ret = st_socket_send(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(ST_EVENT_WRITE));

    return ret;
}

bool Socket::wait_event(int event)
{
    long cid;
    Coroutine *co;
    epoll_event *ev;

    co = Coroutine::get_current();
    cid = co->get_cid();

    if (!study_g.poll) {
        init_st_poll();
    }

    ev = study_g.poll->events;
    
    ev->events = event == ST_EVENT_READ ? EPOLLIN : EPOLLOUT;
    ev->data.u64 = touint64(sockfd, cid);
    epoll_ctl(study_g.poll->epollfd, EPOLL_CTL_ADD, sockfd, ev);
    (study_g.poll->event_num)++;

    co->yield();

    if (epoll_ctl(study_g.poll->epollfd, EPOLL_CTL_DEL, sockfd, NULL) < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return false;
    }

    return true;
}

int Socket::close()
{
    return st_socket_close(sockfd);
}

Socket::~Socket()
{

}

int Socket::init_read_buffer()
{
    if (!read_buffer) {
        try {
            read_buffer = new char[65536];
        } catch(const std::bad_alloc& e) {
            st_error("%s", e.what());
        }

        read_buffer_len = 65536;
    }

    return 0;
}

int Socket::init_write_buffer()
{
    if (!write_buffer) {
        try {
            write_buffer = new char[65536];
        } catch(const std::bad_alloc& e) {
            st_error("%s", e.what());
        }

        write_buffer_len = 65536;
    }

    return 0;
}