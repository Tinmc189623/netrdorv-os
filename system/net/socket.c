/* ============================================================================
 * NetrdorvOS - Socket API 实现（骨架）
 * TODO: 接入 lwIP，当前为占位
 * ============================================================================ */

#include <system/socket.h>
#include <driver/uart.h>

#define MAX_SOCKETS 8
static int socket_used[MAX_SOCKETS];

int sys_socket(int domain, int type, int protocol)
{
    (void)domain; (void)type; (void)protocol;
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (!socket_used[i]) {
            socket_used[i] = 1;
            kprintf("[socket] created fd=%d\n", i);
            return i;
        }
    }
    return -1;
}

int sys_connect(int sockfd, const sockaddr_in_t *addr, size_t len)
{
    (void)sockfd; (void)len;
    kprintf("[socket] connect %lu.%lu.%lu.%lu:%u\n",
            (u8)(addr->sin_addr.s_addr),
            (u8)(addr->sin_addr.s_addr >> 8),
            (u8)(addr->sin_addr.s_addr >> 16),
            (u8)(addr->sin_addr.s_addr >> 24),
            (unsigned)__builtin_bswap16(addr->sin_port));
    return 0;
}

int sys_send(int sockfd, const void *buf, size_t len, int flags)
{
    (void)sockfd; (void)buf; (void)flags;
    return (int)len;
}

int sys_recv(int sockfd, void *buf, size_t len, int flags)
{
    (void)sockfd; (void)buf; (void)flags;
    return 0;
}

int sys_close_socket(int sockfd)
{
    if (sockfd >= 0 && sockfd < MAX_SOCKETS)
        socket_used[sockfd] = 0;
    return 0;
}
