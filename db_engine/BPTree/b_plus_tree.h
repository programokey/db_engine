#ifndef BPTREE_B_PLUS_TREE_H
#define BPTREE_B_PLUS_TREE_H
#include "../stddef.h"
#include "../shcema/table.h"


struct b_plus_node
{
	bool is_leaf;
	int size;
	uint32_t key;//byte** key;
	uint32_t child;//struct b_plus_node** child;
	uint32_t parent;//struct b_plus_node* parent;
	uint32_t t;//struct table* t;
};
/*return balance factor fit the given table.
one page per leaf node*/
int get_balance_factor(struct table* t);

int get_internal_node_size(struct table* t);

#endif // !BPTREE_B_PLUS_TREE_H