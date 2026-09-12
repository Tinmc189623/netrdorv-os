/* ============================================================================
 * NetrdorvOS - 音频处理管道各模块
 * 采样率转换 / 声道重映射 / PCM格式转换 / 事件控制 / 状态上报
 * ============================================================================ */

#ifndef __SYSTEM_AUDIO_PIPELINE_H__
#define __SYSTEM_AUDIO_PIPELINE_H__

#include <types.h>

/* ---- 采样率转换 ---- */
typedef struct {
    u32 src_rate;
    u32 dst_rate;
    u32 pos;        /* 分数位置 */
    u32 step;       /* src/dst 比例 * 2^16 */
} src_state_t;

void src_init(src_state_t *s, u32 src_hz, u32 dst_hz);
/* 输入 16bit 立体声，输出重采样后样本数 */
size_t src_process(src_state_t *s, const s16 *in, size_t in_frames,
                   s16 *out, size_t out_max);

/* ---- 声道重映射 ---- */
typedef enum { CH_MONO, CH_STEREO } ch_mode_t;
/* stereo→mono 或 mono→stereo */
void ch_remap(const s16 *in, s16 *out, size_t frames,
              ch_mode_t in_ch, ch_mode_t out_ch);

/* ---- PCM 格式转换 ---- */
/* 16bit LE → 24bit LE */
void pcm_16to24(const s16 *in, u8 *out, size_t samples);
/* 24bit LE → 16bit LE */
void pcm_24to16(const u8 *in, s16 *out, size_t samples);
/* 大小端转换 */
u16  pcm_swap16(u16 v);
u32  pcm_swap32(u32 v);

/* ---- 事件控制 ---- */
typedef enum {
    EV_PLAY = 1,
    EV_PAUSE,
    EV_STOP,
    EV_NEXT,
    EV_PREV,
    EV_VOLUME,
    EV_BUFFER_UNDERRUN,
    EV_DEVICE_ERROR,
} audio_event_t;

typedef void (*audio_event_cb_t)(audio_event_t ev, void *arg);
void audio_event_subscribe(audio_event_cb_t cb, void *arg);
void audio_event_dispatch(audio_event_t ev);

/* ---- 日志/状态上报 ---- */
typedef struct {
    u32    state;        /* 0=idle 1=playing 2=paused */
    u32    volume;
    size_t buf_watermark;  /* 环形缓冲水位 */
    u32    decode_errors;
    u32    underruns;
} audio_status_t;

void audio_status_report(audio_status_t *st);

#endif
