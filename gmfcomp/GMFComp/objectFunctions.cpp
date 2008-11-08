#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "byteFunctions.h"
#include "asciiFunctions.h"
#include "gmidFunctions.h"

extern FILE* input;
extern FILE* output;

void debugHex(char* in, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%02X ", (unsigned char)in[i]);
	}
	printf("\n");
}

int readGeoMesh()
{
	printInt(16);
	printInt(4);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	readNothing("*MESH");
	openBracket();

	int meshTimeValue = readInt("*TIMEVALUE");
	int meshNumVertex = readInt("*MESH_NUMVERTEX");
	int meshNumFaces = readInt("*MESH_NUMFACES");

	printInt(meshTimeValue);
	printInt(meshNumVertex);
	printInt(meshNumFaces);

	int meshNumTVertexOffset = ftell(output);
	printInt(0);

	int meshNumCVertexOffset = ftell(output);
	printInt(0);

	int meshMatRefOffset = ftell(output);
	printInt(0);

	int meshNumTVertex = 0;
	int meshNumCVertex = 0;

	// Loop da loop, baby.
	char* nextMeshThing = (char*)malloc(sizeof(char)*32);
	char* meshCStuff = (char*)malloc(sizeof(char)*128000);
	char* meshNormals = (char*)malloc(sizeof(char)*128000);
	char* meshSecondary = (char*)malloc(sizeof(char)*128000);
	int meshCStuffPos = 0;
	int meshNormalPos = 0;
	int meshSecondaryPos = 0;
	int matBackFace = 0;
	int meshNumTVFaces = 0;
	int meshNumCVFaces = 0;
	while (1)
	{
		fscanf(input, "%s\n", nextMeshThing);

		if(!strncmp(nextMeshThing, "*MESH_VERTEX_LIST", 17))
		{
			openBracket();
			float f[3];
			int i;
			for (i = 0; i < meshNumVertex; i++)
			{
				bracketize();
				fscanf(input, "*%*s\t%*i\t%f\t%f\t%f\n", &f[0], &f[1], &f[2]);
				printFloat(f[0]);
				printFloat(f[1]);
				printFloat(f[2]);
			}
			closeBracket();
		}
		else if(!strncmp(nextMeshThing, "*MESH_FACE_LIST", 15))
		{
			openBracket();
			int f[4];
			int i;
			for (i = 0; i < meshNumFaces; i++)
			{
				bracketize();
				fscanf(input, "*MESH_FACE\t%*i\tA:%i\tB:%i\tC:%i\t%*s\t%i", &f[0], &f[1], &f[2], &f[3]);
				printInt(f[0]);
				printInt(f[1]);
				printInt(f[2]);
				printInt(f[3]);
			}
			closeBracket();
		}
		else if(!strncmp(nextMeshThing, "*MESH_NUMTVERTEX", 17))
		{
			int numTVertex;
			fscanf(input, "\t%i\n", &numTVertex);
			int currentOffset = ftell(output);
			fseek(output, meshNumTVertexOffset, 0);
			printInt(numTVertex);
			fseek(output, currentOffset, 0);
			meshNumTVertex = numTVertex;
		}
		else if(!strncmp(nextMeshThing, "*MESH_NUMCVERTEX", 17))
		{
			int numCVertex;
			fscanf(input, "\t%i\n", &numCVertex);
			int currentOffset = ftell(output);
			fseek(output, meshNumCVertexOffset, 0);
			printInt(numCVertex);
			fseek(output, currentOffset, 0);
			meshNumCVertex = numCVertex;
		}
		else if(!strncmp(nextMeshThing, "*MESH_NUMTVFACES", 17))
		{
			fscanf(input, "\t%i\n", &meshNumTVFaces);
		}
		else if(!strncmp(nextMeshThing, "*MESH_NUMCVFACES", 17))
		{
			fscanf(input, "\t%i\n", &meshNumCVFaces);
		}
		else if(!strncmp(nextMeshThing, "*MESH_NUMTFACES", 17))
		{
			fscanf(input, "\t%i\n", &meshNumTVFaces);
		}
		else if(!strncmp(nextMeshThing, "*MESH_TVERTLIST", 16))
		{
			openBracket();
			float f[3];
			int i;
			for (i = 0; i < meshNumTVertex; i++)
			{
				bracketize();
				fscanf(input, "*%*s\t%*i\t%f\t%f\t%f\n", &f[0], &f[1], &f[2]);
				printFloat(f[0]);
				printFloat(f[1]);
				printFloat(f[2]);
			}
			closeBracket();
		}
		else if(!strncmp(nextMeshThing, "*MESH_CVERTLIST", 16))
		{
			openBracket();
			float f[3];
			int i;
			for (i = 0; i < meshNumCVertex; i++)
			{
				bracketize();
				fscanf(input, "*%*s\t%*i\t%f\t%f\t%f\n", &f[0], &f[1], &f[2]);
				int x;
				for (x = 0; x < 3; x++)
				{
					addToString(meshCStuff, (char*)&f[x], 4, meshCStuffPos);
					meshCStuffPos += 4;
				}
			}
			closeBracket();
		}
		else if(!strncmp(nextMeshThing, "*MESH_TFACELIST", 16))
		{
			openBracket();
			int f[3];
			int i;
			for (i = 0; i < meshNumTVFaces; i++)
			{
				bracketize();
				fscanf(input, "*%*s\t%*i\t%i\t%i\t%i\n", &f[0], &f[1], &f[2]);
				printInt(f[0]);
				printInt(f[1]);
				printInt(f[2]);
			}
			closeBracket();
		}
		else if(!strncmp(nextMeshThing, "*MESH_CFACELIST", 16))
		{
			openBracket();
			int f[3];
			int i;
			for (i = 0; i < meshNumFaces; i++)
			{
				bracketize();
				fscanf(input, "*%*s\t%*i\t%i\t%i\t%i\n", &f[0], &f[1], &f[2]);
				int x;
				for (x = 0; x < 3; x++)
				{
					addToString(meshCStuff, (char*)&f[x], 4, meshCStuffPos);
					meshCStuffPos += 4;
				}
			}
			closeBracket();
		}
		else if(!strncmp(nextMeshThing, "*MESH_NORMALS", 16))
		{
			openBracket();
			float f[3];
			int i;
			for (i = 0; i < meshNumFaces; i++)
			{
				bracketize();
				fscanf(input, "*%*s\t%*i\t%f\t%f\t%f\n", &f[0], &f[1], &f[2]);
				int x;
				for (x = 0; x < 3; x++)
				{
					addToString(meshNormals, (char*)&f[x], 4, meshNormalPos);
					meshNormalPos += 4;
				}
				
				int j;
				for (j = 0; j < 3; j++)
				{
					fscanf(input, "\t*%*s\t%*i\t%f\t%f\t%f\n", &f[0], &f[1], &f[2]);
					int x;
					for (x = 0; x < 3; x++)
					{
						addToString(meshNormals, (char*)&f[x], 4, meshNormalPos);
						meshNormalPos += 4;
					}
				}
			}
			closeBracket();

		}
		else if(!strncmp(nextMeshThing, "*BACKFACE_CULL", 14))
		{
			fscanf(input, "\t%i\n", &matBackFace);
		}
		else if(!strncmp(nextMeshThing, "*MATERIAL_REF", 13))
		{
			int matRefInt;
			fscanf(input, "\t%i\n", &matRefInt);
			int currentOffset = ftell(output);
			fseek(output, meshMatRefOffset, 0);
			printInt(matRefInt);
			fseek(output, currentOffset, 0);
		}
		else if(!strncmp(nextMeshThing, "*MESH_MAPPINGCHANNEL", 13))
		{
			int zero = 0;
			char* unknBuffer = "\x01\x00\x00\x00\x01\x00\x00\x00";
			addToString(meshSecondary, unknBuffer, 8, meshSecondaryPos);
			meshSecondaryPos +=8;

			int mappingChannel;
			fscanf(input, "\t%i\n", &mappingChannel);
			addToString(meshSecondary, (char*)&mappingChannel, 4, meshSecondaryPos);
			meshSecondaryPos += 4;

			openBracket();
			int numTVert = readInt("*MESH_NUMTVERTEX");
			char* TVertBuffer = (char*)malloc(sizeof(char)*128000);
			int TVertBufferPos = 0;
			readNothing("*MESH_TVERTLIST");
			openBracket();
			int i;
			for (i = 0; i < numTVert; i++)
			{
				float f[3];
				bracketize();
				fscanf(input, "*MESH_TVERT\t%*i\t%f\t%f\t%f\n", &f[0], &f[1], &f[2]);
				addToString(TVertBuffer, (char*)&f, 12, TVertBufferPos);
				TVertBufferPos += 12;
			}
			closeBracket();

			int numTFace = readInt("*MESH_NUMTVFACES");
			char* TFaceBuffer = (char*)malloc(sizeof(char)*128000);
			int TFaceBufferPos = 0;
			readNothing("*MESH_TFACELIST");
			openBracket();
			for (i = 0; i < numTFace; i++)
			{
				int f[3];
				bracketize();
				fscanf(input, "*MESH_TFACE\t%*i\t%i\t%i\t%i\n", &f[0], &f[1], &f[2]);
				addToString(TFaceBuffer, (char*)&f, 12, TFaceBufferPos);
				TFaceBufferPos += 12;
			}
			closeBracket();

			closeBracket();

			addToString(meshSecondary, (char*)&numTVert, 4, meshSecondaryPos);
			meshSecondaryPos +=4;

			addToString(meshSecondary, (char*)&zero, 4, meshSecondaryPos);
			meshSecondaryPos +=4;

			addToString(meshSecondary, (char*)&numTFace, 4, meshSecondaryPos);
			meshSecondaryPos +=4;

			addToString(meshSecondary, (char*)&zero, 4, meshSecondaryPos);
			meshSecondaryPos +=4;

			addToString(meshSecondary, (char*)&zero, 4, meshSecondaryPos);
			meshSecondaryPos +=4;

			addToString(meshSecondary, (char*)&numTVert, 4, meshSecondaryPos);
			meshSecondaryPos +=4;

			addToString(meshSecondary, (char*)&numTFace, 4, meshSecondaryPos);
			meshSecondaryPos +=4;

			addToString(meshSecondary, TVertBuffer, TVertBufferPos, meshSecondaryPos);
			meshSecondaryPos += TVertBufferPos;

			addToString(meshSecondary, TFaceBuffer, TFaceBufferPos, meshSecondaryPos);
			meshSecondaryPos += TFaceBufferPos;
		}
		else if(!strncmp(nextMeshThing, "}", 1))
		{
			break;
		}
		else
		{
			char* error = (char*)malloc(sizeof(char)*128);
			sprintf(error, "Unknown mesh parameter %s! Ignore and continue?", nextMeshThing);
			YesNoError(error, "An error has occured!");
		}
		
	}

	if (meshSecondaryPos == 0)
		printInt(0);
	else
		fwrite(meshSecondary, 1, meshSecondaryPos, output);

	fwrite(meshCStuff, 1, meshCStuffPos, output);

	fwrite(meshNormals, 1, meshNormalPos, output);

	printInt(matBackFace);


	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}

