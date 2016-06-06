#include <memory.h>
#include "memory.h"
#include "../stddef.h"

#define PAGE_NUMBER (MEMORY_POOL_SIZE / PAGE_SIZE)

void* begin_of_memory_pool;


byte bit_map[PAGE_NUMBER/8];
int last_pos = 0;

void* memroy_allocate(int size)
{
	return malloc(size);
}

void  release_memory(void* buffer)
{
	free(buffer);
}

void* get_page()
{
	for (;last_pos < PAGE_NUMBER/8; last_pos++) {
		if (bit_map[last_pos] != 0xff) {
			for (int i = 0; i < 8; i++) {
				if (get_bit(bit_map[last_pos], i) == false) {
					return (byte*)memory_pool + PAGE_SIZE * (last_pos * 8 + i);
				}
			}
		}
	}
	return NULL;
}

void* get_memory_pool()
{
	begin_of_memory_pool =  malloc(MEMORY_POOL_SIZE + PAGE_SIZE);
	/*align to 64KB*/
	memory_pool = ((uint32_t)begin_of_memory_pool) % PAGE_SIZE == 0 ? begin_of_memory_pool :\
		(void*)(PAGE_SIZE - ((uint32_t)begin_of_memory_pool) % PAGE_SIZE + ((uint32_t)begin_of_memory_pool));
	return memory_pool;
}

void release_memory_pool()
{
	free(begin_of_memory_pool);
}

void memory_copy(byte* dest, byte* source, int length)
{
	word* d = (word*)dest;
	word* s = (word*)source;
	int word_num = length / sizeof(word);
	for (int i = 0; i < word_num; i++)
		d[i] = s[i];
	for (int i = 0; i < length%sizeof(word); i++)
		dest[word_num*sizeof(word) + i] = source[word_num*sizeof(word) + i];
}
