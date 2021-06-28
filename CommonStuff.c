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

void showHelp()
{
	setColor2(2);
	printf("Команда \"\x1b[0mlist\x1b[33m\" - показать доступные файлы, \команда \"\x1b[0mexit\x1b[33m\" - выйти из программы.\n");
	setColor2(0);
}

void showFiles()
{
	printf("Доступеые файлы:\n");
	setColor2(3);
	char command[50] = "dir /b *.cpp";
	system(command);
	setColor2(0);
}

void unluckee()
{
	setColor2(1);
	printf("Unluckee, please try again.\n");
	setColor2(0);
}

void exitApp()
{
	setColor(2);
	printf("Вы вышли из программы.");
	setColor(0);
	exit(0);
}
