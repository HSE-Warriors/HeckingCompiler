#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FunctionsGraph.h"

TREE* Inits()
{
	TREE* t = (TREE*)malloc(sizeof(TREE));
	if (t)
	{
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				t->NameOfFunction[i][j] = 0 ;
				t->FileName[i][j] = 0 ;
			}
			t->FlagOfIncludingInTree[i] = 0;
			t->Positions[i] = 0 ;
		}
		t->NumberOfString = 1;
		t->counter = 0;
	}
	return t;
}
char* DeleteComments(char string[])
{
	if (strchr(string, '/'))
	{
		int slash = strchr(string, '/') - string;
		if (string[slash + 1] == '/')
		{
			int j = slash + 2;
			int len = strlen(string);
			for (int i = j; i < len; ++i)
			{
				string[i] = 0;
			}
		}
	}
	return string;
}
void FunctionsGraph(char Files[][40], int AmountOfFiles, char Functions[][50], int AmountOfFunctions, NODE* MassOfNodes[], NODE* MassOfRoots[],
	int *CountRoots)
{
	char Buffers[200] = { 0 };
	char IndexOfEmptyFunc[100] = { 0 };
	/*NODE* MassOfRoots[50] = { 0 };
	int *CountRoots = 0;
	NODE* MassOfNodes[50] = { 0 };*/
	
	TREE* search = Inits();
	search->counter = AmountOfFunctions;
	for (int i = 0; i < search->counter; ++i)
	{
		strcpy(search->NameOfFunction[i], Functions[i]);
	}

	int ExitCounter = AmountOfFunctions;
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
				if (counters == 7)
				{
					int b = 1;
				}
				fgets(Buffers, 200, r);
				if (strchr(Buffers, '(') && !(strstr(Buffers, "if") || strstr(Buffers, "while") || strstr(Buffers, "printf") || strstr(Buffers, "scanf")
				                           || strstr(Buffers, "for") || strstr(Buffers, "getchar") || strchr(Buffers, ';')))
				{
					for (int j = 0; j < AmountOfFunctions; ++j)
					{
						if (IndexOfEmptyFunc[j] != -1)
						{
							char* buff = search->NameOfFunction[j];
							buff[strlen(buff)] = '(';
							strcpy(Buffers, DeleteComments(Buffers));
							if (strstr(Buffers, buff))
							{
								strcpy(search->FileName[j], Files[i]);
								search->Positions[j] = counters;
								--ExitCounter;
								IndexOfEmptyFunc[j] = -1;
								//buff[strlen(buff) - 1] = 0;
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
		if (!strcmp(search->FileName[i], ""))
			continue;
		char Cycled[50][25] = { 0 };
		int CycledCount = 0;
		NODE* NewNode = NULL;
		if (search->FlagOfIncludingInTree[i])
		{
			for (int j = 0; j < *CountRoots; ++j)
			{
				NewNode = BFS(search->NameOfFunction[i], MassOfRoots[j], 1);
				CleanFlags(MassOfRoots[j]);
				if (NewNode)
					break;
			}
		}
		else
		{
			NewNode = AddNode(0, search->NameOfFunction[i], MassOfRoots, *CountRoots, Cycled, &CycledCount);
			search->FlagOfIncludingInTree[i] = 1;
			MassOfRoots[*CountRoots] = NewNode;
			++*CountRoots;
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
						NewNode = AddNode(NewNode, search->NameOfFunction[j], MassOfRoots, *CountRoots, Cycled, &CycledCount);
					}
					else
					{
						for (int k = 0; k < *CountRoots; ++k)
						{
							NODE* tempor = BFS(search->NameOfFunction[j], MassOfRoots[k], 1);
							if (tempor != NULL && !ThisIsNotDescendant(NewNode, tempor->Name))
							{
								strcpy(NewNode->BanListForBFS_DFS[NewNode->AmountOfBan], tempor->Name);
								++NewNode->AmountOfBan;
							}
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
				for (int k = 0; k < *CountRoots; ++k)
				{
					int flag = ThisIsNotDescendant(NewNode, MassOfRoots[k]->Name);
					if (!strcmp(NewNode->children[j]->Name, MassOfRoots[k]->Name) && flag)
					{
						MassOfRoots[k] = NewNode;//подумать над этим
					}
				}
			}
		}
		MassOfNodes[i] = NewNode;
	}
}

NODE* InitN()
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
		NODE* temp = find->Mas[find->high - 1];
		find->Mas[find->high] = NULL;
		--find->high;
		return temp;
	}
	else return NULL;
}

int ThisIsNotDescendant(NODE* node, char name[])
{
	if (!strcmp(node->Name, name))
		return 0;
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

NODE* AddNode(NODE* node, char name[], NODE* MassOfRoots[], int Count, char Cycled[][25], int* CycledCount)
{
	if (node == NULL)
	{
		NODE* nodes = InitN();
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
int CheckNodeNotRote(char NameOfNode[50], NODE* MassOfRoots[50], int *CountRoots)
{
	for (int i = 0; i < *CountRoots; ++i)
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
				if (!temp->children[i]->Visited_flag && NotBanned(temp, temp->children[i], FlagIgnoreBan))
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
			if (NotBanned(temp, temp->children[j], 0))
				AddToQ(temp->children[j], find);
		}
	}
}
int NotInList(char Name[], char List[][50], int Count)
{
	for (int i = 0; i < Count; ++i)
	{
		if (!strcmp(Name, List[i]))
			return 0;
	}
	return 1;
}

void DFS(NODE* node, NODE* PastNode, char List[][50], int* CountList)
{
	if (node->Visited_flag == 1)
	{
		NODE* temp = PastNode;
		while (temp->parent != NULL)
		{
			if (NotInList(temp->Name, List, *CountList))
			{
				char InputBuff[50] = { 0 };
				strcpy(InputBuff, temp->Name);
				InputBuff[strlen(InputBuff) - 1] = '\0';
				printf("%s\n", InputBuff);
				strcpy(List[*CountList], temp->Name);
				++* CountList;
			}
			temp = temp->parent;
		}
		if (NotInList(temp->Name, List, *CountList))
		{
			char InputBuff[50] = { 0 };
			strcpy(InputBuff, temp->Name);
			InputBuff[strlen(InputBuff) - 1] = '\0';
			printf("%s\n", InputBuff);
			strcpy(List[*CountList], temp->Name);
			++* CountList;
		}
		else
		{
		}
	}
	else
	{
		node->Visited_flag = 1;
		for (int i = 0; i < node->AmountOfChildren; ++i)
		{
			DFS(node->children[i], node, List, CountList);
		}
	}
}