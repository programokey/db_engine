#include "../stddef.h"
#include "memory.h"
#include "disk.h"
#define PAGE_NUMBER (MEMORY_POOL_SIZE / PAGE_SIZE)

struct internal_node_page
{
	void* page;
	int n;
	uint32_t location;
	byte used[10];
};
//int a[sizeof(struct internal_node_page)];
uint32_t end_of_file;

void flush(void* p)
{
	fseek(file,0,memory_to_disk(p));
	fwrite(p,PAGE_SIZE,1,file);
}

struct memory_to_disk_item
{
	uint32_t p;
	uint16_t disk;
};

struct disk_to_memory_itme
{
	uint32_t location;
	uint16_t memory;
};

int item_num = 0;
struct memory_to_disk_item memory_to_disk_table[PAGE_NUMBER];

struct disk_to_memory_itme disk_to_memory_table[PAGE_NUMBER];

int dirty_size = 0;
uint32_t dirty[PAGE_NUMBER];

void* swap_in(uint32_t location)
{
	uint32_t offset = location & 0xffff;
	location -= offset;
	if (disk_to_memory(location) != NULL)
		return disk_to_memory(location + offset);
	if (item_num == PAGE_NUMBER) {
		int victim = rand() % PAGE_NUMBER;
		int k = memory_to_disk_table[victim].disk;
		clean();
		disk_to_memory_table[k].location = location;
		disk_to_memory_table[k].memory = (uint16_t)victim;
		for (; k > 0 && disk_to_memory_table[k].location < disk_to_memory_table[k - 1].location; k--) {
			swap(disk_to_memory_table[k - 1].location, disk_to_memory_table[k].location);
			swap(disk_to_memory_table[k - 1].memory, disk_to_memory_table[k].memory);
		}
		for (; k < PAGE_NUMBER - 1 && disk_to_memory_table[k].location > disk_to_memory_table[k + 1].location; k++) {
			swap(disk_to_memory_table[k + 1].location, disk_to_memory_table[k].location);
			swap(disk_to_memory_table[k + 1].memory, disk_to_memory_table[k].memory);
		}
		memory_to_disk_table[victim].disk = (uint16_t)k;
		fseek(file,0,location);
		fread((void*)memory_to_disk_table[victim].p,PAGE_SIZE,1,file);
		return (void*)(memory_to_disk_table[victim].p + offset);
	} else {
		int i = item_num;
		int j = item_num++;
		memory_to_disk_table[i].p = (uint32_t)get_page();
		disk_to_memory_table[j].location = location;
		for (; i > 0 && memory_to_disk_table[i].p < memory_to_disk_table[i - 1].p; i--) {
			swap(memory_to_disk_table[i].p, memory_to_disk_table[i - 1].p);
			swap(memory_to_disk_table[i].disk, memory_to_disk_table[i - 1].disk);
		}
		disk_to_memory_table[j].memory = (uint16_t)i;
		for (; j > 0 && disk_to_memory_table[j].location < disk_to_memory_table[j - 1].location; j--) {
			swap(disk_to_memory_table[j - 1].location, disk_to_memory_table[j].location);
			swap(disk_to_memory_table[j - 1].memory, disk_to_memory_table[j].memory);
		}
		memory_to_disk_table[i].disk = (uint16_t)j;
		fread((void*)memory_to_disk_table[i].p, PAGE_SIZE, 1, file);
		return (void*)(memory_to_disk_table[i].p + offset);
	}
}

int cur_node;
uint32_t get_disk_node()
{
	return 0;
}
uint32_t get_block()
{
	assert(end_of_file % BLOCK_SIZE == 0);
	if(end_of_file + BLOCK_SIZE >= 0x7fffffff)
		return 0xffffffff;
	return (end_of_file += BLOCK_SIZE) - BLOCK_SIZE;
}
void* disk_to_memory(uint32_t disk_pos)
{
	uint32_t offset = 0xffff & disk_pos;
	disk_pos -= offset;
	int left = 0;
	int right = item_num;
	int mid;
	while (left < right)
	{
		mid = left / 2 + right / 2 + (1l & left & right);
		if (disk_to_memory_table[mid].location == disk_pos) {
			return (void*)(offset + memory_to_disk_table[disk_to_memory_table[mid].memory].p);
		} else if (disk_pos > disk_to_memory_table[mid].location) {
			left = mid + 1;
		} else {
			right = mid;
		}
	}
	return NULL;
}
uint32_t memory_to_disk(void* memory_pos)
{
	int left = 0;
	int right = item_num;
	int mid;
	uint32_t offset = 0xffff & (uint32_t)memory_pos;
	memory_pos = (uint32_t)memory_pos -  offset;
	while (left < right)
	{
		mid = left / 2 + right / 2 + (1l & left & right);
		if (memory_to_disk_table[mid].p == memory_pos) {
			return offset + disk_to_memory_table[memory_to_disk_table[mid].disk].location;
		}
		else if (memory_pos > disk_to_memory_table[mid].location) {
			left = mid + 1;
		}
		else {
			right = mid;
		}
	}
	return NULL;
}

void stain(void* p)
{
	dirty[dirty_size++] = (void*)((uint32_t)p & (~0xffff));
}

void clean()
{
	for (int i = 0; i < dirty_size; i++)
		flush(dirty[i]);
	dirty_size = 0;
}