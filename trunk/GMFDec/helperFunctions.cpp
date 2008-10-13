// Open Source C GMF Compiler
// Copyright (c) 2008 Sergiusz Bazanski
// http://www.q3k.org/
// GPL License
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>

extern FILE *source;
extern FILE *output;

#ifdef _BIG_ENDIAN
#define endian_swap32(x) ((((x) & 0xff000000) >> 24) | \
			  (((x) & 0x00ff0000) >>  8) | \
			  (((x) & 0x0000ff00) <<  8) | \
			  (((x) & 0x000000ff) << 24))
#else
#define endian_swap32(x) (x)
#endif

void tab(int num)
{
	int i;
	for (i = 0; i < num; i++)
		fprintf(output, "\t");
}

void debugHex(char* in, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%X ", in[i]);
	}
	printf("\n");
}