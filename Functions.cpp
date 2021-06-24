#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#define SIZE 50

NODE* Init()
{
	NODE* nodes = (NODE*)malloc(sizeof(NODE));
	if (nodes)
	{
		nodes->parent = 0;
		nodes->AmountOfChildren = 0;
		nodes->Visited_flag = 0;
		nodes->AmountOfBan = 0;
		for (int i = 0; i < 10; ++i)
		{
			nodes->children[i] = 0;
			for (int j = 0; j < 50; ++j)
			{
				nodes->BanListForBFS_DFS[i][j] = 0;
			}
		}
		for (int i = 0; i < 50; ++i)
		{
			nodes->Name[i] = 0;
		}
	}
	return nodes;
}

QUEUE* InitQ()
{
	QUEUE* elem = (QUEUE*)malloc(sizeof(QUEUE));
	if (elem)
	{
		elem->high = 0;
	}
	return elem;
}

QUEUE* AddToQ(NODE* node, QUEUE* find)
{
	find->Mas[find->high] = node;
	++find->high;
	return find;
}

NODE* PopFromQ(QUEUE* find)
{
	if (find->high)
	{
		NODE* temp = find->Mas[find->high-1];
		find->Mas[find->high] = NULL;
		--find->high;
		return temp;
	}
	else return NULL;
}

int ThisIsNotDescendant(NODE* node, char name[])
{
	while (node->parent != NULL)
	{
		return ThisIsNotDescendant(node->parent, name);
	}
	if (strcmp(name, node->Name))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

NODE* AddNode(NODE* node,char name[],NODE* MassOfRoots[], int Count, char Cycled[][25], int *CycledCount)
{
	if (node == NULL)
	{
		NODE* nodes = Init();
		strcpy(nodes->Name, name);
		return nodes;
	}
	else
	{
		for (int i = 0; i < Count; ++i)
		{
			if (!strcmp(name, MassOfRoots[i]->Name))
			{
				node->children[node->AmountOfChildren] = MassOfRoots[i];
				strcpy(Cycled[*CycledCount], MassOfRoots[i]->Name);
				++* CycledCount;
				return node;
			}
		}
		node->children[node->AmountOfChildren] = AddNode(node->children[node->AmountOfChildren], name, MassOfRoots, Count, Cycled, CycledCount);
		return node;
	}
}
int CheckNodeNotRote(char NameOfNode[50], NODE* MassOfRoots[50], int CountRoots)
{
	for (int i = 0; i < CountRoots; ++i)
	{
		if (!strcmp(NameOfNode, MassOfRoots[i]->Name))
			return 0;
	}
	return 1;
}
int NotBanned(NODE* amogus, NODE* sus, int flag)
{
	if (flag)
		return 1;
	for (int i = 0; i < amogus->AmountOfBan; ++i)
	{
		if (!strcmp(amogus->BanListForBFS_DFS[i], sus->Name))
			return 0;
	}
	return 1;
}

NODE* BFS(char* NameOfFunc, NODE* node, int FlagIgnoreBan)
{
	QUEUE* find = InitQ();
	find = AddToQ(node, find);
	while (find->high)
	{
		NODE* temp = PopFromQ(find);
		temp->Visited_flag = 1;
		if (!strcmp(temp->Name, NameOfFunc))
		{
			return temp;
		}
		else
		{
			for (int i = 0; i < temp->AmountOfChildren; ++i)
			{
				if (!temp->children[i]->Visited_flag && NotBanned(temp,temp->children[i], FlagIgnoreBan))
					find = AddToQ(temp->children[i], find);
			}
		}
	}
	return NULL;
}
void CleanFlags(NODE* node)
{
	QUEUE* find = InitQ();
	AddToQ(node, find);
	while (find->high)
	{
		NODE* temp = PopFromQ(find);
		temp->Visited_flag = 0;
		for (int j = 0; j < temp->AmountOfChildren; ++j)
		{
			if (NotBanned(temp, temp->children[j],0))
				AddToQ(temp->children[j], find);
		}
	}
}
//void CreateCallFunctionGraph(char* MassiveOfFunctions[][SIZE], FILE* MassiveOfFiles[][SIZE], int AmountOfFiles)
//{
//	FILE* FileName = fopen("New_File.ccp", "r");
//	char str[200] = { 0 };
//	char FuncName[SIZE] = { 0 };
//	while (1)
//	{
//		fgets(str,200,FileName);
//		if (!strcmp(str, "\n\0")) { break; }
//	}
//	fgets(str, 200, FileName);
//	int counter = 0;
//	while (str[counter] != ' ') ++counter;
//	++counter;
//	if (str[counter] >= 65 && str[counter] <= 90)//встретили функцию
//	{
//		int counter1 = 0;
//		while (0)
//		{
//
//		}
//	}
//}
