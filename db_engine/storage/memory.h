#ifndef STROAGE_MEMORY_H
#define STROAGE_MEMORY_H
#define MEMORY_POOL_SIZE 0x20000000//512MB
#include "../stddef.h"


void* memory_pool;

void* get_page();
void* get_memory_pool();
void release_memory_pool();

void memory_copy(byte* dest, byte* source, int length);

/*  0 if buffer1 == buffer2
  > 0 if buffer1 > buffer2
  < 0 if buffer1 < buffer2*/
int memory_compare(byte* buffer1, byte* buffer2, int length);
#endif // !STROAGE_MEMORY_H
