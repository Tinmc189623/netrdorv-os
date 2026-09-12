/* ============================================================================
 * NetrdorvOS - I2S + ES8311 音频驱动 (阶段4)
 * 占位实现：完成接口框架，驱动实现阶段4完成
 * ============================================================================ */

#include <driver/audio.h>
#include <types.h>

int audio_init(const audio_format_t *fmt)
{
    /* TODO(阶段4): I2S 总线初始化 + ES8311 寄存器配置 */
    (void)fmt;
    return 0;
}

int audio_set_volume(u8 vol)
{
    /* TODO(阶段4): ES8311 音量寄存器 */
    (void)vol;
    return 0;
}

int audio_play_pcm(const void *pcm, u32 bytes)
{
    /* TODO(阶段4): 阻塞式播放 PCM */
    (void)pcm; (void)bytes;
    return 0;
}

int audio_stream_start(const audio_format_t *fmt)
{
    /* TODO(阶段4/5): 启动流式播放通道 */
    (void)fmt;
    return 0;
}

int audio_stream_feed(const void *pcm, u32 bytes)
{
    /* TODO(阶段4/5): 送入播放缓冲队列 */
    (void)pcm; (void)bytes;
    return 0;
}

int audio_stream_stop(void)
{
    /* TODO(阶段4/5) */
    return 0;
}
