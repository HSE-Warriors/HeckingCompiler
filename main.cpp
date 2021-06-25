#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "CommonStuff.h"
//#include Lexa
//#include Den
#include "KoshkinStuff.h"
//#include Ribs


//Common I/O
char Files[50][40] = { 0 }; //Для Лёши, чтобы обмениваться с остальными
int AmountOfFiles = 1; ///Для Лёши, чтобы передать Дену
char Functions[50][50] = { 0 }; int AmountOfFunctions = 0; //Для Саши, чтобы передать Дену

int main()
{
	setLocale();
	enableScrolling();

	//Функции Лёши

	//Функции Саши
	KoshkinDoStuff();

	//Функции Дена

	//функции Гоши

	//вывод статистики
	//printThings();

	return 0;
}