int readTM()
{
	printInt(17);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);

	readNothing("*NODE_TM");
	openBracket();

	char* tmName = readString("*NODE_NAME");
	printString(tmName);

	float t[12];

	bracketize(); fscanf(input, "*TM_ROW0\t%f\t%f\t%f\n", &t[0], &t[1], &t[2]);
	bracketize(); fscanf(input, "*TM_ROW1\t%f\t%f\t%f\n", &t[3], &t[4], &t[5]);
	bracketize(); fscanf(input, "*TM_ROW2\t%f\t%f\t%f\n", &t[6], &t[7], &t[8]);
	bracketize(); fscanf(input, "*TM_ROW3\t%f\t%f\t%f\n", &t[9], &t[10], &t[11]);

	int i;
	for (i = 0; i < 12; i++)
	{
		printFloat(t[i]);
	}

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}

int readLight()
{
	printInt(5);
	printInt(3);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);

	openBracket();

	char* objName = readString("*NODE_NAME");
	printString(objName);

	readTM();

	printInt(0); printInt(0);

	readString("*LIGHT_TYPE");

	char* lightType = readString("*LIGHT_SHADOWS");

	if (!memcmp(lightType, "Raytraced", 9))
	{
		printInt(2);
	}
	else
		printInt(0);

	int lightUse = readInt("*LIGHT_USELIGHT");
	printInt(lightUse);

	readString("*LIGHT_SPOTSHAPE");

	char* lightColor = readRGB("*LIGHT_COLOR");
	printBytes(lightColor, 4);

	float lightIntensity = readFloat("*LIGHT_INTENS");
	printFloat(lightIntensity);

	float lightAspect = readFloat("*LIGHT_ASPECT");
	printFloat(lightAspect);

	printInt(0); printInt(0);

	float lightAttnStart = readFloat("*LIGHT_ATTNSTART");
	printFloat(lightAttnStart);

	float lightAttnEnd = readFloat("*LIGHT_ATTNEND");
	printFloat(lightAttnEnd);

	float lightTDist = readFloat("*LIGHT_TDIST");
	printFloat(lightTDist);

	fscanf(input, "*USE FAR ATTENUATION ");
	float lightUseFar = readFloat("=");
	printFloat(lightUseFar);

	closeBracket();

	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}

