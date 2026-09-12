# 更新日志

本项目遵循[语义化版本](https://semver.org/lang/zh-CN/)。

## [0.1.0] - 2026-09-12

首个可编译骨架。

### 内核
- Limine 引导协议，S-mode 入口
- Sv39 恒等映射（2MB 大页）
- S-mode 中断分发 + ecall 系统调用（16 号）
- CLINT 定时器（SBI 调用，100Hz）
- 物理页位图分配器 + first-fit 堆
- 轮转调度器（16 任务槽，睡眠/唤醒）
- SMP 多核骨架（per-cpu 数据 + IPI）

### 系统服务
- VFS 设备抽象（open/read/write/ioctl/close）
- RAMFS 内存文件系统
- 模块加载器（.ko，.modules 段注册）
- ELF64 用户程序加载器
- POSIX 信号 + 互斥锁 + 信号量
- Socket API（socket/connect/send/recv）

### 音频管道
- 环形缓冲区
- 采样率转换（线性插值）
- 声道重映射（单声道↔立体声）
- PCM 格式转换（16bit↔24bit）
- 事件订阅/分发
- 状态上报

### 网络/解析
- 极简 JSON 解析器
- 极简 XML 解析器
- HTTP 流读取器
- 音频解码器接口（MP3/AAC 占位）

### 驱动
- UART 16550（/dev/ttyS0）
- 帧缓冲（/dev/fb0）
- 触控（/dev/input0）
- 音频/WiFi 占位

### 用户态
- 收音机应用（syscall 访问设备）
- init/logcat/reboot 系统工具骨架
- 极简用户 C 库（ecall 封装）

Copyright © 2026 Nexsteaduser. All Rights Reserved
