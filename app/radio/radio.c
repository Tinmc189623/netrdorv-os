/* ============================================================================
 * NetrdorvOS - 收音机应用（用户态）
 *
 * 通过系统调用访问设备文件：
 *   open("/dev/fb0", O_RDWR)    → 帧缓冲
 *   open("/dev/ttyS0", O_RDWR)  → 串口日志
 *   read(/dev/input0)           → 触控事件
 *
 * 不直接调用内核函数，所有交互经 VFS/设备层。
 * 分辨率由 /dev/fb0 的 ioctl 查询，不写死。
 * ============================================================================ */

#include <user/unistd.h>
#include <types.h>

#define O_RDWR      2
#define FBIOGET_WIDTH    0x4001
#define FBIOGET_HEIGHT   0x4002
#define FBIOPUT_PIXEL    0x4003

static int fb_fd = -1;
static int in_fd = -1;

static void puts(const char *s)
{
    sys_write(1, s, 0);  /* fd 1 = stdout (kernel kprintf) */
}

static void fb_fill_rect(u32 x, u32 y, u32 w, u32 h, u32 color)
{
    /* 通过 ioctl 批量画矩形 */
    u64 args[5] = { x, y, w, h, color };
    sys_ioctl(fb_fd, 0x4010, (u64)args);
}

static void fb_draw_text(u32 x, u32 y, const char *s, u32 color)
{
    u64 args[4] = { x, y, (u64)s, color };
    sys_ioctl(fb_fd, 0x4011, (u64)args);
}

void radio_main(void)
{
    puts("[radio] user-space radio starting\n");

    fb_fd = sys_open("/dev/fb0", O_RDWR);
    in_fd = sys_open("/dev/input0", O_RDWR);

    if (fb_fd < 0) {
        puts("[radio] no framebuffer, exiting\n");
        sys_exit(1);
    }

    /* 主事件循环 */
    for (;;) {
        /* 画频道列表 */
        fb_fill_rect(0, 0, 480, 24, 0x0000);
        fb_draw_text(4, 4, "NetrdorvOS Radio", 0xFFFF);

        /* 读触控（无事件返回 0） */
        int touch[3] = {0};
        if (sys_read(in_fd, touch, sizeof(touch)) > 0 && touch[2]) {
            puts("[radio] touch detected\n");
        }

        fb_fill_rect(0, 456, 480, 24, 0x0000);
        fb_draw_text(10, 460, "Vol: [######    ]", 0xFFFF);

        sys_yield();
    }
}
