#include "pmm.h"
#include "std.h"

#define NTHBIT(n) ((uint32_t) 1 << n)

static uint32_t bitmap[1024 * 1024 / 32];
static uint32_t mem_size;
static uint32_t used_blocks;
static uint32_t max_blocks;
static uintptr_t kernel_end;

extern uint32_t KERNEL_END;
extern uint32_t KERNEL_END_PHYS;


void mmap_set(uint32_t bit);
void mmap_unset(uint32_t bit);
uint32_t mmap_test(uint32_t bit);
uint32_t mmap_find_free();
uint32_t mmap_find_free_frame(uint32_t num);

void init_pmm(multiboot* mb) {
    mod_t* modules = (mod_t*) mb->mods_addr;
    
    for (uint32_t i = 0; i < mb->mods_count; i++) {
        mod_t mod = modules[i];
        kernel_end = mod.mod_end;
    }

    if (kernel_end < (uintptr_t) &KERNEL_END_PHYS) {
        kernel_end = (uintptr_t) &KERNEL_END_PHYS;
    }

    mem_size = mb->mem_lower + mb->mem_upper;
    max_blocks = (mem_size * 1024) / PMM_BLOCK_SIZE;
    used_blocks = max_blocks;
    kmemset(bitmap, 0xFF, max_blocks/8);

    uint64_t available = 0;
    uint64_t unavailable = 0;

    //TODO: hardcoded 0xC0000000
    mmap_t* mmap = (mmap_t*) ((mb->mmap_addr) + 0xC0000000);
    while ((uintptr_t) mmap < (((mb->mmap_addr) + 0xC0000000) + mb->mmap_length)) {
        if (!mmap->length) continue;

        if (mmap->type == 1) {
            pmm_init_region((uintptr_t) mmap->addr, mmap->length);
            available += mmap->length;
        } else {
            unavailable += mmap->length;
        }

        mmap = (mmap_t*) ((uintptr_t) mmap + mmap->size + sizeof(uintptr_t));
    }

    mem_size = available;
    pmm_deinit_region(0, kernel_end + max_blocks/8);
}

uint32_t pmm_used_memory(void) {
    return used_blocks * PMM_BLOCK_SIZE;
}

uint32_t pmm_total_memory(void) {
    return mem_size;
}

void pmm_init_region(uintptr_t addr, uint32_t size) {
    uint32_t base_block = addr/PMM_BLOCK_SIZE;

    uint32_t num = divide_up(size, PMM_BLOCK_SIZE);

    while (num-- > 0) mmap_unset(base_block++);

    mmap_set(0);
}

void pmm_deinit_region(uintptr_t addr, uint32_t size) {
    uint32_t base_block = addr/PMM_BLOCK_SIZE;
    uint32_t num = divide_up(size, PMM_BLOCK_SIZE);

    while (num-- > 0) mmap_set(base_block++);
}

uintptr_t pmm_alloc_page() {
    //TODO: epic OOM handling
    uint32_t block = mmap_find_free();

    if (!block) return 0;

    mmap_set(block);

    return (uintptr_t)(block*PMM_BLOCK_SIZE);
}

uintptr_t pmm_alloc_aligned_large_page() {
    if (max_blocks - used_blocks < 2*1024) return 0;

    uint32_t free_block = mmap_find_free_frame(2*1024);

    if (!free_block) return 0;

    uint32_t aligned_block = ((free_block / 1024) + 1) * 1024;

    for (int i = 0; i < 1024; i++) mmap_set(aligned_block + i);

    return (uintptr_t)(aligned_block*PMM_BLOCK_SIZE);
}

uintptr_t pmm_alloc_pages(uint32_t num) {
    if (max_blocks-used_blocks < num) return 0;

    uint32_t first_block = mmap_find_free_frame(num);

    if (!first_block) return 0;

    for (uint32_t i = 0; i < num; i++) mmap_set(first_block+i);

    return (uintptr_t)(first_block*PMM_BLOCK_SIZE);
}

void pmm_free_page(uintptr_t addr) {
    uint32_t block = addr/PMM_BLOCK_SIZE;
    mmap_unset(block);
}

void pmm_free_pages(uintptr_t addr, uint32_t num) {
    uint32_t first_block = addr/PMM_BLOCK_SIZE;

    for (uint32_t i = 0; i < num; i++) mmap_unset(first_block+i);
}

uintptr_t pmm_get_kernel_end() {
    return (uintptr_t) kernel_end + max_blocks / 8;
}

uint32_t divide_up(uint32_t n, uint32_t d) {
    if (n % d == 0) {
        return n / d;
    }

    return 1 + n / d;
}

void mmap_set(uint32_t bit) {
    bitmap[bit / 32] |= NTHBIT(bit % 32);
    used_blocks++;
}

void mmap_unset(uint32_t bit) {
    bitmap[bit / 32] &= ~NTHBIT(bit % 32);
    used_blocks--;
}

uint32_t mmap_test(uint32_t bit) {
    return bitmap[bit / 32] & NTHBIT(bit % 32);
}

uint32_t mmap_find_free() {
    for (uint32_t i = 0; i < max_blocks/32; i++) {
        if (bitmap[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                if (!(bitmap[i] & NTHBIT(j))) {
                    return i * 32 + j;
                }
            }
        }
    }

    return 0;
}

uint32_t mmap_find_free_frame(uint32_t frame_size) {
    uint32_t first = 0;
    uint32_t count = 0;

    for (uint32_t i = 0; i < max_blocks/32; i++) {
        if (bitmap[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                if (!(bitmap[i] & NTHBIT(j))) {
                    if (!first) {
                        first = i*32+j;
                    }

                    count++;
                } else {
                    first = 0;
                    count = 0;
                }

                if (count == frame_size) {
                    return first;
                }
            }
        } else {
            first = 0;
            count = 0;
        }
    }

    return 0;
}
