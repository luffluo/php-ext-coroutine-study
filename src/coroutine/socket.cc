#include "socket.h"
#include "coroutine_socket.h"

using study::coroutine::Socket;

Socket::Socket(int domain, int type, int protocol)
{
    int fd = st_socket_create(domain, type, protocol);
    if (fd < 0) {
        return;
    }

    st_socket_set_nonblock(fd);
}