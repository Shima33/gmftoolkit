#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "byteFunctions.h"
#include "asciiFunctions.h"

extern FILE* input;
extern FILE* output;

int readScene()
{
	printInt(1);
	printInt(2);

	//Placeholder for length
	printBytes("\xFF\xFF\xFF\xFF", 4);

	int beginningOffset = ftell(output);

	readNothing("*SCENE");

	openBracket();
	
	char* sceneFileName = readString("*SCENE_FILENAME");
	int sceneFirstFrame = readInt("*SCENE_FIRSTFRAME");
	int sceneLastFrame = readInt("*SCENE_LASTFRAME");
	int sceneFrameSpeed = readInt("*SCENE_FRAMESPEED");
	int sceneTicksPerFrame = readInt("*SCENE_TICKSPERFRAME");
	char* sceneBackgroundStatic = readRGB("*SCENE_BACKGROUND_STATIC");
	char* sceneAmbientStatic = readRGB("*SCENE_AMBIENT_STATIC");

	printString(sceneFileName);
	printInt(sceneFirstFrame);
	printInt(sceneLastFrame);
	printInt(sceneFrameSpeed);
	printInt(sceneTicksPerFrame);
	printBytes(sceneBackgroundStatic, 4);
	printBytes(sceneAmbientStatic, 4);

	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;

	fseek(output, beginningOffset - 4, 0);
	
	printInt(size);

	fseek(output, endOffset, 0);

	closeBracket();

	return 0;
}
