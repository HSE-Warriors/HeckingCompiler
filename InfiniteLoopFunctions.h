#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MAXSIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* DeleteSpaces(char stroka[]);
char* DeleteComment(char string[]);
void FullBodyOfCycle(char Files[], int StringCounter, char MassiveBody[][200], int* AmountOfStrings);
int CheckValue(char mas[]);
int FindInitVar(char name[], char Files[][40], int counter);
int CheckBreak(char file[], int string);
int FindChangesOfVarInCycle(char file[], int string, char var[]);
int FindValueOfNumber(char MassNumber[], int* fract);
int FindInitialValueOfVar(char VarName[], char Files[][40], int counter, int* fract);
int CheckChangesOfVarAndTheirDirection(char file[], int string, char VarName[], int FlagMoreOrLess);
int FindScanfGetchar(char Files[], int Counter, char VarName[]);
void AddFractPart(float* MainPart, float FractPart);
int CalculateMinMaxOfRand(int* MinBorderOfDiapozone, int* MaxBorderOfDiapozone, char temp[], int l);
int FindIfWithBreak(char Files[], int Counter, int StateOfWork, char VarName[], int* FractPart);
int FindRand(char Files[], int Counter, char VarName[], int* FractRand, int* MinBorderOfDiapozone, int* MaxBorderOfDiapozone);
