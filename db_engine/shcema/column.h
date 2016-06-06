#ifndef SCHEMA_COLUMN_H
#define SCHEMA_COLUMN_H
#include <stdint.h>
#include "../stddef.h"
#include "../data/data.h"
#define COLUMN_NAME 63
/*Specify the table and column the foreign key references to.
The two member must be both null or both not null.
It indicate this column is not a foreign key if the both member
are null*/
struct foreign_key
{
	struct table* reference_table;
	struct column* reference_colunm;
};

struct column
{
	char name[COLUMN_NAME];
	struct data type;
	struct foreign_key fk;
	/*the first bit form right indiacte not_null(1 for not null);
	the second bit form right indiacte unique(1 for unique);
	the thrid bit form right indiacte has_default(1 for has default);
	*/
	byte attribute;
	bool not_null;
	bool unique;
	bool has_default;
	byte* default_value;
	int default_value_length;
	/*Record the column how to auto-increase and current value.
	It indicates this column will not increase automatically
	that the function pointer next is null*/
	byte* (*next)(byte*, struct data);
	/*record current value for auto increasement,
	null if this column will not increase automatically	*/
	byte* current;
};
/*return foreign ket structure on given table and column*/
struct foreign_key get_foreign_key(struct table* t, struct column* c);

/*return a column structure on given innformation*/
struct column get_column(char* name, struct data type,bool not_null, bool unique, struct foreign_key fk,
	bool has_default, byte* default_value, int default_value_length, byte* (*next)(byte*, struct data), byte* current);
#endif // !