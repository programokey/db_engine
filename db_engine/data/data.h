#ifndef DATA_DATA_H
#define DATA_DATA_H
#include "../stddef.h"
struct data
{
    bool is_variable;
	/*average length for variable length data
	length for fix length data*/
    int length;
    char* (*output_func)(byte*,int);
    void (*input_func)(char*,byte*,int*);
};
void store_variable_data(byte* destination, byte* source);
struct data fix_length_data(int length, char* (*output_func)(byte*, int), void(*input_func)(char*, byte*, int*));
struct data variable_length_data(int length, char* (*output_func)(byte*, int), void(*input_func)(char*, byte*, int*));
#endif