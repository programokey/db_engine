#ifndef STROAGE_MEMORY_H
#define STROAGE_MEMORY_H
#define MEMORY_POOL_SIZE 0x20000000//512MB
#include "../stddef.h"
void* memory_pool;
void* memroy_allocate(int size);
void  release_memory(void* buffer);
void* get_page();
void* get_memory_pool();
void release_memory_pool();
void memory_copy(byte* dest, byte* source, int length);
#endif // !STROAGE_MEMORY_H
