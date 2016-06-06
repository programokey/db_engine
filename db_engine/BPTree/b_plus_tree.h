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

/*one page may contain many internal node*/
int internal_node_per_page(struct table* t, struct column** c, int n_column);

/*node for inerenal node or b+ tree of non clustered index*/
struct b_plus_node* get_new_internal_node(int n);

/**/
struct b_plus_node* get_new_leaf_node();
#endif // !BPTREE_B_PLUS_TREE_H