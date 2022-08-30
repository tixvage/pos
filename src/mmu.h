#ifndef MMU_H_
#define MMU_H_

#include <stdint.h>

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

void* phys_to_virt(void* ptr);

void* virt_to_phys(void* ptr);

void page_table_set(uintptr_t address, uintptr_t page_addr, uint16_t flags);

#endif
