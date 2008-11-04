#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "headerFunctions.h"
#include "sceneFunctions.h"
#include "materialFunctions.h"
#include "objectFunctions.h"
#include "asciiFunctions.h"

FILE* input;
FILE* output;
FILE* control;

char* openGMFFile()
{
      OPENFILENAME ofn;
		char* szFile = (char*)malloc(MAX_PATH);
      
      ZeroMemory(&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = NULL;
      ofn.lpstrFile = szFile;
      ofn.lpstrFile[0] = '\0';
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrFilter = "ASCII GMF File (*.gmf)\0*.gmf\0\0";
      ofn.nFilterIndex = 1;
      ofn.lpstrFileTitle = NULL;
      ofn.nMaxFileTitle = 0;
      ofn.lpstrInitialDir = NULL;
      ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	  ofn.lpstrTitle = "Select ASCII GMF File to compile:";

      if (GetOpenFileName(&ofn))
      {
		return szFile;
      }
      
      return 0;
}

char* saveGMFFile()
{
      OPENFILENAME ofn;
      char* szFile = (char*)malloc(MAX_PATH);
      
      ZeroMemory(&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = NULL;
      ofn.lpstrFile = szFile;
      ofn.lpstrFile[0] = '\0';
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrFilter = "Binary GMF File (*.gmf)\0*.gmf\0\0";
      ofn.nFilterIndex = 1;
      ofn.lpstrFileTitle = NULL;
      ofn.nMaxFileTitle = 0;
      ofn.lpstrInitialDir = NULL;
      ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	  ofn.lpstrTitle = "Select Binary GMF File destination:";


      if (GetSaveFileName(&ofn))
      {
         return szFile;
      }
      
      return 0;
}

int main()
{
	char* openPath = openGMFFile();
	if (openPath == 0)
		error("No input file selected!");
	char* savePath = saveGMFFile();
	if (savePath == 0)
		error("No output file selected!");
	input = fopen(openPath, "r");
	output = fopen(savePath, "wb");

	readHeader();
	readScene();	
	readNothing("*MATERIAL_LIST");
	readMaterialList();
	readObjectList();

	fclose(input);
	fclose(output);
	MessageBox(NULL, "The file has been compiled successfully!", "Compiling successful! :)", MB_ICONASTERISK);
}