#include <windows.h>

#define error(x) Error("An error has occured!", (x))
#define closeBracket() CloseBracket(__FUNCTION__)

int Error(const char* title, const char* message);
int YesNoError(const char* title, const char* message);
int bracketize();
int readInt(char* name);
char* readString(char* name);
char* readRGB(char* name);
float readFloat(char* name);
int readNothing(char* name);
int openBracket();
int CloseBracket(const char* caller);
int addToString(char* destination, char* source, int num, int start);
