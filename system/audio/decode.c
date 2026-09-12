/* ============================================================================
 * NetrdorvOS - 音频解码器 (阶段4)
 * 占位实现：完成接口框架，解码器实现阶段4完成
 * ============================================================================ */

#include <system/audio_decode.h>
#include <types.h>

struct decoder {
    decoder_type_t type;
    /* TODO(阶段4): 解码器内部状态（帧头解析、位缓冲、合成滤波） */
};

decoder_t *decoder_create(decoder_type_t type)
{
    /* TODO(阶段4): 分配并初始化解码器 */
    (void)type;
    return (decoder_t *)0;
}

void decoder_destroy(decoder_t *dec)
{
    /* TODO(阶段4) */
    (void)dec;
}

int decoder_feed(decoder_t *dec, const void *in, u32 in_len,
                 void *out, u32 out_cap)
{
    /* TODO(阶段4): 流式分片解码 MP3/AAC → PCM */
    (void)dec; (void)in; (void)in_len; (void)out; (void)out_cap;
    return -1;
}

int decoder_get_format(decoder_t *dec, audio_format_t *fmt)
{
    /* TODO(阶段4) */
    (void)dec; (void)fmt;
    return -1;
}
