#include "cGMF.h"

cGMF::cGMF(string FilePath, int Format, int RW)
{
	cPath = FilePath;
	if (RW == 0)
		cFile.open(cPath.c_str(), ios_base::in | ios_base::binary | ios_base::ate);
	else
		cFile.open(cPath.c_str(), ios_base::out | ios_base::binary | ios_base::ate);

	cFile.seekg(0, ios::beg);
	
	if(Format == 0)
	{
		char* header = getBytes(3);
		if (strncmp(header, "GMI", 3))
		{
			throw("Not a valid GMF file!");
		}
		version = getInteger();

		int id, flag, size;
		int debugcycles = 0;
		while(!cFile.eof())
		{
			if (debugcycles >= 4)
				break;

			id = getInteger();
			flag = getInteger();
			size = getInteger();
			readMain(id, flag, size);
			
			debugcycles++;
		}
	}
}

char* cGMF::getBytes(int NumberOfBytes)
{
	char* toBeReturned = (char*)malloc(sizeof(char) * NumberOfBytes);
	cFile.read(toBeReturned, NumberOfBytes);
	return toBeReturned;
}

char* cGMF::getBytesNF(int NumberOfBytes)
{
	char* toBeReturned = (char*)malloc(sizeof(char) * NumberOfBytes);
	int offset = (int) cFile.tellg();
	cFile.read(toBeReturned, NumberOfBytes);
	cFile.seekg(offset, ios::beg);
	return toBeReturned;
}

int cGMF::getInteger()
{
	char bytes[4];
	cFile.read(bytes, 4);
	return *(int*)bytes;
}

float cGMF::getFloat()
{
	char bytes[4];
	cFile.read(bytes, 4);
	return *(float*)bytes;
}

char* cGMF::getString()
{
	int size = getInteger();
	if (size == 0)
	{
		char* nullstring = "(null)";
		return nullstring;	
	}
	char* data = (char*)malloc(sizeof(char) * size);
	cFile.read(data, size);
	return data;
}
