/* ============================================================================
 * NetrdorvOS - 极简 GUI 引擎 (阶段3 基础版)
 *
 * 设计原则：
 *   - 分辨率不写死，由 display_config 决定
 *   - 用户可在 config.h 或运行时调整 width/height 适配不同屏幕
 *   - 只提供最基础的画点/线/矩形/文字，更多 UI 组件由用户扩展
 * ============================================================================ */

#ifndef __INCLUDE_SYSTEM_GUI_H__
#define __INCLUDE_SYSTEM_GUI_H__

#include <types.h>
#include <driver/display.h>

/* ---------- 颜色 (RGB565) ---------- */
typedef u16 color_t;

#define RGB565(r, g, b) \
    ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

#define COLOR_BLACK     RGB565(0, 0, 0)
#define COLOR_WHITE     RGB565(255, 255, 255)
#define COLOR_RED       RGB565(255, 0, 0)
#define COLOR_GREEN     RGB565(0, 255, 0)
#define COLOR_BLUE      RGB565(0, 0, 255)
#define COLOR_GRAY      RGB565(128, 128, 128)

/* ---------- 显示配置（用户可调，适配不同屏幕） ---------- */
typedef struct {
    u32 width;          /* 屏幕宽（像素），用户按实际屏改 */
    u32 height;         /* 屏幕高（像素） */
    u32 stride;        /* 每行字节数 = width * bpp/8 */
    u32 bpp;            /* 每像素位数，默认 16 */
    void *fb;           /* 帧缓冲基址（由显示驱动设置） */
} gui_display_t;

/* ---------- 全局显示配置（用户可在调用 gui_init 前修改） ---------- */
extern gui_display_t gui_disp;

/* ---------- 基础绘图 API（坐标系基于 gui_disp.width/height） ---------- */

/* 初始化 GUI（绑定帧缓冲） */
void gui_init(void *fb, u32 width, u32 height, u32 bpp);

/* 画点 */
void gui_draw_pixel(u32 x, u32 y, color_t c);

/* 画线（Bresenham） */
void gui_draw_line(u32 x0, u32 y0, u32 x1, u32 y1, color_t c);

/* 填充矩形 */
void gui_fill_rect(u32 x, u32 y, u32 w, u32 h, color_t c);

/* 描边矩形 */
void gui_draw_rect(u32 x, u32 y, u32 w, u32 h, color_t c);

/* 画一个字符（8x16 点阵，内置 ASCII） */
void gui_draw_char(u32 x, u32 y, char ch, color_t c);

/* 画字符串 */
void gui_draw_text(u32 x, u32 y, const char *str, color_t c);

/* 清屏 */
void gui_clear(color_t c);

/* 全屏刷新到帧缓冲（如有双缓冲由显示驱动处理） */
void gui_flush(void);

#endif /* __INCLUDE_SYSTEM_GUI_H__ */
