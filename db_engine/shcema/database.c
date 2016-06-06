#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
void load_database()
{
	FILE* f = fopen("..//database//schemas.db","rb");
	if (f == NULL)
		return;
	fseek(f,0,0);
	fread(&schemas_num,sizeof(int),1,f);
	fseek(f, 0, sizeof(int));
	if (database)
		free(database);
	database = (struct schema*)malloc(sizeof(struct schema)*schemas_num);
	fread(database, sizeof(struct schema), schemas_num, f);
	fclose(f);
}

void save_database()
{
	FILE* f = fopen("..//database//schemas.db", "wb");
	if (f == NULL || database == NULL)
		return;
	fseek(f, 0, 0);
	fwrite(&schemas_num, sizeof(int), 1, f);
	fseek(f, 0, sizeof(int));
	fwrite(database, sizeof(struct schema), schemas_num, f);
	fclose(f);
}