int readGeomobject()
{
	printInt(2);
	printInt(4);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);

	openBracket();

	char* objName = readString("*NODE_NAME");
	printString(objName);

	int offset = ftell(input);
	bracketize();
	char* nextName = (char*)malloc(sizeof(char)*32);
	fscanf(input, "%s", nextName);
	fseek(input, offset, 0);

	if (!strncmp(nextName, "*NODE_PARENT", 11))
	{
		char* objParent = readString("*NODE_PARENT");
		printString(objParent);
	}
	else
	{
		printInt(0);
	}

	int objShadeVerts = readInt("*NODE_SHADEVERTS");
	printBytes((char*)&objShadeVerts, 1);

	readTM();
	readGeoMesh();

	//closeBracket();

	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}

int readObjectList()
{
	printInt(18);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);

	readNothing("*OBJECT_LIST");

	openBracket();
	int objCount = readInt("*OBJECT_COUNT");
	printInt(objCount);
	int i;

	for(i = 0; i < objCount; i++)
	{
		bracketize();
		char* objectType = (char*)malloc(sizeof(char)*64);
		fscanf(input, "%s\n", objectType);

		if (!strncmp(objectType, "*GEOMOBJECT", 11))
		{
			readGeomobject();
		}
		else if (!strncmp(objectType, "*GMID_ATTACHMENTPT", 20))
		{
			readGMIDAttachmentPt();
		}
		else if (!strncmp(objectType, "*GMID_HAVOK_SIMOBJECT", 23))
		{
			readHavokSimobject();
		}
		else if (!strncmp(objectType, "*GMID_HAVOK_RBCOLLECTIOB", 23))
		{
			readHavokRBCollection();
		}
		else if (!strncmp(objectType, "*GMID_HAVOK_CONSTRAINTSOLVER", 28))
		{
			readHavokCSolver();
		}
		else if (!strncmp(objectType, "*LIGHT", 7))
		{
			readLight();
		}
		else
		{
			char* message = (char*)malloc(128);
			sprintf(message, "readObjectList(): Unknown object type %s!\n", objectType);
			error(message);
			exit(1);
			break;
		}
			
	}

	closeBracket();
	
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}