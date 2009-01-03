#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "byteFunctions.h"
#include "asciiFunctions.h"

extern FILE* input;
extern FILE* output;

int readTexture()
{
	printInt(15);
	printInt(4);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);


	readNothing("*TEXTURE");
	openBracket();
	char* mapName = readString("*MAP_NAME");
	char* mapClass = readString("*MAP_CLASS");
	char* mapBitmap = readString("*BITMAP");
	float mapAmount = readFloat("*MAP_AMOUNT");	
	char* mapStyle = (char*)malloc(sizeof(char)*32);
	bracketize(); fscanf(input, "%s\n", mapStyle);
	char* mapType = readString("*MAP_TYPE");
	float mapUO = readFloat("*UVW_U_OFFSET");
	float mapVO = readFloat("*UVW_V_OFFSET");
	float mapUT = readFloat("*UVW_U_TILING");
	float mapVT = readFloat("*UVW_V_TILING");
	float mapAngle = readFloat("*UVW_ANGLE");
	float mapBlur = readFloat("*UVW_BLUR");
	float mapBlurOffset = readFloat("*UVW_BLUR_OFFSET");
	float mapNoiseAmount = readFloat("*UVW_NOISE_AMT");
	float mapNoiseSize = readFloat("*UVW_NOISE_SIZE");
	int mapNoiseLevel = readInt("*UVW_NOISE_LEVEL");
	float mapNoisePhase = readFloat("*UVW_NOISE_PHASE");
	char* mapBitmapFilter = readString("*BITMAP_FILTER");
	int mapBitmapMapChannel = readInt("*BITMAP_MAP_CHANNEL");

	printString(mapName);
	printString(mapClass);
	printString(mapBitmap);
	printFloat(mapAmount);
	if (!strncmp(mapStyle, "*MAP_DIFFUSE", 12))
		printInt(1);
	else if (!strncmp(mapStyle, "*MAP_SELFILLUM", 14))
		printInt(5);
	else if (!strncmp(mapStyle, "*MAP_OPACITY", 12))
		printInt(6);
	else
		printInt(1);

	if (!strncmp(mapType, "Screen", 6))
		printInt(4);
	else
		printInt(0);
	printFloat(mapUO);
	printFloat(mapVO);
	printFloat(mapUT);
	printFloat(mapVT);
	printFloat(mapAngle);
	printFloat(mapBlur);
	printFloat(mapBlurOffset);
	printFloat(mapNoiseAmount);
	printFloat(mapNoiseSize);
	printInt(mapNoiseLevel);
	printFloat(mapNoisePhase);
	if (!strncmp(mapBitmapFilter, "SAT", 3))
	{
		printInt(0);
		printInt(0);
		printInt(1);
	}
	else
	{
		printInt(0);
		printInt(0);
		printInt(0);
	}
	printInt(mapBitmapMapChannel);
	printInt(0);

	closeBracket();


	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}

int readTextureList()
{
	printInt(14);
	printInt(2);

	printBytes("\xFF\xFF\xFF\xFF", 4);

	int beginningOffset = ftell(output);
	
	openBracket();

	int numTextures = readInt("*TEXTURE_COUNT");

	printInt(numTextures);

	int i;
	for (i = 0; i < numTextures; i++)
	{
		readTexture();
	}

	closeBracket();

	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;

	fseek(output, beginningOffset - 4, 0);
	
	printInt(size);

	fseek(output, endOffset, 0);

	return numTextures;
}

int readMaterialList();

