# NetrdorvOS

**RISC-V 自制收音机操作系统** —— 宏内核，适配 Milk-V DuoS (SG2002, RV64GCV)，Limine 引导，S-mode 运行。

> 开发依据：
> - [完整开发计划](https://feishu.doubao.com/docx/VzjNd2fnDodtgRx5IZvcz5QfnSc)
> - [开发语言规范](https://feishu.doubao.com/docx/TWzkdTFc7oIrX4xrOgkcsbq6nle)

## 架构（符合 CLAUDE.md 规则）

| 层 | 组件 | 状态 |
|---|---|---|
| **引导** | Limine 协议（EFI 分区，S-mode 进入） | ✅ 协议头+请求结构 |
| **内核** | Sv39 MMU、S-mode trap、CLINT 定时器 | ✅ |
| **内核** | 轮转调度器（16 任务） | ✅ |
| **内核** | 物理页位图 + 堆分配器 | ✅ |
| **VFS** | open/read/write/ioctl/close + /dev 设备节点 | ✅ |
| **系统调用** | ecall 分发，16 个 syscall 号 | ✅ |
| **模块加载器** | .ko 模块注册表（.modules 段） | ✅ 内置模块 |
| **ELF 加载器** | ELF64 用户程序加载 | ✅ 骨架 |
| **驱动** | /dev/fb0 (显示)、/dev/ttyS0 (串口)、/dev/input0 (触控) | ✅ 接口 |
| **驱动** | I2S 音频、SDIO WiFi | ⏳ 开源社区 .ko |
| **用户态** | 收音机应用（经 syscall 访问 /dev 设备） | ✅ 基础版 |
| **GUI** | 基础画布绘图（分辨率可配置） | ✅ |
| **系统服务** | TCP/IP、HLS、MP3/AAC 解码、环形缓冲 | ⏳ 接入 lwIP/minimp3 |

## 目录结构（对齐规则）

```
NetrdorvOS/
├── arch/riscv64/          # RISC-V 架构层
│   ├── boot.S             #   Limine 头 + S-mode 入口
│   ├── limine.h           #   Limine 协议定义
│   ├── mmu.c              #   Sv39 恒等映射
│   ├── trap.c             #   S-mode 异常 + ecall syscall
│   └── timer.c            #   SBI 定时器
├── kernel/
│   ├── init.c             #   kmain 启动流程
│   ├── elf.c              #   ELF64 用户程序加载
│   ├── module.c           #   .ko 模块加载器
│   ├── syscall.c          #   系统调用分发表
│   ├── vfs/vfs.c          #   VFS + 设备节点
│   ├── mm/pmm.c           #   物理页 + 堆
│   └── sched/             #   调度器 + 上下文切换
├── drivers/               # 内核可加载驱动（.ko）
│   ├── serial/uart.c      #   /dev/ttyS0
│   ├── display/display.c  #   /dev/fb0
│   ├── touch/touch.c      #   /dev/input0
│   ├── audio/audio.c      #   /dev/audio0 占位
│   └── wifi/wifi.c        #   /dev/wifi0 占位
├── system/                # 系统服务
│   ├── gui/gui.c          #   画布绘图
│   ├── net/net.c          #   socket 层
│   └── audio/decode.c     #   解码器接口
├── app/radio/radio.c      # 用户态收音机（syscall 访问设备）
├── include/
│   ├── arch/  kernel/  driver/  system/  user/  lib/
├── link.ld                # Limine S-mode 链接脚本
└── Makefile
```

## 设计原则（Unix 哲学）

- **一切设备皆文件**：`open("/dev/fb0")` → `read/write/ioctl` → `close`
- **用户态不直接调内核函数**：全部经 ecall → syscall 表
- **驱动独立成 .ko**：模块注册 file_ops，内核不感知硬件细节
- **分辨率不写死**：`gui_disp.width/height` 运行时配置

## 构建

```powershell
$env:Path = "D:\xpack-riscv-none-elf-gcc-15.2.0-1\bin;" + $env:Path
make clean && make
```

产物：`build/netrdorvos.elf`（9.5KB text）+ `.bin`

## 分辨率配置

```c
/* 改 include/system/gui.h 或运行时调 gui_disp */
gui_display_t gui_disp = { .width=480, .height=800, .bpp=16, .fb=... };
```

## 上板 TODO

- [ ] 制作 FAT32 分区：EFI/BOOT/BOOTRISCV64.EFI + limine.conf + boot/kernel.elf
- [ ] 校准 UART/CLINT/PLIC 物理地址
- [ ] 接入社区 MIPI-DSI / I2S / SDIO 驱动为 .ko
- [ ] 移植 lwIP（system/net/net.c）+ minimp3（system/audio/decode.c）
- [ ] 从 FAT32 加载 /bin/init.elf 进入用户态
