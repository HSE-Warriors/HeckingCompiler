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
int AmountOfFunctions = 0;//Для Дена
char InfiniteLoopSrtings[50][100] = { 0 };
int AmountOfInfiniteMessages = 0;

NODE* MassOfNodes[50] = { 0 };
NODE* MassOfRoots[50] = { 0 };
int CountRoots = 0;

int main()
{
	setLocale();
	enableScrolling();

	//Функции Лёши
	main_correction(Files, &AmountOfFiles);

	//Функции Саши
	KoshkinDoStuff();

	//Функции Дена
	FunctionsGraph(Files, AmountOfFiles, Functions, AmountOfFunctions, MassOfNodes, MassOfRoots, &CountRoots);
	InfiniteLoop(Files, AmountOfFiles, InfiniteLoopSrtings, &AmountOfInfiniteMessages);

	//функции Гоши
	FILE* FILE_IN = NULL;
	char filelist[50][25] = { 0 };
	for (int i = 0; i < AmountOfFiles; ++i)
		strcpy(filelist[i], Files[i]);
	CountMaxLoopDepth(FILE_IN, filelist, AmountOfFiles, &maxCycled);
	EnlistMutuallyCalledHeaders(FILE_IN, filelist, AmountOfFiles, callPairList, mutualCalls, &mutualCallsCnt);
	DetectCustomFunction(FILE_IN, filelist, AmountOfFiles, customFuncList, &customFuncCnt);
	DetectCustomTypes(FILE_IN, filelist, AmountOfFiles, customTypesList, &customTypesCnt);

	//вывод статистики
	printThings();
	ending();

	return 0;
}
