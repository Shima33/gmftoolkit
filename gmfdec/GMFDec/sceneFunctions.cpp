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

void readSceneInfo()
{
	
	getBytes(8);
	unsigned int sceneLength = getInteger();
	char* sceneName = getString();
	printf("Decompiling scene %s...\n", sceneName);
	unsigned int sceneFirstFrame = getInteger();
	unsigned int sceneLastFrame = getInteger();
	unsigned int sceneFrameSpeed = getInteger();
	unsigned int sceneTicksPerFrame = getInteger();
	char* sceneBackgroundStatic = getRGB();
	char* sceneAmbientStatic = getRGB();
	
	fprintf(output, "*SCENE\n");
	fprintf(output, "{\n");
	fprintf(output, "\t*SCENE_FILENAME\t%s\n", sceneName);
	fprintf(output, "\t*SCENE_FIRSTFRAME\t%i\n", sceneFirstFrame);
	fprintf(output, "\t*SCENE_LASTFRAME\t%i\n", sceneLastFrame);
	fprintf(output, "\t*SCENE_FRAMESPEED\t%i\n", sceneFrameSpeed);
	fprintf(output, "\t*SCENE_TICKSPERFRAME\t%i\n", sceneTicksPerFrame);
	fprintf(output, "\t*SCENE_BACKGROUND_STATIC\t%s\n", sceneBackgroundStatic);
	fprintf(output, "\t*SCENE_AMBIENT_STATIC\t%s\n", sceneAmbientStatic);
	fprintf(output, "}\n");
	//free(sceneName);
}