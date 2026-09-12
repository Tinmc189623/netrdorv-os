/* ============================================================================
 * NetrdorvOS - 网络协议栈 (阶段5：TCP/IP + HTTP 客户端)
 * ============================================================================ */

#ifndef __INCLUDE_SYSTEM_NET_H__
#define __INCLUDE_SYSTEM_NET_H__

#include <types.h>

/* ---------- IP 地址 ---------- */
typedef struct {
    u8 addr[4];
} ip4_t;

static inline ip4_t ip4_make(u8 a, u8 b, u8 c, u8 d)
{
    ip4_t ip = { {a, b, c, d} };
    return ip;
}

/* ---------- HTTP 客户端 ---------- */
typedef struct {
    int     status_code;        /* HTTP 状态码，如 200 */
    size_t  body_len;           /* 正文长度（可能为 0） */
    void   *body;               /* 正文缓冲区（调用方提供） */
    size_t  body_cap;           /* 缓冲区容量 */
} http_response_t;

/* 初始化网络栈 */
int net_init(void);

/* HTTP GET 请求（阶段5：获取网络电台 API） */
int http_get(const char *url, http_response_t *resp);

/* 打开 TCP 连接（阶段5：流式拉取） */
int tcp_connect(ip4_t ip, u16 port);
int tcp_send(int fd, const void *data, u32 len);
int tcp_recv(int fd, void *buf, u32 len);
void tcp_close(int fd);

#endif /* __INCLUDE_SYSTEM_NET_H__ */
