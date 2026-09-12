/* ============================================================================
 * NetrdorvOS - ELF64 加载器实现
 * 验证 ELF 头，遍历 program headers，PT_LOAD 段复制到用户地址
 * ============================================================================ */

#include <kernel/elf.h>
#include <kernel/mm.h>
#include <lib/string.h>

/* ELF64 头 */
typedef struct {
    u8  ident[16];
    u16 type;
    u16 machine;
    u32 version;
    u64 entry;
    u64 phoff;
    u64 shoff;
    u32 flags;
    u16 ehsize;
    u16 phentsize;
    u16 phnum;
    u16 shentsize;
    u16 shnum;
    u16 shstrndx;
} elf64_ehdr_t;

typedef struct {
    u32 type;
    u32 flags;
    u64 offset;
    u64 vaddr;
    u64 paddr;
    u64 filesz;
    u64 memsz;
    u64 align;
} elf64_phdr_t;

#define PT_LOAD     1
#define EM_RISCV    243
#define ELF_MAGIC   0x464c457f  /* \x7fELF */

u64 elf_load_user(const void *image, size_t size, u64 *entry_out,
                  u64 *user_sp_out)
{
    if (size < sizeof(elf64_ehdr_t))
        return 0;

    const elf64_ehdr_t *ehdr = (const elf64_ehdr_t *)image;

    /* 验证 ELF 魔数 */
    if (*(const u32 *)image != ELF_MAGIC)
        return 0;
    if (ehdr->machine != EM_RISCV)
        return 0;
    if (ehdr->phnum == 0 || ehdr->phnum > 16)
        return 0;

    /* 遍历 program headers，加载 PT_LOAD 段 */
    for (u16 i = 0; i < ehdr->phnum; i++) {
        const elf64_phdr_t *ph =
            (const elf64_phdr_t *)((const u8 *)image + ehdr->phoff +
                                    i * ehdr->phentsize);
        if (ph->type != PT_LOAD)
            continue;
        if (ph->offset + ph->filesz > size)
            return 0;

        /* 恒等映射阶段：直接复制到 vaddr */
        memcpy((void *)ph->vaddr,
               (const u8 *)image + ph->offset, ph->filesz);
        /* 清零 BSS（memsz > filesz 部分） */
        if (ph->memsz > ph->filesz)
            memset((void *)(ph->vaddr + ph->filesz), 0,
                   ph->memsz - ph->filesz);
    }

    *entry_out = ehdr->entry;
    /* 用户栈：从 0x80000000 附近向下 64KB */
    *user_sp_out = 0x80010000UL;
    return ehdr->entry;
}
