#ifndef SCHEMA_DATABASE_H
#define SCHEMA_DATABASE_H
#include "schema.h"

int schemas_num;
struct schema* database;

/*load schema from file*/
void load_database();
/*save current schemas_num and array database[] to file*/
void save_database();
#endif // !SCHEMA_DATABASE_H
