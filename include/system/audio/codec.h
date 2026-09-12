/* ============================================================================
 * NetrdorvOS - 音频解码器接口
 * MP3 / AAC 解码器由用户态库 audio_codec.so 提供
 * 内核仅定义统一接口
 * ============================================================================ */

#ifndef __SYSTEM_AUDIO_CODEC_H__
#define __SYSTEM_AUDIO_CODEC_H__

#include <types.h>

typedef enum {
    CODEC_MP3,
    CODEC_AAC,
    CODEC_FLAC,
} codec_type_t;

typedef struct {
    codec_type_t type;
    u32 sample_rate;
    u32 channels;
    u32 bitrate;
} codec_info_t;

typedef struct codec {
    int  (*open)(codec_type_t type);
    int  (*decode)(struct codec *c, const u8 *in, size_t in_len,
                   s16 *out, size_t out_max, size_t *consumed);
    void (*close)(struct codec *c);
    codec_info_t info;
} codec_t;

codec_t *codec_create(codec_type_t type);
void     codec_destroy(codec_t *c);

#endif
