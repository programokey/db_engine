#include "../stddef.h"
#include "../shcema/table.h"
#include "../storage/disk.h"
#include "../storage/memory.h"


/*return balance factor fit the given table*/
int get_balance_factor(struct table* t)
{
	int key_size = 0;
	for (int i = 0; i < t->pk.size; i++) {
		if (t->columns[t->pk.begin_column + i].type.is_variable)
			key_size += sizeof(uint32_t);
		else
			key_size += t->columns[t->pk.begin_column + i].type.length;
	}
	int data_size = 0;
	for (int i = 0; i < t->size; i++) {
		if (t->columns[i].type.is_variable)
			data_size += sizeof(uint32_t);
		else
			data_size += t->columns[i].type.length;
	}
	return (PAGE_SIZE - sizeof(struct b_plus_node) - sizeof(uint32_t)) / (data_size + sizeof(uint32_t) + key_size);
}
int internal_node_per_page(struct table* t, struct column** c, int n_column)
{
	int key_size = 0;
	for (int i = 0; i < n_column; i++) {
		if (c[i]->type.is_variable)
			key_size += sizeof(uint32_t);
		else
			key_size += c[i]->type.length;
	}
	int b = get_balance_factor(t);
	return (PAGE_SIZE) / (sizeof(struct b_plus_node) + b*key_size + (b + 1)*sizeof(uint32_t));
}
/*node for inerenal node or b+ tree of non clustered index*/
struct b_plus_node* get_new_internal_node(int n)
{
	return  (struct b_plus_node*)malloc(PAGE_SIZE / n);
}
struct b_plus_node* get_new_leaf_node()
{
	uint32_t location = get_block();
	void* p = swap_in(location);
	stain(p);
	return (struct b_plus_node*)p;
}