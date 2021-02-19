#include "socket.h"
#include "log.h"

int st_socket_create(int domain, int type, int protocol)
{
    int fd;
    
    fd = socket(domain, type, protocol);

    if (fd < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }

    return fd;
}

int st_socket_set_nonblock(int sockfd)
{
    int flags;

    flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }

    flags = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    if (flags < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }

    return 0;
}

int st_socket_bind(int sockfd, int type, char *host, int port)
{
    int ret;
    struct sockaddr_in servaddr;

    if (ST_SOCK_TCP == type) {
        bzero(&servaddr, sizeof(servaddr));
        inet_aton(host, &(servaddr.sin_addr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        ret = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        if (ret < 0) {
            return -1;
        }
    } else {
        return -1;
    }

    return ret;
}

int st_socket_listen(int sockfd)
{
    int ret;

    ret = listen(sockfd, 512);

    if (ret < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }

    return ret;
}

int st_socket_accept(int sockfd)
{
    int connfd;
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);

    connfd = accept(sockfd, (struct sockaddr *) &sa, &len);

    return connfd;
}

ssize_t st_socket_recv(int sockfd, void *buf, size_t len, int flag)
{
    ssize_t ret;

    ret = recv(sockfd, buf, len, flag);

    if (ret < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }

    return ret;
}

ssize_t st_socket_send(int sockfd, void *buf, size_t len, int flag)
{
    ssize_t ret;

    ret = send(sockfd, buf, len, flag);

    if (ret < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }

    return ret;
}