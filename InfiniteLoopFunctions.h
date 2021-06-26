#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MAXSIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* DeleteSpaces(char stroka[]);
//
void FullBodyOfCycle(char Files[], int StringCounter, char MassiveBody[][200], int* AmountOfStrings);
//
int FindInitVar(char name[], char Files[][20], int counter);
int CheckBreak(char file[], int string);
int FindChangesOfVarInCycle(char file[], int string, char var[]);
//
int FindInitialValueOfVar(char VarName[], char Files[][20], int counter, int* fract);
int CheckChangesOfVarAndTheirDirection(char file[], int string, char VarName[], int FlagMoreOrLess);
////2
int CalculateMinMaxOfRand(int* MinBorderOfDiapozone, int* MaxBorderOfDiapozone, char temp[], int l);
int FindIfWithBreak(char Files[], int Counter, int StateOfWork, char VarName[], int* FractPart);
//2


