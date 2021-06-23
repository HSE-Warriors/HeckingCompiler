#include "CommonStuff.h"
#include <locale.h>		//для setlocale

void setLocale()
{
	setColor(0);
	char* locale = setlocale(LC_ALL, "");
}

void enableScrolling()
{
	COORD newSize = { 10, 9999 };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), newSize);
}

void setColor(int color)
{
	if (color == 1)
	{
		printf(COLOR_RED);
	}

	if (color == 2)
	{
		printf(COLOR_YELLOW);
	}

	if (color == 3)
	{
		printf(COLOR_GREEN);
	}

	if (color == 4)
	{
		printf(COLOR_CYAN);
	}

	if (color == 5)
	{
		printf(COLOR_MAGENTA);
	}

	if (color == 0)
	{
		printf(COLOR_RESET);
	}
}
