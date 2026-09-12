/* ============================================================================
 * NetrdorvOS / Nexera Kernel - 环形缓冲区模块
 * FIFO 队列，分离网络读取/解码/音频输出三线程
 * ============================================================================ */

#ifndef __SYSTEM_AUDIO_RINGBUF_H__
#define __SYSTEM_AUDIO_RINGBUF_H__

#include <types.h>

typedef struct {
    u8     *buf;
    size_t  size;
    size_t  head;    /* 写位置 */
    size_t  tail;    /* 读位置 */
} ringbuf_t;

void ringbuf_init(ringbuf_t *rb, void *buf, size_t size);
size_t ringbuf_write(ringbuf_t *rb, const void *data, size_t len);
size_t ringbuf_read(ringbuf_t *rb, void *data, size_t len);
size_t ringbuf_avail(const ringbuf_t *rb);   /* 可读字节数 */
size_t ringbuf_free(const ringbuf_t *rb);     /* 可写字节数 */
void   ringbuf_reset(ringbuf_t *rb);

#endif
