#pragma once


#ifndef RIBSGBFUNCTIONS_H

#define _CRT_SECURE_NO_WARNINGS
#define MAX_CALL_LIST_SIZE 40
#define MAX_STR_LEN 500
#define MAX_STR_QUANTITY 100
#define MAX_WORDS 1000
#define MAX_WORD_LEN 60
#define MAX_NAME_LEN 25
#define MAX_FUNC_QUANTITY 50
#define MAS_LENG 200

#define ARITHMETIC_TYPES_CNT 7
#define OPTIONAL_TYPES_CNT 4

#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct CallPair
{
	char Caller[200];
	char Called[200];
} CALLPAIR; //CORRECT

bool belongsToTheWordArray(char* word, char wordArray[][10], unsigned wordsCnt);

void EnlistMutuallyCalledHeaders(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, CallPair callPairList[MAX_CALL_LIST_SIZE], 
								CallPair mutualCalls[MAX_CALL_LIST_SIZE], int *mutualCallsCnt);

void DetectCustomFunction(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, CallPair customFuncList[MAX_FUNC_QUANTITY],
					      int *customFuncCnt);

void DetectCustomTypes(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, CallPair customTypesList[MAX_FUNC_QUANTITY], 
					   int *customTypesCnt);

void CountMaxLoopDepth(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, int *maxTabs);

#endif