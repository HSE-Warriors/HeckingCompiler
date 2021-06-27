#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Treee
{
	char NameOfFunction[50][50];
	char FileName[50][50];
	int Positions[50];
	int NumberOfString;
	int counter;
	int FlagOfIncludingInTree[50];
} TREE;

typedef struct node
{
	char Name[50];
	struct node* children[10];
	char BanListForBFS_DFS[10][50];
	struct node* parent;
	int AmountOfChildren;
	int AmountOfBan;
	int Visited_flag;
}NODE;

typedef struct queue
{
	NODE* Mas[50];
	int high;

}QUEUE;

TREE* Inits();
void FunctionsGraph(char Files[][40], int AmountOfFiles, char Functions[][50], int AmountOfFunctions, NODE* MassOfNodes[], NODE* MassOfRoots[]
				   , int *CountRoots);
QUEUE* AddToQ(NODE* node, QUEUE* mas);
NODE* PopFromQ(QUEUE* mas);
NODE* AddNode(NODE* node, char name[], NODE* MassOfRoots[], int Count, char Cycled[][25], int* CycledCount);
NODE* BFS(char* NameOfFunc, NODE* node, int FlagIgnoreBan);
void DFS(NODE* node, NODE* PastNode, char List[][50], int* CountList);
void CleanFlags(NODE* node);
int ThisIsNotDescendant(NODE* node, char name[]);