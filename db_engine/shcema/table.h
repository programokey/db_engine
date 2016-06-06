#ifndef SCHEMA_TABLE_H
#define SCHEMA_TABLE_H
#include <stdint.h>
#include "../BPTree/b_plus_tree.h"
#include "../stddef.h"
#include "column.h"

#define MAX_COLUMN  63

/*Record primary key of table,
there exists a primary key for every relation.
If user does not specify a primary key,
database will add a column that records time
of the record is inserted
if primary key is on multi-column
these column must be continual in the relation*/
struct primary_key
{
    uint32_t begin_column;
    uint32_t size;
};

struct table
{
	char name[MAX_NAME];
	struct column columns[MAX_COLUMN];
	int size;
	struct primary_key pk;
	uint32_t root;
    /*specify the additional constraint on this table.
    It indicate this table does not have any additional constraint*/
    bool (*constraint_func)(byte*, struct table*);
};
#endif