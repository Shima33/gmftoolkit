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

void readTexture(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	char* mapName = getString();
	printf("Decompiling texture %s...\n", mapName);
	char* mapClass = getString();
	char* mapBitmap = getString();
	float mapAmount = getFloat();
	
	char* mapStyle = (char*)malloc(15);
	switch (getInteger())
	{
		case 1:
			strncpy(mapStyle, "*MAP_DIFFUSE", 15);
			break;
		case 5:
			strncpy(mapStyle, "*MAP_SELFILLUM", 15);
			break;
		case 6:
			strncpy(mapStyle, "*MAP_OPACITY", 15);
			break;
		default:
			strncpy(mapStyle, "*MAP_DIFFUSE", 15);
			break;
	}
	
	char* mapType = (char*)malloc(7);
	if (getInteger() == 4)
		strncpy(mapType, "Screen", 7);
	else
		strncpy(mapType, "Other", 7);
		
	float mapUO = getFloat();
	float mapVO = getFloat();
	float mapUT = getFloat();
	float mapVT = getFloat();
	float mapUVWAngle = getFloat();
	float mapUVWBlur = getFloat();
	float mapUVWBlurOffset = getFloat();
	float mapUVWNoiseAmt = getFloat();
	float mapUVWNoiseSize = getFloat();
	int mapUVWNoiseLevel = getInteger();
	float mapUVWNoisePhase = getFloat();
	
	getBytes(8);
	
	char* mapBitmapFilter = (char*)malloc(10);
	if (getInteger() == 0)
		strncpy(mapBitmapFilter, "Pyramidal", 10);
	else
		strncpy(mapBitmapFilter, "SAT", 10);
		
	int mapBitmapChannel = getInteger();
	getBytes(4);
	
	tab(preTabNum); fprintf(output, "*TEXTURE\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*MAP_NAME\t%s\n", mapName);
	tab(preTabNum+1); fprintf(output, "*MAP_CLASS\t%s\n", mapClass);
	tab(preTabNum+1); fprintf(output, "*BITMAP\t%s\n", mapBitmap);
	tab(preTabNum+1); fprintf(output, "*MAP_AMOUNT\t%f\n", mapAmount);
	tab(preTabNum+1); fprintf(output, "%s\n", mapStyle);
	tab(preTabNum+1); fprintf(output, "*MAP_TYPE\t%s\n", mapType);
	tab(preTabNum+1); fprintf(output, "*UVW_U_OFFSET\t%f\n", mapUO);
	tab(preTabNum+1); fprintf(output, "*UVW_V_OFFSET\t%f\n", mapVO);
	tab(preTabNum+1); fprintf(output, "*UVW_U_TILING\t%f\n", mapUT);
	tab(preTabNum+1); fprintf(output, "*UVW_V_TILING\t%f\n", mapVT);
	tab(preTabNum+1); fprintf(output, "*UVW_ANGLE\t%f\n", mapUVWAngle);
	tab(preTabNum+1); fprintf(output, "*UVW_BLUR\t%f\n", mapUVWBlur);
	tab(preTabNum+1); fprintf(output, "*UVW_BLUR_OFFSET\t%f\n", mapUVWBlurOffset);
	tab(preTabNum+1); fprintf(output, "*UVW_NOISE_AMT\t%f\n", mapUVWNoiseAmt);
	tab(preTabNum+1); fprintf(output, "*UVW_NOISE_SIZE\t%f\n", mapUVWNoiseSize);
	tab(preTabNum+1); fprintf(output, "*UVW_NOISE_LEVEL\t%i\n", mapUVWNoiseLevel);
	tab(preTabNum+1); fprintf(output, "*UVW_NOISE_PHASE\t%f\n", mapUVWNoisePhase);
	tab(preTabNum+1); fprintf(output, "*BITMAP_FILTER\t%s\n", mapBitmapFilter);
	tab(preTabNum+1); fprintf(output, "*BITMAP_MAP_CHANNEL\t%i\n", mapBitmapChannel);
	tab(preTabNum); fprintf(output, "}\n");
	//free(mapName);
	//free(mapClass);
	//free(mapBitmap);
	//free(mapStyle);
	//free(mapType);
	//free(mapBitmapFilter);
}

void readTextureList(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	int totalCount = getInteger();
	printf("Decompiling %i texture(s)...\n", totalCount);
	
	tab(preTabNum); fprintf(output, "*TEXTURE_LIST\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*TEXTURE_COUNT\t%i\n", totalCount);
	
	int i;
	for (i =0; i < totalCount; i++)
	{
		readTexture(preTabNum+1);
	}
	
	tab(preTabNum); fprintf(output, "}\n");
}

void readMaterialList(int preTabNum);

void readMaterial(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	int matRef = getInteger();
	char* matName = getString();
	printf("Decompiling material %s...\n", matName);
	char* matClass = getString();
	char* matAmbient = getRGB();
	char* matDiffuse = getRGB();
	char* matSpecular = getRGB();
	float matShine = getFloat();
	float matShineStrength = getFloat();
	float matTransparency = getFloat();
	float matWiresize = getFloat();
	
	char* matShading = (char*) malloc(6);
	if (getInteger() == 12)
		strncpy(matShading, "Blinn", 6);
	else
		strncpy(matShading, "Other", 6);
		
	float matXPFallof = getFloat();
	float matSelfillum = getFloat();
	
	char* matFallof = (char*) malloc(6);
	if (getInteger() == 1)
		strncpy(matFallof, "In", 6);
	else
		strncpy(matFallof, "Other", 6);
	
	char* matXPType = (char*) malloc(7);
	if (getInteger() == 1)
		strncpy(matXPType, "Filter", 7);
	else
		strncpy(matXPType, "Other", 7);
	
	tab(preTabNum); fprintf(output, "*MATERIAL\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*MATERIAL_REF_NO\t%i\n", matRef);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_NAME\t%s\n", matName);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_CLASS\t%s\n", matClass);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_AMBIENT\t%s\n", matAmbient);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_DIFFUSE\t%s\n", matDiffuse);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_SPECULAR\t%s\n", matSpecular);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_SHINE\t%f\n", matShine);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_SHINESTRENGTH\t%f\n", matShineStrength);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_TRANSPARENCY\t%f\n", matTransparency);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_WIRESIZE\t%f\n", matWiresize);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_SHADING\t%s\n", matShading);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_XP_FALLOFF\t%f\n", matXPFallof);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_SELFILLUM\t%f\n", matSelfillum);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_FALLOFF\t%s\n", matFallof);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_XP_TYPE\t%s\n", matXPType);
	
	int textNum = getInteger();
	if (textNum > 0)
		readTextureList(preTabNum + 1);
		
	int matNum = getInteger();
	if (matNum > 0)
		readMaterialList(preTabNum + 1);
		
	tab(preTabNum); fprintf(output, "}\n");
	//free(matName);
	//free(matClass);
}

void readMaterialList(int preTabNum)
{
	
	getBytes(8);
	int matLength = getInteger();
	int matNumber = getInteger();

	printf("Decompiling %i material(s)...\n", matNumber);
	
	tab(preTabNum); fprintf(output, "*MATERIAL_LIST\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*MATERIAL_COUNT\t%i\n", matNumber);
	
	int i;
	for(i = 0; i < matNumber; i++)
	{
		readMaterial(preTabNum+1);
	}
	
	tab(preTabNum); fprintf(output, "}\n");
}
