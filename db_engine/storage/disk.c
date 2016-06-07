#include "../stddef.h"
#include "memory.h"
#include "disk.h"
#include "../shcema/schema.h"

#define PAGE_NUMBER (MEMORY_POOL_SIZE / PAGE_SIZE)

uint32_t file_length;

struct memory_to_disk_item
{
	bool dirty;
	/* index of coresponding disk_to_memory_item;*/
	uint32_t disk;
};
struct disk_to_memory_itme
{
	uint32_t location;
	/*memory value corespond this location.*/
	uint32_t memory;
};

int item_num = 0;
struct memory_to_disk_item memory_to_disk_table[PAGE_NUMBER];
struct disk_to_memory_itme disk_to_memory_table[PAGE_NUMBER];

int dirty_size = 0;
word dirty[PAGE_NUMBER];

void* swap_in(uint32_t location)
{
	uint32_t offset = location & 0xffff;
	location -= offset;
	if (disk_to_memory(location) != NULL)
		return disk_to_memory(location + offset);
	if (item_num == PAGE_NUMBER) {
		clean();

		int k = rand() % PAGE_NUMBER;
		int victim = (disk_to_memory_table[k].memory - (word)memory_pool) >> 16;


		disk_to_memory_table[k].location = location;
		word p = (victim << 16) + (byte*)memory_pool;
		disk_to_memory_table[k].memory = p;

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
		fread((void*)p,PAGE_SIZE,1,file);
		return (byte*)p + offset;
	} else {
		int j = item_num++;
		void* p = get_page();
		int i = (word)p >> 16;
		
		disk_to_memory_table[j].location = location;
		disk_to_memory_table[j].memory = (word)p;

		for (; j > 0 && disk_to_memory_table[j].location < disk_to_memory_table[j - 1].location; j--) {
			swap(disk_to_memory_table[j - 1].location, disk_to_memory_table[j].location);
			swap(disk_to_memory_table[j - 1].memory, disk_to_memory_table[j].memory);
		}

		memory_to_disk_table[i].dirty = false;
		memory_to_disk_table[i].disk = j;
		fread(p, PAGE_SIZE, 1, file);
		return (byte*)p + offset;
	}
}


uint32_t data_page;
void* leaf_node[MAX_TABLES];
uint32_t get_block()
{
	assert( BLOCK_SIZE == 0);
	if(file_length + BLOCK_SIZE >= 0x7fffffff)
		return 0xffffffff;
	fseek(file,0,file_length);
	char buffer = '\0';
	fwrite(&buffer,sizeof(byte), sizeof(PAGE_SIZE),file);
	swap_in(file_length - BLOCK_SIZE);
	stain(disk_to_memory(file_length - BLOCK_SIZE));
	return file_length - BLOCK_SIZE;
}
uint32_t get_data_node() 
{
	struct block_info* bi = disk_to_memory(data_page);
	for (int i = 0; i < MAP_SIZE; i++) {
		if (bi->bitmap[i] != 0xff) {
			for (int j = 0; i < 8; j++) {
				if (!get_bit(bi->bitmap[i], j)) {
					set_bit(bi->bitmap[i], j, true);
					return (i * 8 + j)*NODE_SIZE + data_page;
				}
			}
		}
	}
	data_page = get_block();
	bi = disk_to_memory(data_page);
	for (int i = 0; i < MAP_SIZE; i++)
		bi->bitmap[i] = '\0';
	set_bit(bi->bitmap[0],1,true);
	set_bit(bi->bitmap[0], 2, true);
	bi->areas = MAP_SIZE;
	bi->type = DATA_NODE;
	return NODE_SIZE + data_page;
}
uint32_t get_internal_node(struct table*t)
{
	int index = -1;
	for (int i = 0; i < MAX_TABLES; i++) {
		if (t->name == s.tables[i].name)
			index = i;
	}
	assert(index != -1);
	struct block_info* bi = disk_to_memory(leaf_node[index]);
	int internal_node_size = get_internal_node_size(t);
	for (int i = 0; i < bi->areas; i++) {
		if (bi->bitmap[i] != 0xff) {
			for (int j = 0; i < 8; j++) {
				if (!get_bit(bi->bitmap[i], j)) {
					set_bit(bi->bitmap[i], j, true);
					return (i * 8 + j)*internal_node_size + data_page;
				}
			}
		}
	}
	leaf_node[index] = get_block();
	bi = disk_to_memory(leaf_node[index]);
	bi->areas = PAGE_SIZE / internal_node_size;
	for (int i = 0; i < bi->areas; i++)
		bi->bitmap[i] = '\0';
	bi->type = INTERNAL_NODE;
	set_bit(bi->bitmap[0], 1, true);
	set_bit(bi->bitmap[0], 2, true);
	return internal_node_size + data_page;
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
			return (void*)(offset + disk_to_memory_table[mid].memory);
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
	if (memory_pos >= memory_pool && memory_pos < (byte*)memory_pool + PAGE_NUMBER * PAGE_SIZE) {
		uint32_t offset = 0xffff & (uint32_t)memory_pos;
		if(memory_to_disk_table[(word)memory_pos >> 16].disk != -1)
			return disk_to_memory_table[memory_to_disk_table[(word)memory_pos >> 16].disk].location + offset;
	}
	return -1;
}


void stain(void* p)
{
	if (!memory_to_disk_table[(word)p >> 16].dirty) {
		memory_to_disk_table[(word)p >> 16].dirty = true;
		dirty[dirty_size++] = (word)p >> 16;
	}	
}
void clean()
{
	for (int i = 0; i < dirty_size; i++) {
		fseek(file, 0, disk_to_memory_table[memory_to_disk_table[dirty[i]].disk].location);
		fwrite((dirty[i] << 16) + (byte*)memory_pool, PAGE_SIZE, 1, file);
		memory_to_disk_table[dirty[i]].dirty = false;
	}
	dirty_size = 0;
}