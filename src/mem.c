#include "mem.h"

Memory_Manager init_mm(uint32_t start, uint32_t size) {
    Memory_Manager mm;

    if (size < sizeof(Memory_Chunk)) {
        mm.first = 0;
    } else {
        mm.first = (Memory_Chunk*) start;

        mm.first->allocated = false;
        mm.first->prev = 0;
        mm.first->next = 0;
        mm.first->size = size - sizeof(Memory_Chunk);
    }

    return mm;
}

void* mm_alloc(Memory_Manager* mm, uint32_t size) {
    Memory_Chunk* res = 0;
    for (Memory_Chunk* chunk = mm->first; chunk != 0 && res == 0; chunk = chunk->next) {
        if (chunk->size > size && !chunk->allocated)
            res = chunk;
    }

    if (res == 0) return 0;

    if (res->size >= size + sizeof(Memory_Chunk) + 1) {
        Memory_Chunk* temp = (Memory_Chunk*)((uint32_t)res + sizeof(Memory_Chunk) + size);

        temp->allocated = false;
        temp->size = res->size - size - sizeof(Memory_Chunk);
        temp->prev = res;
        temp->next = res->next;
        if(temp->next != 0)
            temp->next->prev = temp;
        res->size = size;
        res->next = temp;
    }

    res->allocated = true;
    return (void*)(((uint32_t)res) + sizeof(Memory_Chunk));
}

void mm_free(Memory_Manager* mm, void* ptr) {

    Memory_Chunk* chunk = (Memory_Chunk*)((uint32_t)ptr - sizeof(Memory_Chunk));

    chunk->allocated = false;

    if (chunk->prev != 0 && !chunk->prev->allocated) {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(Memory_Chunk);
        if(chunk->next != 0) chunk->next->prev = chunk->prev;
        chunk = chunk->prev;
    }

    if (chunk->next != 0 && !chunk->next->allocated) {
        chunk->size += chunk->next->size + sizeof(Memory_Chunk);
        chunk->next = chunk->next->next;
        if(chunk->next != 0) chunk->next->prev = chunk;
    }
}
