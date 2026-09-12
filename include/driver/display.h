/* ============================================================================
 * NetrdorvOS - 显示驱动接口 (阶段3)
 * 驱动实现由开源社区提供，此处只定义接口
 * ============================================================================ */

#ifndef __INCLUDE_DRIVER_DISPLAY_H__
#define __INCLUDE_DRIVER_DISPLAY_H__

#include <types.h>

/* 默认屏幕配置（用户按实际屏改，GUI 会自动适配） */
#define DISPLAY_DEFAULT_WIDTH   480
#define DISPLAY_DEFAULT_HEIGHT  800
#define DISPLAY_DEFAULT_BPP     16

typedef struct {
    u32 width;
    u32 height;
    u32 bpp;
    u32 stride;
    void *fb_base;
} display_info_t;

/* 初始化显示驱动（由开源社区驱动实现） */
int display_init(void);

/* 设置背光 */
void display_backlight(int on);

/* 刷新一帧（如有双缓冲） */
void display_flush(void *src);

/* 获取显示信息 */
const display_info_t *display_get_info(void);

#endif /* __INCLUDE_DRIVER_DISPLAY_H__ */
