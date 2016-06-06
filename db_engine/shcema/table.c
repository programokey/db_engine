#include "table.h"
#include "schema.h"
#include "../stddef.h"
/*create a relation on given information*/
struct table create_table(struct schema* s,char* name, struct column columns[],
	int size, struct primary_key pk, bool(*constraint_func)(byte*, struct table*))
{
	struct table t;
	strcpy(t.name,name);
	t.size = size;
	for (int i = 0; i < size; i++)
		t.columns[i] = columns[i];
	t.pk = pk;
	t.constraint_func = constraint_func;
	t.root = NULL;
	add_table(s,t);
	return t;
}