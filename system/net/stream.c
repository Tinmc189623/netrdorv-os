/* ============================================================================
 * NetrdorvOS - 网络流读取器实现
 * ============================================================================ */

#include <system/net/stream.h>
#include <system/socket.h>
#include <driver/uart.h>
#include <lib/string.h>

int stream_open(stream_t *s, const char *url)
{
    memset(s, 0, sizeof(*s));
    strncpy(s->url, url, sizeof(s->url) - 1);

    s->sockfd = sys_socket(AF_INET, SOCK_STREAM, 0);
    if (s->sockfd < 0) return -1;

    /* TODO: 解析 URL → DNS → connect → HTTP GET */
    kprintf("[stream] opening %s\n", url);
    return 0;
}

int stream_read(stream_t *s, u8 *buf, size_t len)
{
    int n = sys_recv(s->sockfd, buf, len, 0);
    if (n > 0) s->bytes_read += (size_t)n;
    return n;
}

void stream_close(stream_t *s)
{
    sys_close_socket(s->sockfd);
    s->sockfd = -1;
}
