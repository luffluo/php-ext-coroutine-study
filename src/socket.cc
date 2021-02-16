#include "socket.h"
#include "log.h"

int st_socket_create(int type)
{
    int _domain;
    int _type;

    if (ST_SOCK_TCP == type) {
        _domain = AF_INET;
        _type = SOCK_STREAM;
    } else if (ST_SOCK_UDP == type) {
        _domain = AF_INET;
        _type = SOCK_DGRAM;
    } else {
        return -1;
    }

    return socket(_domain, _type, 0);
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