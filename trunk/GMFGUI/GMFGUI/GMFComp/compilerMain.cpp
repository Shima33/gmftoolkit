#include "stdafx.h"

#include "../mystdio.h"
#include <stdlib.h>
#include <windows.h>

#include "headerFunctions.h"
#include "sceneFunctions.h"
#include "materialFunctions.h"
#include "objectFunctions.h"
#include "asciiFunctions.h"

FILE* input;
extern FILE* output;
FILE* control;


/*int mainlol(char *openPath, char *savePath)
{
	if (openPath == 0)
	{
		return 1;
    }
	if (savePath == 0)
	{
		return 1;
    }
    input = fopen(openPath, "r");
	output = fopen(savePath, "wb");

	readHeader();
	readScene();	
	readNothing("*MATERIAL_LIST");
	readMaterialList();
	readObjectList();

	fclose(input);
	fclose(output);
	MessageBox(NULL, L"The file has been compiled successfully!", L"Compiling successful! :)", MB_ICONASTERISK);

	return 0;
}*/

int compile(HWND parent, const LPWSTR sourcePath, const LPWSTR outputPath)
{
	_wfopen_s(&input, sourcePath, L"rb");
	_wfopen_s(&output, outputPath, L"w");
	
	if (input == 0 || output == 0)
	{
		myprintf("No file selected.");
		return 1;
	}

	readHeader();
	readScene();	
	readNothing("*MATERIAL_LIST");
	readMaterialList();
	readObjectList();

	MessageBox(parent, L"The file has been compiled successfully!", L"Compiling successful! :)", MB_ICONASTERISK);

	return 0;
}