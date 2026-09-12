/* ============================================================================
 * NetrdorvOS - MIPI 显示驱动 (/dev/fb0)
 * 驱动硬件实现由开源社区提供，此处注册 VFS 设备接口
 * ============================================================================ */

#include <driver/display.h>
#include <kernel/vfs.h>
#include <kernel/module.h>
#include <driver/uart.h>
#include <types.h>
#include <lib/string.h>

static display_info_t disp = {
    .width    = DISPLAY_DEFAULT_WIDTH,
    .height   = DISPLAY_DEFAULT_HEIGHT,
    .bpp      = DISPLAY_DEFAULT_BPP,
    .stride   = DISPLAY_DEFAULT_WIDTH * DISPLAY_DEFAULT_BPP / 8,
    .fb_base  = (void *)0,
};

/* ---- VFS file_ops ---- */
static int fb_open(const char *path, int flags)
{
    (void)path; (void)flags;
    return 0;
}

static int fb_read(int fd, void *buf, size_t n)
{
    (void)fd; (void)buf; (void)n;
    return 0;  /* 帧缓冲不可读 */
}

static int fb_write(int fd, const void *buf, size_t n)
{
    (void)fd;
    memcpy(disp.fb_base, buf, n);
    return (int)n;
}

static int fb_ioctl(int fd, unsigned long req, void *arg)
{
    (void)fd;
    u64 *a = (u64 *)arg;
    switch (req) {
    case 0x4010:  /* fill_rect */
        if (disp.fb_base) {
            u32 x = a[0], y = a[1], w = a[2], h = a[3], color = a[4];
            for (u32 yy = y; yy < y + h && yy < disp.height; yy++)
                for (u32 xx = x; xx < x + w && xx < disp.width; xx++)
                    ((u16 *)disp.fb_base)[yy * disp.width + xx] = (u16)color;
        }
        break;
    case 0x4011:  /* draw_text (stub: 用户态字库) */
        break;
    }
    return 0;
}

static int fb_close(int fd) { (void)fd; return 0; }

static file_ops_t fb_ops = {
    .open  = fb_open,
    .read  = fb_read,
    .write = fb_write,
    .ioctl = fb_ioctl,
    .close = fb_close,
};

/* 模块初始化（由 module_init_all 调用） */
static int display_module_init(void)
{
    return display_init();
}

static void display_module_exit(void) {}

int display_init(void)
{
    kprintf("[display] registered /dev/fb0 (%lux%u@%ubpp)\n",
            (unsigned)disp.width, (unsigned)disp.height,
            (unsigned)disp.bpp);
    return 0;
}

void display_backlight(int on) { (void)on; }
void display_flush(void *src) { (void)src; }
const display_info_t *display_get_info(void) { return &disp; }

/* 注册为内置模块 */
MODULE_DEFINE(fb0, display_module_init, display_module_exit, &fb_ops);
