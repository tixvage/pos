#include "mmu.h"
#include "pmm.h"
#include "std.h"

#define DIRECTORY_INDEX(x) ((x) >> 22)
#define TABLE_INDEX(x) (((x) >> 12) & 0x3FF)

static Directory_Entry* current_page_dir;
extern Directory_Entry boot_page_directory[1024];

void init_paging(void) {
    //TODO: page fault handler

    uintptr_t dir_phys = virt_to_phys(&boot_page_directory);
    boot_page_directory[1023] = dir_phys | PAGE_PRESENT | PAGE_RW;
    paging_invalidate_page(0xFFC00000);

    // Replace the initial identity mapping, extending it to cover grub modules
    uint32_t to_map = divide_up(pmm_get_kernel_end(), 0x1000);
    boot_page_directory[0] = 0;
    paging_map_pages(0x00000000, 0x00000000, to_map, PAGE_RW);
    paging_invalidate_page(0x00000000);
    
    current_page_dir = boot_page_directory;
}

uintptr_t paging_get_kernel_dir(void) {
    return (uintptr_t)virt_to_phys(&boot_page_directory);
}

Page* paging_get_page(uintptr_t virt, bool create, uint32_t flags) {
    //TODO: unaligned address check

    uint32_t dir_i = DIRECTORY_INDEX(virt);
    uint32_t table_i = TABLE_INDEX(virt);

    Directory_Entry* dir = (Directory_Entry*) 0xFFFFF000;
    Page* table = (Page*) (0xFFC00000 + (dir_i << 12));
    
    if (!(dir[dir_i] & PAGE_PRESENT) && create) {
        Page* new_table = (Page*) pmm_alloc_page();
        dir[dir_i] = (uint32_t) new_table | PAGE_PRESENT | PAGE_RW | (flags & PAGE_FLAGS);
        kmemset(table, 0, 4096);
    }

    if (dir[dir_i] & PAGE_PRESENT) return &table[table_i];

    return 0;
}

void paging_map_page(uintptr_t virt, uintptr_t phys, uint32_t flags) {
    Page* page = paging_get_page(virt, true, flags);

    if (*page & PAGE_PRESENT) {
        //TODO: panic
    }

    *page = phys | PAGE_PRESENT | (flags & PAGE_FLAGS);
    paging_invalidate_page(virt);
}

void paging_invalidate_page(uintptr_t virt) {
    __asm__ volatile ("invlpg (%0)" :: "b"(virt) : "memory");
}

void* load_page_table(void) {
    void *raw_ptr;
    __asm__ volatile("mov %%cr3, %0" : "=a"(raw_ptr));
    return (uint32_t *) ((uintptr_t) raw_ptr + KERNEL_PAGE_OFFSET);
}

uintptr_t phys_to_virt(void* ptr) {
    if ((uintptr_t) ptr > KERNEL_PAGE_OFFSET) return ptr;
    return ((uintptr_t) ptr + KERNEL_PAGE_OFFSET);
}

uintptr_t virt_to_phys(void* ptr) {
    if ((uintptr_t) ptr < KERNEL_PAGE_OFFSET) return ptr;
    return ((uintptr_t) ptr - KERNEL_PAGE_OFFSET);
}

void page_table_set(uintptr_t address, uintptr_t page_addr, uint16_t flags) {
    uint32_t *page_table = load_page_table();
    uint16_t i = (uint16_t) (page_addr >> 22);
    page_table[i] = (address & 0xFFC00000) | flags;
    __asm__ volatile("invlpg %0" : : "m"(i));
}

void paging_map_pages(uintptr_t virt, uintptr_t phys, uint32_t num, uint32_t flags) {
    for (uint32_t i = 0; i < num; i++) {
        paging_map_page(virt, phys, flags);
        phys += 0x1000;
        virt += 0x1000;
    }
}
