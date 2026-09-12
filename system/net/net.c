/* ============================================================================
 * NetrdorvOS - 网络协议栈 (阶段5)
 * 占位实现：完成接口框架，协议栈实现阶段5完成
 * ============================================================================ */

#include <system/net.h>
#include <types.h>

int net_init(void)
{
    /* TODO(阶段5): 初始化 TCP/IP 协议栈、ARP 缓存 */
    return 0;
}

int http_get(const char *url, http_response_t *resp)
{
    /* TODO(阶段5): DNS + TCP + HTTP 请求解析 */
    (void)url; (void)resp;
    return -1;
}

int tcp_connect(ip4_t ip, u16 port)
{
    /* TODO(阶段5): 三次握手 */
    (void)ip; (void)port;
    return -1;
}

int tcp_send(int fd, const void *data, u32 len)
{
    /* TODO(阶段5) */
    (void)fd; (void)data; (void)len;
    return -1;
}

int tcp_recv(int fd, void *buf, u32 len)
{
    /* TODO(阶段5) */
    (void)fd; (void)buf; (void)len;
    return -1;
}

void tcp_close(int fd)
{
    /* TODO(阶段5) */
    (void)fd;
}
