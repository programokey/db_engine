#include "stddef.h"
bool get_bit(byte a, int i)
{
	return ((1 << i)&(a)) == 0 ? false : true;
}
void set_bit(byte* a, int i, bool value)
{
	if (get_bit(*a, i) != value)
		*a ^= (1 << i);
}