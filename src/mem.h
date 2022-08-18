#ifndef MEM_H_
#define MEM_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct Memory_Chunk {
    struct Memory_Chunk* next;
    struct Memory_Chunk* prev;
    bool allocated;
    uint32_t size;
} Memory_Chunk;

typedef struct Memory_Manager {
    Memory_Chunk* first;
} Memory_Manager;

Memory_Manager init_mm(uint32_t start, uint32_t size);

void* mm_alloc(Memory_Manager* mm, uint32_t size);
void mm_free(Memory_Manager* mm, void* ptr);

#endif

