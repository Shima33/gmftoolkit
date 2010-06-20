// Open Source C GMF Compiler
// Copyright (c) 2008 Sergiusz Bazanski
// http://www.q3k.org/
// GPL License
#include "stdafx.h"

#include "../mystdio.h"
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

extern int isRA1;

void readObjectNodeTM(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*NODE_TM\n");
	tab(preTabNum); fprintf(output, "{\n");


	//Fix padding
	char* databytes = getBytesNF(1);
	while (databytes[0] == '\x00')
	{
		free(databytes);
		free(getBytes(1));
		databytes = getBytesNF(1);
	}
	free(databytes);
		
	free(getBytes(8));
	int totalLength = getInteger();

	databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);	
		myprintf("Decompiling NODE_TM %s...\n", objName);
		free(objName);
	}
	else
	{
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling NODE_TM...\n");
	}
	free(databytes);

		
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
	free(getBytes(8));

	int totalLength = getInteger();

	tab(preTabNum); fprintf(output, "*GMID_ATTACHMENTPT\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling GMID_ATTACHMENTPT %s...\n", objName);
		free(objName);
	}
	else
	{
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling GMID_ATTACHMENTPT...\n");
	}
	free(databytes);

	readObjectNodeTM(preTabNum + 1);

	char* attData = getString();
	tab(preTabNum+1); fprintf(output, "USER DATA\t%s\n", attData);
	free(attData);

	tab(preTabNum); fprintf(output, "}\n");
	myprintf("\n");

}

void readObjectLight(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();

	tab(preTabNum); fprintf(output, "*LIGHT\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling LIGHT %s..\n", objName);
		free(objName);
	}
	else
	{
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling LIGHT...\n");
	}
	free(databytes);

	readObjectNodeTM(preTabNum+1);

	int hasSecondTM = getInteger();

	if (hasSecondTM)
		readObjectNodeTM(preTabNum+1);

	int lightType = getInteger();

	switch(lightType)
	{
		case 0:
			tab(preTabNum+1); fprintf(output, "*LIGHT_TYPE\tOmni\n");
			break;

		default:
			tab(preTabNum+1); fprintf(output, "*LIGHT_TYPE\tOther\n");
			break;
	}

	int shadowType = getInteger();

	switch(shadowType)
	{
		case 2:
			tab(preTabNum+1); fprintf(output, "*LIGHT_SHADOWS\tRaytraced\n");
			break;

		default:
			tab(preTabNum+1); fprintf(output, "*LIGHT_SHADOWS\tOther\n");
			break;
	}

	int useLight = getInteger();
	char* lightColor = getRGB();
	float lightIntensity = getFloat();
	float lightAspect = getFloat();

	float lightAttnStart;
	float lightAttnEnd;

	if (!isRA1)
	{
		free(getBytes(8));
		lightAttnStart = getFloat();
		lightAttnEnd = getFloat();
	}
	else
	{
		lightAttnStart = getFloat();
		lightAttnEnd = getFloat();
		free(getBytes(8));
	}
	float lightTDist = getFloat();

	int lightUseFarAttn;
	if (!isRA1)
		lightUseFarAttn = getInteger();

	tab(preTabNum+1); fprintf(output, "*LIGHT_USELIGHT\t%i\n", useLight);
	tab(preTabNum+1); fprintf(output, "*LIGHT_SPOTSHAPE\tCircle\n");
	tab(preTabNum+1); fprintf(output, "*LIGHT_COLOR\t%s\n", lightColor);
	tab(preTabNum+1); fprintf(output, "*LIGHT_INTENS\t%f\n", lightIntensity);
	tab(preTabNum+1); fprintf(output, "*LIGHT_ASPECT\t%f\n", lightAspect);
	tab(preTabNum+1); fprintf(output, "*LIGHT_ATTNSTART\t%f\n", lightAttnStart);
	tab(preTabNum+1); fprintf(output, "*LIGHT_ATTNEND\t%f\n", lightAttnEnd);
	tab(preTabNum+1); fprintf(output, "*LIGHT_TDIST\t%f\n", lightTDist);
	if (!isRA1)
	{
		tab(preTabNum+1); fprintf(output, "*USE FAR ATTENUATION\t%i\n", lightUseFarAttn);
	}

	tab(preTabNum); fprintf(output, "}\n");
	myprintf("\n");

	free(lightColor);
}

