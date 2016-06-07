#ifndef STORAGE_DISK_H
#define STORAGE_DISK_H
#include "../stddef.h"
#define NODE_SIZE 256
#define BYTE_PER_NODE (NODE_SIZE - 2 * sizeof(uint32_t))
#define NODE_PER_NODE (NODE_SIZE/sizeof(uint32_t) - 2)
#define MAP_SIZE      (PAGE_SIZE / NODE_SIZE / 8)
#define INTERNAL_NODE 1
#define LEAF_NODE 2
#define DATA_NODE 3
#define SCHEMA_INFO 4


/*information of a block*/
struct block_info
{
	/*1 for internal node,2 for leaf node,3 for data, 4 for schema info*/
	uint32_t type;
	uint16_t areas;
	byte bitmap[MAP_SIZE];
};

uint32_t get_data_node();
uint32_t get_internal_node(struct table*t);
/*return a memory space corresponding to a disk spcace
and set the disk space dirty*/
uint32_t get_block();

void* disk_to_memory(uint32_t disk_pos);
uint32_t memory_to_disk(void* memory_pos);

/*swap a block in disk into a page in memory*/
void* swap_in(uint32_t location);

/*set the page contains the pointer p to be dirty*/
void stain(void* p);
void clean();
#endif // !STORAGE_DISK_H