int readMaterial()
{
	printInt(8);
	printInt(2);

	//Placeholder for length
	printBytes("\xFF\xFF\xFF\xFF", 4);

	int beginningOffset = ftell(output);

	readNothing("*MATERIAL");

	openBracket();

	int matRefNo = readInt("*MATERIAL_REF_NO");
	char* matName = readString("*MATERIAL_NAME");
	char* matClass = readString("*MATERIAL_CLASS");
	char* matAmbient = readRGB("*MATERIAL_AMBIENT");
	char* matDiffuse = readRGB("*MATERIAL_DIFFUSE");
	char* matSpecular = readRGB("*MATERIAL_SPECULAR");
	float matShine = readFloat("*MATERIAL_SHINE");
	float matShineStrength = readFloat("*MATERIAL_SHINESTRENGTH");
	float matTransparency = readFloat("*MATERIAL_TRANSPARENCY");
	/*float matTransparency;
	bracketize();
	char* buffer = (char*) malloc(sizeof(char)* 24);
	int offset = ftell(input);*/
	float matWireSize = readFloat("*MATERIAL_WIRESIZE");
	char* matShading = readString("*MATERIAL_SHADING");
	float matXPFallof = readFloat("*MATERIAL_XP_FALLOF");
	float matSelfIllum = readFloat("*MATERIAL_SELFILLUM");
	char* matFallof = readString("*MATERIAL_FALLOF");
	char* matXPType = readString("*MATERIAL_XP_TYPE");

	printInt(matRefNo);
	printString(matName);
	printString(matClass);
	printBytes(matAmbient, 4);
	printBytes(matDiffuse, 4);
	printBytes(matSpecular, 4);
	printFloat(matShine);
	printFloat(matShineStrength);
	printFloat(matTransparency);
	printFloat(matWireSize);

	if(!strncmp(matShading, "Blinn", 5))
		printInt(12);
	else
		printInt(0);

	printFloat(matXPFallof);
	printFloat(matSelfIllum);

	if(!strncmp(matFallof, "In", 2))
		printInt(1);
	else
		printInt(0);

	if(!strncmp(matXPType, "Filter", 6))
		printInt(1);
	else
		printInt(0);

	char* nextType = (char*)malloc(sizeof(char)*32);

	int numMaterials = 0;
	int numTextures = 0;

	int tParsed = 0;
	int mParsed = 0;

	while(1)
	{
		int currentOffset = ftell(input);
		fscanf(input, "%s\n", nextType);
		if(!strncmp(nextType, "*TEXTURE_LIST", 14) && tParsed == 0)
		{
			//Placeholder for number of textures
			int numTexturesOffset = ftell(output);
			printBytes("\xDE\xAD\xBE\xEF", 4);

			numTextures += readTextureList();

			int afterTextureList = ftell(output);
			fseek(output, numTexturesOffset, 0);
			printInt(numTextures);
			fseek(output, afterTextureList, 0);
			mParsed = 1;
		}
		else if(tParsed == 0)
		{
			tParsed = 1;
			printInt(0);
		}

		if(!strncmp(nextType, "*MATERIAL_LIST", 14) && mParsed == 0)
		{
			//Placeholder for number of textures
			int numMaterialsOffset = ftell(output);
			printBytes("\xDE\xAD\xBE\xEF", 4);

			numMaterials += readMaterialList();

			int afterMaterialList = ftell(output);
			fseek(output, numMaterialsOffset, 0);
			printInt(numMaterials);
			fseek(output, afterMaterialList, 0);
			mParsed = 1;
		}
		else if (mParsed == 0)
		{
			mParsed = 1;
			printInt(0);
		}		
		
		if(!strncmp(nextType, "}", 1))
		{
			fseek(input, currentOffset, 0);
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

int readMaterialList()
{
	printInt(7);
	printInt(2);

	//Placeholder for length
	printBytes("\xFF\xFF\xFF\xFF", 4);

	int beginningOffset = ftell(output);


	openBracket();

	int matCount = readInt("*MATERIAL_COUNT");

	printInt(matCount);

	int  i;
	for (i = 0; i < matCount; i++)
	{
		readMaterial();
	}

	closeBracket();

	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;

	fseek(output, beginningOffset - 4, 0);
	
	printInt(size);

	fseek(output, endOffset, 0);

	return matCount;
}
