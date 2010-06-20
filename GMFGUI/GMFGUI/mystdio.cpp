#include "stdafx.h"

extern CListBox* theLog;

void myprintf(char* format, ...)
{
	char buffer[512];
	WCHAR UnicodeBuffer[512];

	va_list args;
	va_start(args, format);

	vsprintf_s(buffer, 512, format, args);
	
	MultiByteToWideChar(CP_ACP, 0, buffer, -1, UnicodeBuffer, 512);

	theLog->AddString(UnicodeBuffer);
}