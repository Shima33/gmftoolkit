// Open Source C GMF Compiler
// Copyright (c) 2008 Sergiusz Bazanski
// http://www.q3k.org/
// GPL License
#include "stdafx.h"

#include "../mystdio.h"
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <windows.h>

#include "byteFunctions.h"
#include "materialFunctions.h"
#include "helperFunctions.h"
#include "sceneFunctions.h" 
#include "objectGeoFunctions.h"
#include "objectFunctions.h"

FILE *source, *output;

#ifdef _BIG_ENDIAN
#define endian_swap32(x) ((((x) & 0xff000000) >> 24) | \
			  (((x) & 0x00ff0000) >>  8) | \
			  (((x) & 0x0000ff00) <<  8) | \
			  (((x) & 0x000000ff) << 24))
#else
#define endian_swap32(x) (x)
#endif

int isRA1;

int decompile(HWND parent, const LPWSTR sourcePath, const LPWSTR outputPath)
{
	isRA1 = 0;
	_wfopen_s(&source, sourcePath, L"rb");
	_wfopen_s(&output, outputPath, L"w");
	
	if (source == 0 || output == 0)
	{
		myprintf("No file selected.");
		return 1;
	}
	
	char *header = getBytes(3);
	
	if(strncmp(header, "GMI", 3))
	{
		myprintf("This is not a valid binary GMF file!\n");
		return 1;
	}
	free(header);
	
	myprintf("Decompiling GMI...\n\n");


	int gmfVersion = getInteger();
	
	fprintf(output, "GMA\n");
	fprintf(output, "*GABRIEL_ASCIIEXPORT\t%i\n", gmfVersion);
	while(!feof(source))
	{
		char* objectType = getBytesNF(8);
		
		// Model Type
		if (!memcmp(objectType, "\x01\x00\x00\x00\x00\x00\x00\x00", 8) || !memcmp(objectType, "\x03\x00\x00\x00\x00\x00\x00\x00", 8) || !memcmp(objectType, "\x02\x00\x00\x00\x00\x00\x00\x00", 8))
		{
			if (!memcmp(objectType, "\x03\x00\x00\x00\x00\x00\x00\x00", 8) || !memcmp(objectType, "\x02\x00\x00\x00\x00\x00\x00\x00", 8))
				isRA1 = 1;
			else
				isRA1 = 0;
			free(getBytes(8));
			int type = getInteger();
			
			if (type == 15)
			{
				fprintf(output, "*MODEL_TYPE\tBasic Model\n");
			}
			else
			{
				myprintf("Unknown Model Type: %d\n", type);
				return 1;
			}
		}

		// Scene Info
		else if (!memcmp(objectType, "\x01\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readSceneInfo();
		}
		
		// Material Info
		else if (!memcmp(objectType, "\x07\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readMaterialList(0);
			
		}
		
		//Object List
		else if (!memcmp(objectType, "\x12\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readObjectList(0);			
		}
		else
		{
			free(getBytes(8));
			if (fgetc(source) == EOF)
				break;
			myprintf("Object type unknown!\n");
			debugHex(objectType, 8);
			MessageBox(parent, L"Error! Unknown Object Type!\n",L"Decompilation unsuccessfull!", MB_OK | MB_ICONWARNING);
			free(objectType);
			return 1;
		}

		free(objectType);
	}
	myprintf("Decompiled sucessfully!\n");
	MessageBox(parent, L"Binary GMF decompiled sucessfully!" ,L"Decompilation successfull!", MB_OK);
	fclose(source);
	fclose(output);
	return 0;
}
