#include "column.h"
#include "../stddef.h"

struct foreign_key get_foreign_key(struct table* t,struct column* c)
{
	struct foreign_key fk;
	fk.reference_table = t;
	fk.reference_colunm = c;
	return fk;
}
struct column get_column(char * name, struct data type,	bool not_null, bool unique, struct foreign_key fk,
	bool has_default,byte * default_value, int default_value_length, byte* (*next)(byte*, struct data), byte* current)
{
	struct column c;
	strcpy(c.name, name);
	c.fk = fk;
	c.type = type;
	set_bit(&c.attribute, 0, not_null);
	set_bit(&c.attribute, 1, unique);
	set_bit(&c.attribute, 2, has_default);
	c.default_value = default_value;
	c.default_value_length = default_value_length;
	c.next = next;
	c.current = current;
	return c;
}