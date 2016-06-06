#include <stdlib.h>
#include "schema.h"
#include "database.h"
#include "time.h"
void add_table(struct schema* s, struct table t)
{
	assert(s->size < MAX_TABLES - 1);
	s->tables[s->size] = t;
}

struct schema create_schema(char* name)
{
	struct schema s;
	strcpy(s.name, name);
	s.size = 0;
	s.timestamp = time(NULL);
	database[schemas_num++] = s;
	return s;
}