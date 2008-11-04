// Open Source C GMF Compiler
// Copyright (c) 2008 Sergiusz Bazanski
// http://www.q3k.org/
// GPL License
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>


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

char* openGMFFile()
{
      OPENFILENAME ofn;
      TCHAR szFile[MAX_PATH] = L"";
      
      ZeroMemory(&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = NULL;
      ofn.lpstrFile = (LPWSTR)szFile;
      ofn.lpstrFile[0] = '\0';
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrFilter = L"Binary GMF File (*.gmf)\0*.gmf\0\0";
      ofn.nFilterIndex = 1;
      ofn.lpstrFileTitle = NULL;
      ofn.nMaxFileTitle = 0;
      ofn.lpstrInitialDir = NULL;
      ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	  ofn.lpstrTitle = L"Select Binary GMF File to decompile:";

      if (GetOpenFileName(&ofn))
      {
	     char* resultString = (char*)malloc(MAX_PATH);
		 WideCharToMultiByte(CP_ACP, 0, szFile, wcslen(szFile)+1, resultString , MAX_PATH, NULL, NULL);
         return resultString;
      }
      
      return "Error file choosen doesn't exist or no file was chosen";
}

char* saveGMFFile()
{
      OPENFILENAME ofn;
      TCHAR szFile[MAX_PATH] = L"";
      
      ZeroMemory(&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = NULL;
      ofn.lpstrFile = (LPWSTR)szFile;
      ofn.lpstrFile[0] = '\0';
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrFilter = L"ASCII GMF File (*.gmf)\0*.gmf\0\0";
      ofn.nFilterIndex = 1;
      ofn.lpstrFileTitle = NULL;
      ofn.nMaxFileTitle = 0;
      ofn.lpstrInitialDir = NULL;
      ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	  ofn.lpstrTitle = L"Select ASCII GMF File destination:";


      if (GetSaveFileName(&ofn))
      {
	     char* resultString = (char*)malloc(MAX_PATH);
		 WideCharToMultiByte(CP_ACP, 0, szFile, wcslen(szFile)+1, resultString , MAX_PATH, NULL, NULL);
         return resultString;
      }
      
      return "Error file choosen doesn't exist or no file was chosen";
}

void tab(int num)
{
	int i;
	for (i = 0; i < num; i++)
		fprintf(output, "\t");
}

void tabList(int num)
{
	int i;
	for (i = 0; i < num-1; i++)
		printf("\t");
}

void debugHex(char* in, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%02X ", in[i]);
	}
	printf("\n");
}