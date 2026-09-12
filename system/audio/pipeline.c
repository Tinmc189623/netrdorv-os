/* ============================================================================
 * NetrdorvOS - 音频处理管道实现
 * ============================================================================ */

#include <system/audio/pipeline.h>
#include <lib/string.h>

/* ---- 采样率转换（线性插值） ---- */
void src_init(src_state_t *s, u32 src_hz, u32 dst_hz)
{
    s->src_rate = src_hz;
    s->dst_rate = dst_hz;
    s->pos = 0;
    s->step = ((u64)src_hz << 16) / dst_hz;
}

size_t src_process(src_state_t *s, const s16 *in, size_t in_frames,
                   s16 *out, size_t out_max)
{
    size_t out_frames = 0;
    while (out_frames < out_max) {
        u32 idx = s->pos >> 16;
        u32 frac = s->pos & 0xFFFF;
        if (idx + 1 >= in_frames) break;
        /* 线性插值 L/R */
        for (int ch = 0; ch < 2; ch++) {
            s32 a = in[idx * 2 + ch];
            s32 b = in[(idx + 1) * 2 + ch];
            out[out_frames * 2 + ch] =
                (s16)(a + ((b - a) * (s32)frac >> 16));
        }
        out_frames++;
        s->pos += s->step;
    }
    return out_frames;
}

/* ---- 声道重映射 ---- */
void ch_remap(const s16 *in, s16 *out, size_t frames,
              ch_mode_t in_ch, ch_mode_t out_ch)
{
    if (in_ch == out_ch) {
        memcpy(out, in, frames * (in_ch == CH_STEREO ? 4 : 2));
        return;
    }
    if (in_ch == CH_STEREO && out_ch == CH_MONO) {
        for (size_t i = 0; i < frames; i++)
            out[i] = (in[i*2] + in[i*2+1]) / 2;
    } else {
        for (size_t i = 0; i < frames; i++) {
            out[i*2] = in[i];
            out[i*2+1] = in[i];
        }
    }
}

/* ---- PCM 格式 ---- */
void pcm_16to24(const s16 *in, u8 *out, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        s32 v = ((s32)in[i]) << 8;
        out[i*3]   = (u8)(v & 0xFF);
        out[i*3+1] = (u8)((v >> 8) & 0xFF);
        out[i*3+2] = (u8)((v >> 16) & 0xFF);
    }
}

void pcm_24to16(const u8 *in, s16 *out, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        s32 v = ((s32)in[i*3] | ((s32)in[i*3+1] << 8) |
                 ((s32)in[i*3+2] << 16));
        out[i] = (s16)(v >> 8);
    }
}

u16 pcm_swap16(u16 v) { return (v >> 8) | (v << 8); }
u32 pcm_swap32(u32 v) {
    return ((v & 0xFF) << 24) | ((v & 0xFF00) << 8) |
           ((v >> 8) & 0xFF00) | ((v >> 24) & 0xFF);
}

/* ---- 事件控制 ---- */
#define MAX_SUBSCRIBERS 8
static audio_event_cb_t event_cbs[MAX_SUBSCRIBERS];
static void *event_args[MAX_SUBSCRIBERS];

void audio_event_subscribe(audio_event_cb_t cb, void *arg)
{
    for (int i = 0; i < MAX_SUBSCRIBERS; i++) {
        if (!event_cbs[i]) {
            event_cbs[i] = cb;
            event_args[i] = arg;
            return;
        }
    }
}

void audio_event_dispatch(audio_event_t ev)
{
    for (int i = 0; i < MAX_SUBSCRIBERS; i++)
        if (event_cbs[i])
            event_cbs[i](ev, event_args[i]);
}

/* ---- 状态上报 ---- */
static audio_status_t current_status;
void audio_status_report(audio_status_t *st)
{
    if (st) *st = current_status;
}
