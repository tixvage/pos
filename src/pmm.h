#ifndef PMM_H_
#define PMM_H_

#define PMM_BLOCK_SIZE 4096

#include "multiboot.h"
#include <stdint.h>

uint32_t divide_up(uint32_t n, uint32_t d);
void init_pmm(multiboot* mb);
uint32_t pmm_used_memory(void);
uint32_t pmm_total_memory(void);
void pmm_init_region(uintptr_t addr, uint32_t size);
void pmm_deinit_region(uintptr_t addr, uint32_t size);
uintptr_t pmm_alloc_page(void);
uintptr_t pmm_alloc_aligned_large_page(void);
uintptr_t pmm_alloc_pages(uint32_t num);
void pmm_free_page(uintptr_t addr);
void pmm_free_pages(uintptr_t addr, uint32_t num);
uintptr_t pmm_get_kernel_end(void);

#endif
