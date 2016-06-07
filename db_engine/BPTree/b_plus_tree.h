#ifndef BPTREE_B_PLUS_TREE_H
#define BPTREE_B_PLUS_TREE_H
#include "../stddef.h"
#include "../shcema/table.h"
#include "../storage/memory.h"
#include "../storage/disk.h"

struct b_plus_node
{
	bool is_leaf;
	int size;
	uint32_t key;//byte** key;
	uint32_t child;//struct b_plus_node** child;
	uint32_t parent;//struct b_plus_node* parent;
	uint32_t t;//struct table* t;
	uint32_t key_size;
};

/*pretend b_plus_node is a class*/
/*getter*/
byte* get_key(struct b_plus_node* p, int i);
struct b_plus_node* get_child(struct b_plus_node* p, int i);
struct b_plus_node* get_parent(struct b_plus_node* p);
struct table* get_table(struct b_plus_node* p);

/*setter*/
void set_key(struct b_plus_node* p, int i, byte* key);
void set_parent(struct b_plus_node* p, struct b_plus_node* parent);
void set_child(struct b_plus_node* p, int i, struct b_plus_node* child);
void set_table(struct b_plus_node* p, struct table* t);

int get_key_size(struct table* t, struct column** c, int n_column);
/*return balance factor fit the given table.
one page per leaf node*/
int get_balance_factor(struct table* t);

int internal_node_per_page(struct table* t, struct column** c, int n_column);

int pk_internal_node_per_page(struct table* t);

int get_internal_node_size(struct table* t, struct column** c, int n_column);
int get_pk_internal_node_size(struct table* t);
#endif // !BPTREE_B_PLUS_TREE_H