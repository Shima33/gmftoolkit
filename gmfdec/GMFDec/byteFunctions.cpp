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

extern int isRA1;

char* getBytes(int num)
{
	char* datab = (char*)malloc(sizeof(char)*(num));
	//char* data = new char[num];
	/*int i;
	for(i = 0; i < num; i++)
	{
		data[i] = fgetc(source);
	}*/
	fread(datab, sizeof(char), num, source);
	return datab;
}

char* getBytesNF(int num)
{
	char* data = (char*)malloc(sizeof(char)*(num+1));
	fread(data, sizeof(char), num, source);
	fseek(source, num * -1, 1);
	return data;
}

int getInteger()
{
	int ret;
	fread(&ret, sizeof(int), 1, source);
	return endian_swap32(ret);
}

int getIntegerNF()
{
	int ret;
	fread(&ret, sizeof(int), 1, source);
	fseek(source, -4, 1);
	return endian_swap32(ret);
}

char* getString()
{
	int i = 0;
	int size = getInteger();

	if (size == 0)
		return "";

	char* data = (char*)malloc(sizeof(char)*(size));
	for(i = 0; i <= size-1; i++)
	{
		data[i] = fgetc(source);
	}
	return data;
}

char* getRGB()
{
	unsigned char* in = (unsigned char*)getBytes(3);
	getBytes(1);
	char* data = (char*)malloc(sizeof(char)*9);
	if (!isRA1)
		sprintf(data, "0x%02X%02X%02X", in[0], in[1], in[2]);
	else
		sprintf(data, "0x%02X%02X%02X", in[2], in[1], in[0]);
	//free(in);
	return data;
}

float getFloat()
{
	char bytes[sizeof(float)];
	fread(bytes, sizeof(float), 1, source);
	(*(unsigned int *)bytes) = endian_swap32(*(unsigned int *)bytes);
	return *(float *)bytes;
}
