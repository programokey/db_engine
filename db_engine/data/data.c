#include "data.h"
#include "../storage/disk.h"
#include "../storage/memory.h"
struct data fix_length_data(int length, char* (*output_func)(byte*, int),void(*input_func)(char*, byte*, int*))
{
	struct data type;
	type.length = length;
	type.is_variable = false;
	type.input_func = input_func;
	type.output_func = output_func;
	return type;
}
struct data variable_length_data(int length, char* (*output_func)(byte*, int), void(*input_func)(char*, byte*, int*))
{
	struct data type;
	type.length = length;
	type.is_variable = true;
	type.input_func = input_func;
	type.output_func = output_func;
	return type;
}
struct data_node
{
	uint32_t level;
	uint32_t length;
};
void store_variable_data(byte* destination, byte* source)
{
	uint32_t length = *(uint32_t*)source;
	int node_needed = length / BYTE_PER_NODE;
	uint32_t* nodes = (uint32_t*)malloc(node_needed);
	byte* p = NULL;
	struct data_node* dn;
	int level = 0;
	for (int i = 0; i < node_needed; i++) {
		nodes[i] = get_data_node();
		p = disk_to_memory(nodes[i]);
		dn = (struct data_node*)p;
		dn->length = BYTE_PER_NODE;
		dn->level = level;
		memory_copy(p + sizeof(struct data_node), source + i*BYTE_PER_NODE, BYTE_PER_NODE);
	}
	if (length % BYTE_PER_NODE != 0) {
		nodes[node_needed] = get_data_node();
		p = disk_to_memory(nodes[node_needed]);
		dn = (struct data_node*)p;
		dn->length = length%BYTE_PER_NODE;
		dn->level = level;
		memory_copy(p + sizeof(struct data_node), source + node_needed++*BYTE_PER_NODE, BYTE_PER_NODE);
	}

	while (node_needed > 1){
		level++;
		length = node_needed;
		node_needed = length / NODE_PER_NODE;
		for (int i = 0; i < node_needed; i++) {
			nodes[i] = get_data_node();
			p = disk_to_memory(nodes[i]);
			dn = (struct data_node*)p;
			dn->length = NODE_PER_NODE;
			dn->level = level;
			memory_copy(p + sizeof(struct data_node), nodes + i*NODE_PER_NODE, BYTE_PER_NODE);
		}
		if (length % NODE_PER_NODE != 0) {
			nodes[node_needed] = get_data_node();
			p = disk_to_memory(nodes[node_needed]);
			dn = (struct data_node*)p;
			dn->length = length%NODE_PER_NODE;
			dn->level = level;
			memory_copy(p + sizeof(struct data_node), nodes + node_needed++*NODE_PER_NODE, BYTE_PER_NODE);
		}
	}
	*(uint32_t*)destination = *nodes;
	free(nodes);
}