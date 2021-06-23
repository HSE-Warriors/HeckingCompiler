#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Functions.h"

typedef struct coord
{
	char NameOfFunction[50][50];
	char FileName[50][50];
	int Positions[50];
	int NumberOfString;
	int counter;
	int FlagOfIncludingInTree[50];
}COORD;

COORD* Init()
{
	COORD* t = (COORD*)malloc(sizeof(COORD));
	if (t)
	{
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				t->NameOfFunction[i][j] = { 0 };
				t->FileName[i][j] = { 0 };
			}
			t->FlagOfIncludingInTree[i] = 0;
			t->Positions[i] = { 0 };
		}
		t->NumberOfString = 1;
		t->counter = 0;
	}
	return t;
}

int main()
{
	char Functions[50][50] = { 0 };//
	int AmountOfFunctions = 0;//
	int IndexOfEmptyFunc[50] = { 0 };
	int CounterEmpty = 0;
	char Files[50][20] = { 0 };//
	int AmountOfFiles = 0;//
	char Buffers[200] = { 0 };
	NODE* MassOfRoots[50] = { 0 };
	int CountRoots = 0;
	NODE* MassOfNodes[50] = { 0 };
	
	COORD* search = Init();
	
	if (CounterEmpty)
	{
		int ExitCounter = CounterEmpty;
		for (int i = 0; i < AmountOfFiles; ++i)
		{
			if (ExitCounter == 0)
				break;
			FILE* r;
			int counters = 0;
			if (strstr(Files[i], ".cpp"))
			{
				r = fopen(Files[i], "r");
				while (!feof(r))
				{
					++counters;
					for (int j = 0; j < CounterEmpty; ++j)
					{
						if (IndexOfEmptyFunc[j] != -1)
						{
							char* buff = search->NameOfFunction[j];
							buff[strlen(buff)] = '(';
							if (strstr(Buffers, buff) && !strchr(Buffers, ';'))
							{
								strcpy(search->FileName[j], Files[i]);
								search->Positions[j] = counters;
								--ExitCounter;
								IndexOfEmptyFunc[j] = -1;
								buff[strlen(buff) - 1] = 0;
								break;
							}
							buff[strlen(buff) - 1] = 0;
							//int a = 0;
						}
					}
				}
			}
		}
	}
}