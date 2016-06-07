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
	data_size += t->size / 8 + (t->size & 0x7) == 0 ? 0 : 1;
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

int pk_internal_node_per_page(struct table* t)
{
	int key_size = 0;
	for (int i = 0; i < t->pk.size; i++) {
		if (t->columns[i].type.is_variable)
			key_size += sizeof(uint32_t);
		else
			key_size += t->columns[i].type.length;
	}
	int b = get_balance_factor(t);
	return (PAGE_SIZE) / (sizeof(struct b_plus_node) + b*key_size + (b + 1)*sizeof(uint32_t));
}

int get_internal_node_size(struct table* t, struct column** c, int n_column)
{
	int key_size = 0;
	for (int i = 0; i < n_column; i++) {
		if (c[i]->type.is_variable)
			key_size += sizeof(uint32_t);
		else
			key_size += c[i]->type.length;
	}
	int b = get_balance_factor(t);
	return (sizeof(struct b_plus_node) + b*key_size + (b + 1)*sizeof(uint32_t));
}
int get_pk_internal_node_size(struct table* t)
{
	int key_size = 0;
	for (int i = 0; i < t->pk.size; i++) {
		if (t->columns[i].type.is_variable)
			key_size += sizeof(uint32_t);
		else
			key_size += t->columns[i].type.length;
	}
	int b = get_balance_factor(t);
	return (sizeof(struct b_plus_node) + b*key_size + (b + 1)*sizeof(uint32_t));
}


byte* get_key(struct b_plus_node* p, int i)
{
	byte* q = disk_to_memory(p->key);
	return q + p->key_size*i;
}
struct b_plus_node* get_child(struct b_plus_node* p, int i)
{
	return disk_to_memory(*(uint32_t*)((byte*)disk_to_memory(p->child) + sizeof(word) * i));
}
struct b_plus_node* get_parent(struct b_plus_node* p)
{
	return disk_to_memory(p->parent);
}
struct table* get_table(struct b_plus_node* p)
{
	return disk_to_memory(p->t);
}

void set_key(struct b_plus_node* p, int i, byte* key)
{
	byte* q = (byte*)disk_to_memory(p->key) + p->key_size*i;
	memory_copy(q, key, p->key_size);
}
void set_parent(struct b_plus_node* p, struct b_plus_node* parent)
{
	p->parent = memory_to_disk(parent);
}
void set_child(struct b_plus_node* p, int i, struct b_plus_node* child)
{
	struct b_plus_node** q = (byte*)disk_to_memory(p->child) + sizeof(word) * i;
	*q = memory_to_disk(child);
}
void set_table(struct b_plus_node* p, struct table* t)
{
	p->t = memory_to_disk(t);
}



int get_key_size(struct table* t, struct column** c, int n_column)
{
	int key_size = 0;
	for (int i = 0; i < n_column; i++) {
		if (c[i]->type.is_variable)
			key_size += sizeof(uint32_t);
		else
			key_size += c[i]->type.length;
	}
	return key_size;
}