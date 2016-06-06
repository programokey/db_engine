#include"../shcema/record.h"
#include "../storage/memory.h"
#include "../shcema/column.h"
void get_reocrd(byte* record, int* length, struct table* t, byte** data)
{
	byte* null_map = record;
	int map_size = t->size / 8 + (t->size % 8 == 0) ? 0 : 1;
	byte* cur = record + map_size;
	for (int i = 0; i < t->size; i++) {
		struct column* c = &(t->columns[i]);
		struct data* type = &(c->type);
		assert((!c->not_null)||(data[i] != NULL));
		if (data[i] != NULL) {
			if (type->is_variable) {
				store_variable_data(cur,data[i]);
			}
			else {
				for (int j = 0; j < type->length; j++)
					cur[i] = data[i][j];
			}
		}
		if (type->is_variable)
			cur += sizeof(uint32_t);
		else
			cur += type->length;
		if (data[i] == NULL) 
			set_bit(&null_map[i / 8], i % 8, false);
		else
			set_bit(&null_map[i / 8], i % 8, true);
	}
}