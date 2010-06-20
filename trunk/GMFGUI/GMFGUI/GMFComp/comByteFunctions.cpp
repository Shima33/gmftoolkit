#include "stdafx.h"

#include "../mystdio.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern FILE* input;
extern FILE* output;

int printBytes(char* byteArray, int num)
{
	fwrite(byteArray, sizeof(char), num, output);
	return 0;
}

int printInt(int in)
{
	fwrite(&in, sizeof(int), 1, output);
	return 0;
}

int printString(char* in)
{
	int size = (int)strlen(in);
	printInt(size+1);
	fwrite(in, sizeof(char), size+1, output);
	return 0;
}

int printFloat(float in)
{
	fwrite(&in, sizeof(float), 1, output);
	return 0;
}
