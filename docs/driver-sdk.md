# NetrdorvOS 驱动开发 SDK

**Nexera Kernel (NK) 驱动模块开发规范**

## 概述

NetrdorvOS 采用模块化宏内核架构。硬件驱动以独立 `.ko` 模块形式存在，
通过 VFS 设备接口与内核交互。驱动开发者只需实现 `file_ops_t` 并调用
`MODULE_DEFINE` 即可。

## 驱动模块结构

每个驱动模块必须：

1. 包含 `#include <kernel/module.h>` 和 `#include <kernel/vfs.h>`
2. 实现 `file_ops_t` 的回调函数（open/read/write/ioctl/close）
3. 使用 `MODULE_DEFINE` 注册模块

## 最小驱动示例

```c
#include <kernel/module.h>
#include <kernel/vfs.h>
#include <driver/uart.h>

static int my_open(const char *path, int flags) { return 0; }
static int my_read(int fd, void *buf, size_t n) { return 0; }
static int my_write(int fd, const void *buf, size_t n) { return (int)n; }
static int my_ioctl(int fd, unsigned long req, void *arg) { return 0; }
static int my_close(int fd) { return 0; }

static file_ops_t my_ops = {
    .open = my_open, .read = my_read, .write = my_write,
    .ioctl = my_ioctl, .close = my_close,
};

static int my_init(void) { return 0; }
static void my_exit(void) {}

MODULE_DEFINE(mydev, my_init, my_exit, &my_ops);
```

编译后生成 `mydev.ko`，放入 `/drivers/` 目录即可被 `boot.toml` 加载。

## 设备节点命名

| 设备 | 节点路径 | 说明 |
|---|---|---|
| 帧缓冲 | `/dev/fb0` | 显示输出 |
| 串口 | `/dev/ttyS0` | 调试日志 |
| 触控 | `/dev/input0` | 输入事件 |
| 音频 | `/dev/audio0` | I2S PCM 输出 |
| WiFi | `/dev/wifi0` | 网络收发 |

## 不允许的行为

- 直接访问内核全局变量
- 直接调用内核函数（经 VFS/syscall 接口）
- 链接标准库
- 使用 Rust/Python/任何脚本语言

## ioctl 命令约定

| 范围 | 用途 |
|---|---|
| 0x4000-0x4FFF | 帧缓冲 |
| 0x4010-0x401F | 音频 |
| 0x4020-0x402F | 网络 |
| 0x4030-0x403F | 触控 |
