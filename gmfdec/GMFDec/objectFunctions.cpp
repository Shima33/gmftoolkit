// Open Source C GMF Compiler
// Copyright (c) 2008 Sergiusz Bazanski
// http://www.q3k.org/
// GPL License
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>

#include "byteFunctions.h"
#include "helperFunctions.h"
#include "objectGeoFunctions.h"
#include "havokFunctions.h"

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

void readObjectNodeTM(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*NODE_TM\n");
	tab(preTabNum); fprintf(output, "{\n");


	//Fix padding
	while (getBytesNF(1)[0] == '\x00')
		getBytes(1);
		
	getBytes(8);
	int totalLength = getInteger();

	if (getBytesNF(6)[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);	
		//free(objName);
	}
	else
	{
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
	}

		
	// Create and fill the Transformation Matrix
	float TM[4][3];
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 3; j++)
		{
			TM[i][j] = getFloat();
		}
	}
	
	
	for (i = 0; i < 4; i++)
	{
		tab(preTabNum + 1);  fprintf(output, "*TM_ROW%i %f\t%f\t%f\n", i, TM[i][0], TM[i][1], TM[i][2]);
	}
	
	tab(preTabNum); fprintf(output, "}\n");
}

void readObjectAttachment(int preTabNum)
{
	getBytes(8);

	int totalLength = getInteger();

	tab(preTabNum); fprintf(output, "*GMID_ATTACHMENTPT\n");
	tab(preTabNum); fprintf(output, "{\n");

	if (getBytesNF(6)[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
	}
	else
	{
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
	}

	readObjectNodeTM(preTabNum + 1);

	char* attData = getString();
	tab(preTabNum+1); fprintf(output, "USER DATA\t%s\n", attData);

	tab(preTabNum); fprintf(output, "}\n");

}

void readObjectList(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	int objCount = getInteger();
	
	tab(preTabNum); fprintf(output, "*OBJECT_LIST\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*OBJECT_COUNT\t%i\n", objCount);
	
	
	int i;
	for (i = 0; i < objCount; i++)
	{
		char* peekData = getBytesNF(8);
		// Geometry
		if (!memcmp(peekData, "\x02\x00\x00\x00\x04\x00\x00\x00", 8))
		{
			readObjectGeo(preTabNum+1);
		}
		
		//Light
		else if (!memcmp(peekData, "\x05\x00\x00\x00\x03\x00\x00\x00", 8))
		{
			
		}
		
		//Attachement
		else if (!memcmp(peekData, "\x15\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readObjectAttachment(preTabNum+1);
		}
		
		//SimObject
		else if (!memcmp(peekData, "\x1E\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readSimObject(preTabNum+1);
		}
		
		//RBCollection
		else if (!memcmp(peekData, "\x1F\x00\x00\x00\x04\x00\x00\x00", 8))
		{
			readRBCollection(preTabNum + 1);
		}
		
		//COnstraint Solver
		else if (!memcmp(peekData, "\x2A\x00\x00\x00\x03\x00\x00\x00", 8))
		{
			
		}
	}
	
	tab(preTabNum); fprintf(output, "}");
	//free(peekData);
}
