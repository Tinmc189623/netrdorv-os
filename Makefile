# ============================================================================
# NetrdorvOS - RISC-V 自制收音机操作系统 (Milk-V DuoS RV64)
# 构建系统：riscv-none-elf-gcc / riscv64-unknown-elf-gcc 纯裸机工具链
# 语言构成：95% C + 5% RV64 汇编
# ============================================================================

# ---------------- 工具链 ----------------
# 默认使用 riscv-none-elf-（xPack）；Ubuntu 系统包为 riscv64-unknown-elf-
# 用法：make CROSS_COMPILE=riscv64-unknown-elf-
CROSS_COMPILE ?= riscv-none-elf-
CC      := $(CROSS_COMPILE)gcc
AS      := $(CROSS_COMPILE)gcc
LD      := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
SIZE    := $(CROSS_COMPILE)size
NM      := $(CROSS_COMPILE)nm

# ---------------- 跨平台命令 ----------------
ifeq ($(OS),Windows_NT)
  # Windows cmd: 目录已存在时 mkdir 返回非零，用 if not exist 包裹
  MKDIR_P = @if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
  RMDIR   = if exist build rmdir /s /q build
else
  MKDIR_P = @mkdir -p $(dir $@)
  RMDIR   = rm -rf build
endif

# ---------------- 架构与目标 ----------------
ARCH       := riscv64
TARGET     := netrdorvos
KERNEL_ELF := build/$(TARGET).elf
KERNEL_BIN := build/$(TARGET).bin
KERNEL_LST := build/$(TARGET).lst

# ---------------- 编译选项 ----------------
CFLAGS  := -march=rv64gc -mabi=lp64d
CFLAGS  += -std=gnu11
CFLAGS  += -ffreestanding -nostdlib -fno-builtin -fno-stack-protector
CFLAGS  += -mcmodel=medany -fno-pic -fno-pie
CFLAGS  += -Wall -Wextra -O2 -g
CFLAGS  += -Iinclude -Iinclude/user -Iarch/$(ARCH)

# 链接选项
LDFLAGS := -m elf64lriscv -nostdlib -static -no-pie -T link.ld -Map build/$(TARGET).map

# ---------------- 源文件收集 ----------------
SRC_DIRS := arch/$(ARCH) kernel kernel/mm kernel/sched kernel/vfs \
            drivers/serial drivers/display drivers/touch drivers/audio drivers/wifi \
            system/gui system/net system/audio system/json system/xml \
            app/radio bin lib

C_SRCS   := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_SRCS   := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.S))
OBJS     := $(C_SRCS:.c=.o) $(S_SRCS:.S=.o)
OBJS     := $(addprefix build/,$(OBJS))
DEPS     := $(OBJS:.o=.d)

# ---------------- 目标 ----------------
.PHONY: all clean asm size dirs

all: $(KERNEL_BIN)

$(KERNEL_ELF): $(OBJS) link.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $(KERNEL_ELF)

asm: $(KERNEL_ELF)
	$(OBJDUMP) -d $(KERNEL_ELF) > $(KERNEL_LST)
	@echo "反汇编已输出到 $(KERNEL_LST)"

size: $(KERNEL_ELF)
	$(SIZE) $(KERNEL_ELF)

# ---------------- 编译规则 ----------------
build/%.o: %.c
	$(MKDIR_P)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

build/%.o: %.S
	$(MKDIR_P)
	$(AS) $(CFLAGS) -c $< -o $@

clean:
	$(RMDIR)

# 依赖文件
-include $(DEPS)
