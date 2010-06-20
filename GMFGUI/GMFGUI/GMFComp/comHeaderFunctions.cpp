#include "stdafx.h"

#include "../mystdio.h"
#include <stdlib.h>
#include <string.h>

#include "byteFunctions.h"
#include "asciiFunctions.h"

extern FILE* input;
extern FILE* output;

int readHeader()
{
	char* headerGMA = (char*)malloc(sizeof(char)*32);
	fscanf(input, "%s\n", headerGMA);

	if (strncmp(headerGMA, "GMA", 3))
	{
		printf("readHeader(): GMA header not found!\n");
		return 1;
	}

	fprintf(output, "GMI");

	int headerVersion = readInt("*GABRIEL_ASCIIEXPORT");
	char* headerModelType = readString("*MODEL_TYPE");

	printInt(headerVersion);

	if (strncmp(headerModelType, "Basic Model", strlen(headerModelType)))
	{
		printf("readHeader(): *MODEL_TYPE is not \"Basic Model\"!\n");
		return 1;
	}

	printBytes("\x01\x00\x00\x00\x00\x00\x00\x00\x0F\x00\x00\x00", 12);

	return 0;
}
