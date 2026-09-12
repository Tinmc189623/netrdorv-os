/* ============================================================================
 * NetrdorvOS - 环形缓冲区实现
 * ============================================================================ */

#include <system/audio/ringbuf.h>
#include <lib/string.h>

void ringbuf_init(ringbuf_t *rb, void *buf, size_t size)
{
    rb->buf = (u8 *)buf;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
}

size_t ringbuf_write(ringbuf_t *rb, const void *data, size_t len)
{
    size_t free = ringbuf_free(rb);
    if (len > free) len = free;
    size_t first = rb->size - rb->head;
    if (len <= first) {
        memcpy(rb->buf + rb->head, data, len);
    } else {
        memcpy(rb->buf + rb->head, data, first);
        memcpy(rb->buf, (const u8 *)data + first, len - first);
    }
    rb->head = (rb->head + len) % rb->size;
    return len;
}

size_t ringbuf_read(ringbuf_t *rb, void *data, size_t len)
{
    size_t avail = ringbuf_avail(rb);
    if (len > avail) len = avail;
    size_t first = rb->size - rb->tail;
    if (len <= first) {
        memcpy(data, rb->buf + rb->tail, len);
    } else {
        memcpy(data, rb->buf + rb->tail, first);
        memcpy((u8 *)data + first, rb->buf, len - first);
    }
    rb->tail = (rb->tail + len) % rb->size;
    return len;
}

size_t ringbuf_avail(const ringbuf_t *rb)
{
    return (rb->head - rb->tail + rb->size) % rb->size;
}

size_t ringbuf_free(const ringbuf_t *rb)
{
    return rb->size - ringbuf_avail(rb) - 1;
}

void ringbuf_reset(ringbuf_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
}
