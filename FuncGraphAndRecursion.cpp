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

	strcpy(Files[0], "Source.cpp");
	strcpy(Files[1], "Source1.cpp");
	strcpy(Files[2], "Header.h");
	strcpy(Files[3], "Source2.cpp");
	AmountOfFiles = 4;
	strcpy(search->NameOfFunction[0], "func0");
	strcpy(search->NameOfFunction[1], "func1");
	strcpy(search->NameOfFunction[2], "func2");
	strcpy(search->NameOfFunction[3], "func3");
	strcpy(search->NameOfFunction[4], "func4");
	strcpy(search->NameOfFunction[5], "func5");
	strcpy(search->NameOfFunction[6], "func6");
	search->counter = 7;
	CounterEmpty = 7;
	AmountOfFunctions = 7;
	
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

	for (int i = 0; i < search->counter; ++i)
	{
		char Cycled[50][25] = { 0 };
		int CycledCount = 0;
		NODE* NewNode = NULL;
		if (search->FlagOfIncludingInTree[i])
		{
			for (int j = 0; j < CountRoots; ++j)
			{
				NewNode = BFS(search->NameOfFunction[i], MassOfRoots[j], 1);
				CleanFlags(MassOfRoots[j]);
				if (NewNode)
					break;
			}
		}
		else
		{
			NewNode = AddNode(0, search->NameOfFunction[i], MassOfRoots, CountRoots, Cycled, &CycledCount);
			search->FlagOfIncludingInTree[i] = 1;
			MassOfRoots[CountRoots] = NewNode;
			++CountRoots;
		}
		FILE* r;
		r = fopen(search->FileName[i], "r");
		char Buffs[200] = { 0 };
		int c = 0;
		while (c <= search->Positions[i])
		{
			fgets(Buffs, 200, r);
			++c;
		}
		int CounterOfBrackets = 1;
		while (1)
		{
			fgets(Buffs, 200, r);
			if (strchr(Buffs, '{')) ++CounterOfBrackets;
			if (strchr(Buffs, '}')) --CounterOfBrackets;
			if (!CounterOfBrackets) 
				break;
			for (int j = 0; j < AmountOfFunctions; ++j)
			{
				if (strstr(Buffs, search->NameOfFunction[j]))
				{
					if (!search->FlagOfIncludingInTree[j])
					{
						search->FlagOfIncludingInTree[j] = 1;
						NewNode = AddNode(NewNode, search->NameOfFunction[j], MassOfRoots, CountRoots, Cycled, &CycledCount);
					}
					else
					{
						for (int k = 0; k < CountRoots; ++k)
						{
							NODE* tempor = BFS(search->NameOfFunction[j], MassOfRoots[k], 1);
							CleanFlags(MassOfRoots[k]);
							if (tempor != NULL)
							{
								NewNode->children[NewNode->AmountOfChildren] = tempor;
								break;
							}
						} 
					}
					++NewNode->AmountOfChildren;
				}
			}
		}
		for (int j = 0; j < NewNode->AmountOfChildren; ++j)
		{
			if (ThisIsNotDescendant(NewNode, NewNode->children[j]->Name))
			{
				NewNode->children[j]->parent = NewNode;
			}
			else
			{
				strcpy(NewNode->BanListForBFS_DFS[NewNode->AmountOfBan], NewNode->children[j]->Name);
				++NewNode->AmountOfBan;
			}
			for (int k = 0; k < CountRoots; ++k)
			{
				int flag = ThisIsNotDescendant(NewNode, MassOfRoots[k]->Name);
				if (!strcmp(NewNode->children[j]->Name, MassOfRoots[k]->Name) && flag)
				{
					MassOfRoots[k] = NewNode;//подумать над этим
				}				
			}
		}
		MassOfNodes[i] = NewNode;
	}
	for (int i = 0; i < search->counter; ++i)
	{
		printf("%s: ", MassOfNodes[i]->Name);
		for (int j = 0; j < MassOfNodes[i]->AmountOfChildren; ++j )
		{
			printf("%s", MassOfNodes[i]->children[j]->Name);
			if (j != MassOfNodes[i]->AmountOfChildren - 1)
				printf(",");
		}
		printf("\n");
	}

	////Find Recursive Fucntions////
	char ListAlreadyPrinted[50][50] = { 0 };
	int CountList = 0;
	printf("\nRecursive functions:\n");
	for (int i = 0; i < CountRoots; ++i)
	{
		DFS(MassOfRoots[i], NULL, ListAlreadyPrinted, &CountList);
		CleanFlags(MassOfRoots[i]);
	}
	return 0;
	
}