void readCamera(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();

	tab(preTabNum); fprintf(output, "*CAMERA\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling CAMERA %s..\n", objName);
		free(objName);
	}
	else
	{
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling CAMERA...\n");
	}
	free(databytes);

	readObjectNodeTM(preTabNum+1);

	int hasSecondTM = getInteger();

	if (hasSecondTM)
		readObjectNodeTM(preTabNum+1);

	int Type = getInteger();
	float Hither = getFloat();
	float Yon = getFloat();
	float Near = getFloat();
	float Far = getFloat();
	float FOV = getFloat();
	float TDist = getFloat();

	if (Type == 0)
	{
		tab(preTabNum+1); fprintf(output, "*CAMERA_TYPE\tTarget\n");
	}
	else
	{
		tab(preTabNum+1); fprintf(output, "*CAMERA_TYPE\tUnknown\n");
	}

	tab(preTabNum+1); fprintf(output, "*CAMERA_HITHER\t%f\n", Hither);
	tab(preTabNum+1); fprintf(output, "*CAMERA_YON\t%f\n", Yon);
	tab(preTabNum+1); fprintf(output, "*CAMERA_NEAR\t%f\n", Near);
	tab(preTabNum+1); fprintf(output, "*CAMERA_FAR\t%f\n", Far);
	tab(preTabNum+1); fprintf(output, "*CAMERA_FOV\t%f\n", FOV);
	tab(preTabNum+1); fprintf(output, "*CAMERA_TDIST\t%f\n", TDist);

	tab(preTabNum); fprintf(output, "}\n");
	myprintf("\n");
}

void readCollisionBox(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();

	tab(preTabNum); fprintf(output, "*GMID_COLLISION_BOX\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling GMID_COLLISION_BOX %s..\n", objName);
		free(objName);
	}
	else
	{
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling GMID_COLLISION_BOX...\n");
	}
	free(databytes);

	readObjectNodeTM(preTabNum+1);

	float L = getFloat();
	float W = getFloat();
	float H = getFloat();

	tab(preTabNum+1); fprintf(output, "*LENGTH\t%f\n", L);
	tab(preTabNum+1); fprintf(output, "*WIDTH\t%f\n", W);
	tab(preTabNum+1); fprintf(output, "*HEIGHT\t%f\n", H);

	tab(preTabNum); fprintf(output, "}\n");
	myprintf("\n");
}

void readObjectList(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();
	int objCount = getInteger();
	//printf("Decompiling OBJECT_LIST (%i)...\n", objCount);
	
	tab(preTabNum); fprintf(output, "*OBJECT_LIST\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*OBJECT_COUNT\t%i\n", objCount);
	
	
	int i;
	for (i = 0; i < objCount; i++)
	{
		char* peekData = getBytesNF(8);
		// RA2 Geometry
		if (!memcmp(peekData, "\x02\x00\x00\x00\x04\x00\x00\x00", 8))
		{
			readObjectGeo(preTabNum+1);
		}
		
		// RA1 Geometry
		else if (!memcmp(peekData, "\x02\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readObjectGeo(preTabNum+1);
		}

		// RA2 Light
		else if (!memcmp(peekData, "\x05\x00\x00\x00\x03\x00\x00\x00", 8))
		{
			readObjectLight(preTabNum + 1);
		}

		// RA1 Light
		else if (!memcmp(peekData, "\x05\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readObjectLight(preTabNum + 1);
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
		
		//Constraint Solver
		else if (!memcmp(peekData, "\x2A\x00\x00\x00\x03\x00\x00\x00", 8))
		{
			readConstraintSolver(preTabNum + 1);
		}

		//Angular Dashpot
		else if (!memcmp(peekData, "\x31\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readAngularDashpot(preTabNum + 1);
		}

		//RA1 Collision Mesh
		else if (!memcmp(peekData, "\x18\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readCollisionMesh(preTabNum + 1);
		}

		//Camera
		else if (!memcmp(peekData, "\x04\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readCamera(preTabNum+1);
		}

		// RA1 Collision Box
		else if (!memcmp(peekData, "\x19\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readCollisionBox(preTabNum+1);
		}
		free(peekData);
	}
	tab(preTabNum); fprintf(output, "}");
	//free(peekData);
}
