#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define error(x) Error("An error has occured!", (x))
#define closeBracket() CloseBracket(__FUNCTION__)
extern FILE* input;
extern FILE* output;

int indentation = 0; 

int Error(const char* title, const char* message)
{
	MessageBox(NULL, message, title, MB_ICONWARNING);
	char* temp = (char*)malloc(sizeof(char)*64);
	fread(temp, 1, 64, input);
	temp[63] = '\x00';
	MessageBox(NULL, temp, "Input File Dump", MB_ICONWARNING);
	exit(1);
	return 0;
}

int YesNoError(const char* title, const char* message)
{
	int result =  MessageBox(NULL, title, message, MB_YESNO);
	if (result == IDNO)
	{
		Error("Compilation aborted.", "Compilation unsuccessful!");
		return 1;
	}
	else return 0;
}

int bracketize()
{
	int i;
	for (i = 0; i < indentation; i++)
	{
		fscanf(input, "\t");
	}
	return 0;
}

int readInt(char* name)
{
	bracketize();
	char* nameInFile = (char*)malloc(sizeof(char)*128);
	int output;

	fscanf(input, "%s\t%i\n", nameInFile, &output);

	if (strncmp(name, nameInFile, strlen(name)))
	{
		char* message = (char*)malloc(128);
		sprintf(message, "readInt(): Expected %s, but found %s instead!\n Ignore and continue?", name, nameInFile);
		YesNoError(message, "An error has occured!");
	}

	return output;
	
}

char* readString(char* name)
{
	bracketize();
	char* nameInFile = (char*)malloc(sizeof(char)*128);
	char* output = (char*)malloc(sizeof(char)*512);

	fscanf(input, "%s	", nameInFile);	

	if (strncmp(name, nameInFile, strlen(name)))
	{
		char* message = (char*)malloc(128);
		sprintf(message, "readString(): Expected %s, but found %s instead!\n Ignore and continue?", name, nameInFile);
		YesNoError(message, "An error has occured!");
	}

	char in;
	int i = 0;
	while(1)
	{
		in = fgetc(input);

		if(in == '\n')
			break;

		output[i] = in;
		i++;
	}
	output[i] = '\x00';

	return output;
	
}

char* readRGB(char* name)
{
	bracketize();
	char* nameInFile = (char*)malloc(sizeof(char)*128);
	char* RGB = (char*)malloc(sizeof(char)*7);
	char* out = (char*)malloc(sizeof(char)*4);

	fscanf(input, "%s\t0x%s", nameInFile, RGB);

	if (strncmp(name, nameInFile, strlen(name)))
	{
		char* message = (char*)malloc(128);
		sprintf(message, "readRGB(): Expected %s, but found %s instead!\n Ignroe and continue?", name, nameInFile);
		YesNoError(message, "An error has occured!");
	}

	int i;
	int k = 0;
	for (i = 0; i < 5; i+=2)
	{
		int resultingByte;
		switch (RGB[i])
		{
		case 'F': case 'E': case 'D': case 'C': case 'B': case 'A':
			resultingByte = (10 + RGB[i] - 'A') * 16;
			break;
		case 'f': case 'e': case 'd': case 'c': case 'b': case 'a':
			resultingByte = (10 + RGB[i] - 'a') * 16;
			break;
		default:
			resultingByte = (RGB[i] - '0') * 16;
			break;
		}


		switch (RGB[i+1])
		{
		case 'F': case 'E': case 'D': case 'C': case 'B': case 'A':
			resultingByte += (10 + RGB[i+1] - 'A');
			break;
		case 'f': case 'e': case 'd': case 'c': case 'b': case 'a':
			resultingByte += (10 + RGB[i+1] - 'a');
			break;
		default:
			resultingByte += (RGB[i+1] - '0');
			break;
		}

		out[k] = resultingByte;
		k++;
	}
	out[3] = '\x00';
	return out;
}

float readFloat(char* name)
{
	bracketize();
	char* nameInFile = (char*)malloc(sizeof(char)*128);
	float output;

	fscanf(input, "%s\t%f\n", nameInFile, &output);

	if (strncmp(name, nameInFile, strlen(name)))
	{
		char* message = (char*)malloc(128);
		sprintf(message, "readFloat(): Expected %s, but found %s instead!\nIgnore and continue?", name, nameInFile);
		YesNoError(message, "An error has occured!");
	}

	return output;
}

int readNothing(char* name)
{
	bracketize();
	char* nameInFile = (char*)malloc(sizeof(char)*128);

	fscanf(input, "%s\n", nameInFile);

	if (strncmp(name, nameInFile, strlen(name)))
	{
		char* message = (char*)malloc(128);
		sprintf(message, "readNothing(): Expected %s, but found %s instead!\n Ignore and continue?", name, nameInFile);
		YesNoError(message, "An error has occured!");
	}
	return 0;
}

int openBracket()
{
	char* whatIHaveRead = (char*)malloc(sizeof(char)*32);
	bracketize();
	fscanf(input, "%s\n", whatIHaveRead);
	if (strncmp(whatIHaveRead, "{", 1))
	{
		char* message = (char*)malloc(128);
		sprintf(message, "openBracket(): Expected opening bracket, found %s instead!\n", whatIHaveRead);
		error(message);
		exit(1);
	}
	indentation++;
	return 0;
}

int CloseBracket(const char* caller)
{
	char* whatIHaveRead = (char*)malloc(sizeof(char)*32);
	indentation--;
	bracketize();
	fscanf(input, "%s\n", whatIHaveRead);	
	if (strncmp(whatIHaveRead, "}", 1))
	{
		char* message = (char*)malloc(128);
		sprintf(message, "closeBracket(): Expected closing bracket, found %s instead!\n", whatIHaveRead);
		Error(caller, message);
		exit(1);
	}
	return 0;
}

int addToString(char* destination, char* source, int num, int start)
{
	int i;
	for (i = 0; i < num; i++)
	{
		destination[start + i] = source[i];
	}
	return 0;
}