/* ============================================================================
 * NetrdorvOS - 音频驱动 (阶段4：I2S + ES8311)
 * ============================================================================ */

#ifndef __INCLUDE_DRIVER_AUDIO_H__
#define __INCLUDE_DRIVER_AUDIO_H__

#include <types.h>

typedef enum {
    AUDIO_SAMPLE_16BIT = 16,
    AUDIO_SAMPLE_24BIT = 24,
} audio_sample_bits_t;

typedef struct {
    u32 sample_rate;            /* 采样率：8000~48000 */
    u32 channels;               /* 声道数：1/2 */
    audio_sample_bits_t bits;   /* 位深 */
} audio_format_t;

/* 初始化 I2S 总线 + ES8311 编解码 */
int audio_init(const audio_format_t *fmt);

/* 设置音量（0~100） */
int audio_set_volume(u8 vol);

/* 播放一段 PCM 数据（阻塞式，供阶段4调试） */
int audio_play_pcm(const void *pcm, u32 bytes);

/* 开始/停止流式播放（阶段5 边下边播） */
int audio_stream_start(const audio_format_t *fmt);
int audio_stream_feed(const void *pcm, u32 bytes);   /* 送入播放缓冲 */
int audio_stream_stop(void);

#endif /* __INCLUDE_DRIVER_AUDIO_H__ */
