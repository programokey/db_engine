#include <stdlib.h>
#include <stdio.h>
#include "schema.h"
#include "database.h"
#include <time.h>
void create_table(struct schema* s, struct table t)
{
	assert(s->size < MAX_TABLES - 1);
	s->tables[s->size] = t;
}

void load_schema(const char* path)
{
	file = fopen(path, "wb+");
	if (file == NULL) {
		printf("load schema fail!\a\n");
		return;
	}
	fread(&s,sizeof(s),1,file);
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