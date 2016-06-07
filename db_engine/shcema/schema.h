#ifndef SCHEMA_SCHEMA_H
#define SCHEMA_SCHEMA_H
#include "table.h"
#include "../stddef.h"
#define MAX_TABLES 127
struct schema
{
    char name[MAX_NAME];
    struct table tables[MAX_TABLES];
	int size;
	time_t timestamp;
};
struct schema s;
int a[sizeof(struct schema)/1024/64];
/*creat a new schema on given name*/
struct schema create_schema(char* name);
/*add a new table to a schema*/
void add_table(struct schema* s, struct table t);
#endif