#ifndef STDDEF_H
#define STDDEF_H
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef unsigned char byte;
typedef uint32_t word;

#define NULL 0
#define MAX_NAME 63
#define PAGE_SIZE 0x10000 //64KB
#define BLOCK_SIZE 0x10000 //64KB

/*macro function for swap,
only can be used to variables which type could do xor operation*/
#define swap(a, b) \
 {(a) ^= (b);\
(b) ^= (a);\
(a) ^= (b); }
bool get_bit(byte a, int i);
void set_bit(byte* a, int i, bool value);
FILE* file;
#endif