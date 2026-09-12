/* ============================================================================
 * NetrdorvOS - Socket API
 * 套接字接口：socket/bind/connect/send/recv/close
 * 实际网络栈由 WiFi 驱动 + lwIP 接入
 * ============================================================================ */

#ifndef __SYSTEM_SOCKET_H__
#define __SYSTEM_SOCKET_H__

#include <types.h>

#define AF_INET     2
#define SOCK_STREAM 1
#define SOCK_DGRAM  2

typedef struct {
    u32 s_addr;
} in_addr_t;

typedef struct {
    u16 sin_family;
    u16 sin_port;
    in_addr_t sin_addr;
} sockaddr_in_t;

int sys_socket(int domain, int type, int protocol);
int sys_connect(int sockfd, const sockaddr_in_t *addr, size_t len);
int sys_send(int sockfd, const void *buf, size_t len, int flags);
int sys_recv(int sockfd, void *buf, size_t len, int flags);
int sys_close_socket(int sockfd);

#endif
