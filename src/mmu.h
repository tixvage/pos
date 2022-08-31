#ifndef MMU_H_
#define MMU_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t Directory_Entry;
typedef uint32_t Page;

#define X86_PAGE_PRESENT (1<<0)
#define X86_PAGE_WRITE (1<<1)
#define X86_PAGE_USER (1<<2)
#define X86_PAGE_PWT (1<<3)
#define X86_PAGE_PCD (1<<4)
#define X86_PAGE_ACCESSED (1<<5)
#define X86_PAGE_DIRTY (1<<6)
#define X86_PAGE_PAGESIZE (1<<7)
#define X86_PAGE_GLOBAL (1<<8)
#define X86_PAGE_AVAIL1 (1<<9)
#define X86_PAGE_AVAIL2 (1<<10)
#define X86_PAGE_AVAIL3 (1<<11)

#define PAGE_PRESENT 1
#define PAGE_RW      2
#define PAGE_USER    4
#define PAGE_LARGE   128

#define PAGE_FRAME   0xFFFFF000
#define PAGE_FLAGS   0x00000FFF

#define KERNEL_PAGE_OFFSET 0xC0000000
#define KERNEL_END_MAP 0xC0400000
#define KERNEL_HEAP_BEGIN KERNEL_END_MAP
#define KERNEL_HEAP_SIZE 0x1E00000

void init_paging(void);
uintptr_t paging_get_kernel_dir(void);
Page* paging_get_page(uintptr_t virt, bool create, uint32_t flags);
void paging_map_page(uintptr_t virt, uintptr_t phys, uint32_t flags);
void paging_invalidate_page(uintptr_t virt);
uintptr_t phys_to_virt(void* ptr);
uintptr_t virt_to_phys(void* ptr);
void page_table_set(uintptr_t address, uintptr_t page_addr, uint16_t flags);
void paging_map_pages(uintptr_t virt, uintptr_t phys, uint32_t num, uint32_t flags);

#endif
