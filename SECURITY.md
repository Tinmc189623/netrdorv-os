# 安全政策

## 报告漏洞

发现安全漏洞请邮件至 admin@nexsteaduser.com，不要公开 issue。

## 支持版本

| 版本 | 支持状态 |
|---|---|
| 0.1.x | 开发中，未发布 |

## 内核安全

- 所有用户态访问经 syscall 表，不直接碰内核内存
- 驱动模块不可读内核全局变量
- 用户态程序运行在独立地址空间
- 页表隔离 U/S 特权级

Copyright © 2026 Nexsteaduser. All Rights Reserved
