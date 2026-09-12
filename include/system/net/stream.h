/* ============================================================================
 * NetrdorvOS - 网络流读取器
 * 通过 socket 读取 HTTP/HLS 音频流，分段缓冲
 * ============================================================================ */

#ifndef __SYSTEM_NET_STREAM_H__
#define __SYSTEM_NET_STREAM_H__

#include <types.h>
#include <system/socket.h>

typedef struct {
    int      sockfd;
    char     url[256];
    u32      http_status;
    size_t   content_length;
    size_t   bytes_read;
} stream_t;

int  stream_open(stream_t *s, const char *url);
int  stream_read(stream_t *s, u8 *buf, size_t len);
void stream_close(stream_t *s);

#endif
