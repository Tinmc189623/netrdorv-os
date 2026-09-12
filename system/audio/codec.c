/* ============================================================================
 * NetrdorvOS - 解码器实现占位
 * TODO: 接入 minimp3 / libaac
 * ============================================================================ */

#include <system/audio/codec.h>
#include <kernel/mm.h>
#include <driver/uart.h>

static int null_open(codec_type_t type)
{
    kprintf("[codec] open type=%d (TODO: real decoder)\n", type);
    return 0;
}

static int null_decode(struct codec *c, const u8 *in, size_t in_len,
                       s16 *out, size_t out_max, size_t *consumed)
{
    (void)c; (void)in; (void)in_len; (void)out; (void)out_max;
    *consumed = 0;
    return 0;
}

static void null_close(struct codec *c) { (void)c; }

codec_t *codec_create(codec_type_t type)
{
    codec_t *c = kmalloc(sizeof(codec_t));
    if (!c) return NULL;
    c->open = null_open;
    c->decode = null_decode;
    c->close = null_close;
    c->info.type = type;
    return c;
}

void codec_destroy(codec_t *c)
{
    if (c) kfree(c);
}
