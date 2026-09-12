/* ============================================================================
 * NetrdorvOS - I2C 触控驱动 (/dev/input0)
 * 硬件实现由开源社区提供，此处注册 VFS 设备接口
 * ============================================================================ */

#include <driver/touch.h>
#include <kernel/vfs.h>
#include <kernel/module.h>

int touch_init(void) { return 0; }
int touch_read(touch_point_t *pt) { (void)pt; return -1; }
gesture_t touch_get_gesture(void) { return GESTURE_NONE; }

/* ---- VFS: /dev/input0 ---- */
static int in_open(const char *p, int f) { (void)p; (void)f; return 0; }
static int in_read(int fd, void *buf, size_t n) {
    (void)fd; (void)buf; (void)n;
    return 0;  /* 无事件 */
}
static int in_write(int fd, const void *b, size_t n) { (void)fd;(void)b; return (int)n; }
static int in_ioctl(int fd, unsigned long r, void *a) { (void)fd;(void)r;(void)a; return 0; }
static int in_close(int fd) { (void)fd; return 0; }

static file_ops_t in_ops = {
    .open = in_open, .read = in_read, .write = in_write,
    .ioctl = in_ioctl, .close = in_close,
};

static int touch_module_init(void) { return touch_init(); }
static void touch_module_exit(void) {}

MODULE_DEFINE(input0, touch_module_init, touch_module_exit, &in_ops);
