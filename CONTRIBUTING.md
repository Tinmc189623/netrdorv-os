# 贡献指南

## 语言规范

- 95% C11 + 5% RISC-V 汇编
- 禁止 Rust / Python / Java / 任何脚本语言
- 无标准库依赖，全部自研

## 构建

```powershell
$env:Path = "D:\xpack-riscv-none-elf-gcc-15.2.0-1\bin;" + $env:Path
make clean; make
```

## 提交规范

- 分支：`feature/xxx`、`fix/xxx`
- commit 信息用英文，祈使句
- 每次提交必须 `make` 通过，零警告

## 驱动开发

驱动开发者请看 [docs/driver-sdk.md](docs/driver-sdk.md)。核心规则：

1. 实现 `file_ops_t`（open/read/write/ioctl/close）
2. 用 `MODULE_DEFINE` 注册
3. 不直接访问内核全局变量
4. 不链接标准库

## 代码风格

- 4 空格缩进
- 函数级注释必须写
- 不允许占位符，所有内容真实可落地

## 报告问题

邮箱：admin@nexsteaduser.com

Copyright © 2026 Nexlyh. All Rights Reserved
