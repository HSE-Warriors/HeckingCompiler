#pragma once

void grabStuff(int AmountOfFiles, char Files[][40], int AmountOfFunctions, char Functions[][50], int debug, int showMe, int allowSpam);
void KoshkinDoStuff();
void printOrigin(char givenString[]);
void isItMain(char givenString[], char Files[][40], int i);
void findWhitespace(char givenString[], int mode); //0 - первый, 1 - последний
void getName(char givenString[], int mode, int type, int number); //0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef; number
void countThings(char givenString[]);
void printThings();
void sayMessage(char which[99]);
void ending();

int countOfFiles = 0;
char namesOfFiles[50][256] = { 0 };

int countOfStrings = 0;
int numberOfCurrentString = 0;

int locationOfMain = 0;
char nameOfMainFile[99];

int countOfVariables = 0;
char namesOfVariables[99][256] = { "" };
int countOfMisspelledVariables = 0;
char namesOfMisspelledVariables[99][256];
int countOfUnusedVariables = 0;
char namesOfUnusedVariables[99][256];
int countOfEmptyVariables = 0;
char namesOfEmptyVariables[99][256];

int countOfFunctions = 0;
char namesOfFunctions[99][256] = { 0 };
int countOfMisspelledFunctions = 0;
char namesOfMisspelledFunctions[99][256];
int countOfUnusedFunctions = 0;
char namesOfUnusedFunctions[99][256];
int countOfSelfDefinedFunctions = 0;
char namesOfSelfDefinedFunctions[99][256];

int countOfMeetingThisVariable[99] = { 0 };
int countOfMeetingThisFunction[99] = { 0 };

int posOfWhitespace = 0;
char nameOfFileToReOpen[256];
char compareString[99];
int seen = 0; //почти не костыль
//int keysForVariables[999][4] = { -1, -1, -1, -1 }; //[номер строки][PascalStyle][correctName][selfdefined][used][type] //0 - перед main, 1 - в конце файла, 3 - в другом файле

typedef struct nodes
{
	char Name[50];
	struct nodes* children[10];
	char BanListForBFS_DFS[10][50];
	struct nodes* parent;
	int AmountOfChildren;
	int AmountOfBan;
	int Visited_flag;
}NODES;

int flag_E = 0; //кол-во успешно открытых файлов
int flag_F = 0; //показывалась ли директори€ с файлами
int flag_S = 0; //показывать исходный код в консоли или нет
int flag_D = 0; //дебаг-режим для бедных
int flag_L = 0; //больше спама
