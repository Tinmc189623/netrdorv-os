/* ============================================================================
 * NetrdorvOS - 音频解码 (阶段4：MP3/AAC 轻量流式解码器)
 * ============================================================================ */

#ifndef __INCLUDE_SYSTEM_AUDIO_DECODE_H__
#define __INCLUDE_SYSTEM_AUDIO_DECODE_H__

#include <types.h>
#include <driver/audio.h>

typedef enum {
    DECODER_MP3 = 0,
    DECODER_AAC,
} decoder_type_t;

/* 解码器句柄 */
typedef struct decoder decoder_t;

/* 创建/销毁解码器 */
decoder_t *decoder_create(decoder_type_t type);
void decoder_destroy(decoder_t *dec);

/* 输入压缩数据（流式分片），输出 PCM 到 out
 * 返回写入的 PCM 字节数；-1 表示需要更多输入数据 */
int decoder_feed(decoder_t *dec, const void *in, u32 in_len,
                 void *out, u32 out_cap);

/* 获取解码后音频格式 */
int decoder_get_format(decoder_t *dec, audio_format_t *fmt);

#endif /* __INCLUDE_SYSTEM_AUDIO_DECODE_H__ */
