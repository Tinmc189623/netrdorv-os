# 路线图

## v0.1 — 骨架（当前）
- [x] Limine S-mode 引导
- [x] 调度器 / 内存 / 中断 / 定时器
- [x] VFS + 设备抽象
- [x] 系统调用表
- [x] 音频管道模块
- [x] JSON/XML 解析

## v0.2 — 用户态跑通
- [ ] fork/exec 完整实现
- [ ] 用户态独立地址空间（用户页表）
- [ ] /bin/init.elf 从 FAT32 加载执行
- [ ] logcat 从内核环形日志缓冲读取
- [ ] reboot 写 PMU 复位寄存器

## v0.3 — 驱动接入
- [ ] MIPI-DSI 显示驱动（社区 .ko）
- [ ] I2C 触控驱动
- [ ] I2S + ES8311 音频驱动
- [ ] SDIO WiFi 驱动
- [ ] FAT32 文件系统

## v0.4 — 网络与音频
- [ ] lwIP 移植（TCP/IP + Socket）
- [ ] minimp3 移植（MP3 解码）
- [ ] HLS 播放（m3u8 解析 + 切片下载）
- [ ] 环形缓冲水位控制

## v1.0 — 生产可用
- [ ] 多 SMP 核跑通
- [ ] POSIX 信号完整
- [ ] 权限用户体系
- [ ] 完整 shell（sh）
- [ ] 在线 OTA 更新

Copyright © 2026 Nexsteaduser. All Rights Reserved
