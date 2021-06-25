#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>

typedef struct Node
{
	char Name[50];
	struct Node* children[10];
	char BanListForBFS_DFS[10][50];
	struct Node* parent;
	int AmountOfChildren;
	int AmountOfBan;
	int Visited_flag;
} NODE;

typedef struct Queue
{
	NODE* Mas[50];
	int high;

}QUEUE;

QUEUE* AddToQ(NODE* node, QUEUE* mas);
NODE* PopFromQ(QUEUE* mas);

NODE* AddNode(NODE* node, char name[], NODE* MassOfRoots[], int Count, char Cycled[][25], int *CycledCount);
NODE* AddChild(NODE* node, NODE* child);
NODE* BFS(char* NameOfFunc, NODE* node, int FlagIgnoreBan);
void DFS(NODE* node, NODE* PastNode, char List [][50], int *CountList);
void CleanFlags(NODE* node);
int ThisIsNotDescendant(NODE* node, char name[]);