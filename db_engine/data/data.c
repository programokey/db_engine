#include "data.h"
#include "../storage/disk.h"
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
void store_variable_data(byte* destination, byte* source)
{
	uint32_t length = *(uint32_t*)source;
	int node_needed = length / BYTE_PER_NODE;
	uint32_t* nodes = (uint32_t*)malloc(node_needed);
	for (int i = 0; i < node_needed; i++) {

	}
}