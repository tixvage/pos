#ifndef VMM_H_
#define VMM_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MIN_ALIGN 4

typedef struct Mem_Block {
    struct Mem_Block* next;
    uint32_t size;
    uint8_t data[1];
} Mem_Block;

uint32_t align_to(uint32_t n, uint32_t align);
void* sbrk(uint32_t size);
uint32_t mem_block_size(Mem_Block* block);
Mem_Block* mem_get_block(void* pointer);
Mem_Block* mem_new_block(uint32_t size, uint32_t align);
bool mem_is_aligned(Mem_Block* block, uint32_t align);
Mem_Block* mem_find_block(uint32_t size, uint32_t align);
void* aligned_alloc(size_t align, size_t size);

#endif
