#include <stdio.h>
#include <time.h>
#include "../db_engine/stddef.h"
int main()
{
	int a = 5;
	printf("sizeof(void*) = %d\n", sizeof(&a));
	return 0;
}
