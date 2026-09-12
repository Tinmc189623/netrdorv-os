/* ============================================================================
 * NetrdorvOS - 链接脚本 (RV64 S-mode, Limine)
 *
 * Limine 加载内核到物理地址 0x80200000，虚拟地址恒等映射。
 * ============================================================================ */

OUTPUT_ARCH(riscv)
ENTRY(_start)

KERNEL_BASE = 0x80200000;

PHDRS
{
    text PT_LOAD;
    data PT_LOAD;
}

SECTIONS
{
    . = KERNEL_BASE;

    /* Limine 请求头（必须最先被找到） */
    .limine_requests : {
        KEEP(*(.limine_requests))
    } :text

    . = ALIGN(4K);
    .text : {
        *(.text.entry)
        *(.text*)
    } :text

    . = ALIGN(4K);
    .rodata : {
        *(.rodata*)
    } :text

    /* 内置模块指针表 */
    . = ALIGN(8);
    __modules_start = .;
    .modules : {
        *(.modules)
    }
    __modules_end = .;

    . = ALIGN(4K);
    .data : {
        *(.data*)
        *(.sdata*)
    } :data

    . = ALIGN(4K);
    __bss_start = .;
    .bss : {
        *(.bss*)
        *(.sbss*)
        *(COMMON)
    } :data
    __bss_end = .;

    . = ALIGN(16);
    __stack_bottom = .;
    . += 64K;
    __stack_top = .;

    . = ALIGN(4K);
    __kernel_end = .;

    /DISCARD/ : {
        *(.comment)
        *(.note*)
    }
}
