#ifndef STORAGE_DISK_H
#define STORAGE_DISK_H
#include "../stddef.h"
#define NODE_SIZE 256
#define BYTE_PER_NODE (NODE_SIZE - 2*sizeof(uint16_t))
#define NODE_PER_NODE (NODE_SIZE/sizeof(uint64_t) - 1)
/*return a memory space corresponding to a disk spcace 
and set the disk space dirty*/
uint32_t get_disk_node();
uint32_t get_block();
/*set the page contains the pointer p to be dirty*/
void stain(void* p);
void* disk_to_memory(uint32_t disk_pos);
uint32_t memory_to_disk(void* memory_pos);
/*swap a block in disk into a page in memory*/
void* swap_in(uint32_t location);
void clean();
/*flush drity block in memory to disk*/
void flush(void*);
#endif // !STORAGE_DISK_H