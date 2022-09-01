#include "vmm.h"
#include "pmm.h"
#include "mmu.h"

static Mem_Block* bottom = NULL;
static Mem_Block* top = NULL;
static uint32_t used_memory = 0;

uint32_t vmm_used_memory(void) {
    return used_memory;
}

uint32_t align_to(uint32_t n, uint32_t align) {
    if (n % align == 0) return n;
    return n + (align - n % align);
}

void* sbrk(uint32_t size) {
    uintptr_t addr;
    __asm__ volatile (
        "mov $4, %%eax\n"
        "mov %[size], %%ebx\n"
        "int $0x30\n"
        "mov %%eax, %[addr]\n"
        : [addr] "=r" (addr)
        : [size] "r" (size)
        : "%eax", "%ebx"
    );
    return (void*) addr;
}

uint32_t mem_block_size(Mem_Block* block) {
    return sizeof(Mem_Block) + (block->size & ~1);
}

Mem_Block* mem_get_block(void* pointer) {
    uintptr_t addr = (uintptr_t) pointer;
    return (Mem_Block*) (addr - sizeof(Mem_Block) + 4);
}

Mem_Block* mem_new_block(uint32_t size, uint32_t align) {
    const uint32_t header_size = offsetof(Mem_Block, data);

    uintptr_t next = (uintptr_t) top + mem_block_size(top);
    uintptr_t next_aligned = align_to(next+header_size, align) - header_size;

    Mem_Block* block = (Mem_Block*) next_aligned;
    block->size = size | 1;
    block->next = NULL;

    // Insert a free block between top and our aligned block, if there's enough
    // space. That block is 8-bytes aligned.
    next = align_to(next+header_size, MIN_ALIGN) - header_size;
    if (next_aligned - next > sizeof(Mem_Block) + MIN_ALIGN) {
        Mem_Block* filler = (Mem_Block*) next;
        filler->size = next_aligned - next - sizeof(Mem_Block);
        top->next = filler;
        top = filler;
    }

    top->next = block;
    top = block;

    return block;
}

bool mem_is_aligned(Mem_Block* block, uint32_t align) {
    uintptr_t addr = (uintptr_t) block->data;
    return addr % align == 0;
}

Mem_Block* mem_find_block(uint32_t size, uint32_t align) {
    if (!bottom) return NULL;

    Mem_Block* block = bottom;

    while ((block->size & ~1) < size || block->size & 1 || !mem_is_aligned(block, align)) {
        block = block->next;
        if (!block) return NULL;
    }

    return block;
}

void* aligned_alloc(size_t align, size_t size) {
    const uint32_t header_size = offsetof(Mem_Block, data);
    size = align_to(size, 8);

    if (!top) {
        uintptr_t addr = KERNEL_HEAP_BEGIN;
        uintptr_t heap_phys = pmm_alloc_pages(KERNEL_HEAP_SIZE/0x1000);
        paging_map_pages(addr, heap_phys, KERNEL_HEAP_SIZE/0x1000, PAGE_RW);
        bottom = (Mem_Block*) addr;
        top = bottom;
        top->size = 1;
        top->next = NULL;
    }
    
    Mem_Block* block = mem_find_block(size, align);

    if (block) {
        used_memory += block->size;
        block->size |= 1;
        return block->data;
    } else {
        uintptr_t end = (uintptr_t) top + mem_block_size(top) + header_size;
        end = align_to(end, align) + size;
        //TODO: OOM handling
        block = mem_new_block(size, align);
    }

    used_memory += size;
    return block->data;
}
