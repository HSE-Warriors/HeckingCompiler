#define _CRT_SECURE_NO_WARNINGS  //антикостыль
#include "KoshkinNeedsSomeHelp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //для isupper()/islower()

FILE *fileStream;

void grabStuff(int AmountOfFiles, char Files[][40], int * AmountOfFunctions, char * Functions[][50], int debug, int showMe, int allowSpam)
{
	flag_D = debug;
	flag_S = showMe;
	flag_L = allowSpam;
	сountOfFiles = AmountOfFiles;

	for (int i = 0; i < AmountOfFiles; i++)
	{
		strcpy(namesOfFiles[i], Files[i]);
	}
}

void KoshkinDoStuff()
{
	char lineOfCode[256] = "";
	for (int i = 0; i < countOfFiles; i++)
	{

		fileStream = fopen(namesOfFiles[i], "r");
		if (fileStream == NULL)
		{
			sayMessage("FE");
		}
		else
		{
			printf("\n���� �%d - \"%s\":\n", i + 1, namesOfFiles[i]);
		}

		//fgets(lineOfCode, sizeof lineOfCode, file);
		while (fgets(lineOfCode, sizeof lineOfCode, fileStream) != NULL)
		{
			numberOfCurrentString++;
			printOrigin(lineOfCode);
			isItMain(lineOfCode, namesOfFiles, i);
			countThings(lineOfCode);
		}
		//saveForOthers(namesOfFunctions, countOfFiles);
		countOfStrings += numberOfCurrentString;
		numberOfCurrentString = 0;

	}
	fclose(fileStream);

	for (int i = 0; i < countOfFiles; i++)
	{

		fileStream = fopen(namesOfFiles[i], "r");
		if (fileStream == NULL)
		{
			sayMessage("FE");
		}

		while (fgets(lineOfCode, sizeof lineOfCode, fileStream) != NULL)
		{
			doubleCheck(lineOfCode);
		}
	}
	fclose(fileStream);

	if (flag_D == 1)
	{
		printf("\x1b[32;1m > Second check is done, results:\n\x1b[0m");
		printf("\x1b[32;1mVariables:\nCount: %d\nMisspelled: %d\nUnused: %d\nEmpty: %d\n\x1b[0m", countOfVariables, countOfMisspelledVariables, countOfUnusedVariables, countOfEmptyVariables);
		printf("\x1b[32;1mFunctions:\nCount: %d\nMisspelled: %d\nUnused: %d\x1b[0m", countOfFunctions, countOfMisspelledFunctions, countOfUnusedFunctions);
	}
}

void printOrigin(char givenString[])
{
	if (flag_S == 1)
	{
		//printf("printOrigin");
		if (numberOfCurrentString < 10)
		{
			printf("  ");
		}
		else if (numberOfCurrentString < 100)
		{
			printf(" ");
		}
		setColor(4);
		printf("%d | ", numberOfCurrentString);
		fputs(givenString, stdout); //write line to console
		setColor(0);
	}
}

void isItMain(char givenString[], char Files[][40], int i)
{
	if (strstr(givenString, "int main()") || strstr(givenString, "int main(void)"))
	{
		locationOfMain = numberOfCurrentString;
		strcpy(nameOfMainFile, Files[i]);
	}
}

void findWhitespace(char givenString[], int mode)
{
	if (mode == 0) //первый
	{
		for (int i = 0; i < strlen(givenString); i++)
		{
			if (isspace(givenString[i]))
			{
				posOfWhitespace = i;
				break;
			}
		}
	}
	else if (mode == 1) //последний
	{
		for (int i = 0; i < strlen(givenString); i++)
		{
			if (isspace(givenString[i]))
			{
				posOfWhitespace = i;
			}
		}
	}
}

void getName(char givenString[], int mode, int type, int number)
{
	int i = 1;
	char gettinName[256];
	strcpy(gettinName, givenString);
	char gottenName[256];

	if (mode == 0)
	{
		char separation[10] = " =[];"; //для переменных
		char* subString = strtok(gettinName, separation);
		while (subString != NULL) //пока есть лексемы
		{
			if (i == 2)
			{
				strcpy(gottenName, subString);
				if (flag_D == 1)
				{
					printf("\x1b[35;1m \t> getName() used.\n\x1b[0m");
					printf("\x1b[35;1m \t> Gotten name - \"\x1b[33;1m%s\x1b[35;1m\". \n\x1b[0m", gottenName);
				}
			}
			subString = strtok(NULL, separation);
			i++;
		}

		if (type == 0)
		{
			strcpy(namesOfVariables[number], gottenName);
		}
		else if (type == 1)
		{
			strcpy(namesOfMisspelledVariables[number], gottenName);
		}
		else if (type == 2)
		{
			strcpy(namesOfUnusedVariables[number], gottenName);
		}
		else if (type == 3)
		{
			strcpy(namesOfEmptyVariables[number], gottenName);
		}
		else if (type == 4)
		{
			strcpy(compareString, gottenName);
		}
	}
	else if (mode == 1)
	{
		char separation[10] = " ();"; //для функций
		char* subString = strtok(gettinName, separation);
		while (subString != NULL) //пока есть лексемы
		{
			if (i == 2)
			{
				strcpy(gottenName, subString);
				if (flag_D == 1)
				{
					printf("\x1b[35;1m \t> getName() used.\n\x1b[0m");
					printf("\x1b[35;1m \t> Gotten name - \"\x1b[33;1m%s\x1b[35;1m\". \n\x1b[0m", gottenName);
				}
			}
			subString = strtok(NULL, separation);
			i++;
		}

		if (type == 0)
		{
			strcpy(namesOfFunctions[number], gottenName);
		}
		else if (type == 1)
		{
			strcpy(namesOfMisspelledFunctions[number], gottenName);
		}
		else if (type == 2)
		{
			strcpy(namesOfUnusedFunctions[number], gottenName);
		}
		else if (type == 3)
		{
			strcpy(namesOfSelfDefinedFunctions[number], gottenName); //����� //...��� ���?
		}
		else if (type == 4)
		{
			strcpy(compareString, gottenName);
		}
	}
}

void countThings(char givenString[])
{
	char comma[10] = ",";
	//is variable?
	if (flag_D == 1)
	{
		printf("\x1b[35;1m > Is variable? \n\x1b[0m");
	}
	if (strstr(givenString, ";") && !strstr(givenString, "(") && !strstr(givenString, ")") && (strstr(givenString, "int") || strstr(givenString, "long int") || strstr(givenString, "short int") || strstr(givenString, "long long int") || strstr(givenString, "unsigned int") || strstr(givenString, "long") || strstr(givenString, "long long") || strstr(givenString, "unsigned long") || strstr(givenString, "unsigned short") || strstr(givenString, "unsigned long long") || strstr(givenString, "float") || strstr(givenString, "double") || strstr(givenString, "lonng double") || strstr(givenString, "char") || strstr(givenString, "signed char") || strstr(givenString, "unsigned char")) && (!strstr(givenString, "printf") || !strstr(givenString, "// int") || !strstr(givenString, "// char" || !strstr(givenString, "// double" || !strstr(givenString, "// float"))) //мда
	{
		if (flag_D == 1)
		{
			printf("\x1b[35;1m \t> \x1b[32;1mYes.\x1b[35;1m \n\x1b[0m");
		}
		//was seen?
		getName(givenString, 0, 4, 0);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
		for (int i = 0; i < countOfVariables; i++)
		{
			if (strstr(namesOfVariables[i], compareString))
			{
				seen = 1;
			}
		}
		int temp = countOfVariables;
		if (seen != 1)
		{
			countOfVariables++;
		}

		int temp = countOfVariables;
		//is solo?
		if (strstr(givenString, comma))
		{
			//No
			if (flag_D == 1)
			{
				printf("\x1b[35;1m \t> \x1b[32;1mTwo of them.\x1b[35;1m \n\x1b[0m");
			}
			char twoNames[256];
			strcpy(twoNames, givenString);
			char* subStr = strtok(twoNames, ",");
			countOfVariables++;
			getName(subStr, 0, 0, countOfVariables - 1);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
			countOfVariables++;
			getName(twoNames, 0, 0, countOfVariables - 1);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
		}
		else
		{
			countOfVariables++;
			getName(givenString, 0, 0, countOfVariables - 1);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
		}

		//countOfMeetingThisVariable[countOfVariables - 1]++;

		if (flag_D == 1)
		{
			printf("\x1b[35;1m \t> Count of variables: \x1b[32;1m%d -> %d\x1b[35;1m. \n\x1b[0m", temp, countOfVariables);
		}
		//getName(givenString, 0, 0, countOfVariables - 1);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef

		if (flag_D == 1)
		{
			printf("\x1b[35;1m \t> It's name is \"\x1b[33;1m%s\x1b[35;1m\".\n\x1b[0m", namesOfVariables[countOfVariables - 1]);
		}

		//misspelled?
		if (flag_D == 1)
		{
			printf("\x1b[35;1m > Misspelled? \n\x1b[0m");
		}

		//findWhitespace(givenString, 0);
		if (isupper(namesOfVariables[countOfVariables - 1][0]) != 0)
		{
			temp = countOfMisspelledVariables;
			countOfMisspelledVariables++;
			if (flag_D == 1)
			{
				printf("\x1b[35;1m \t> \x1b[31;1mYes, it's first letter is upper.\n\x1b[0m");
				printf("\x1b[35;1m \t> Count of misspelled variables: \x1b[32;1m%d -> %d\x1b[32;1m.\n\x1b[0m", temp, countOfMisspelledVariables);
			}
			//printf("\n", temp, countOfMisspelledVariables);
			getName(givenString, 0, 1, countOfMisspelledVariables - 1); //0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
		}
		else
		{
			temp = countOfMisspelledVariables;
			if (flag_D == 1)
			{
				printf("\x1b[35;1m \t> \x1b[32;1mNo, it's first letter is lower.\n\x1b[0m");
				printf("\x1b[35;1m \t> Count of mispelled variables: \x1b[33m%d -> %d\x1b[35;1m.\n\x1b[0m", temp, countOfMisspelledVariables);
			}
		}
		//empty?
		if (flag_D == 1)
		{
			printf("\x1b[35;1m > Empty?\n\x1b[0m");
		}
		if (!(strchr(givenString, '=') || strstr(givenString, "scanf")))
		{
			temp = countOfEmptyVariables;
			countOfEmptyVariables++;
			getName(givenString, 0, 3, countOfEmptyVariables - 1);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
			if (flag_D == 1)
			{
				printf("\x1b[35;1m \t> \x1b[32;1mNot empty.\n\x1b[0m");
				printf("\x1b[35;1m \t> Count of empty variables: \x1b[33m%d -> %d\x1b[35;1m.\n\x1b[0m", temp, countOfEmptyVariables);
			}
		}
		else
		{
			temp = countOfEmptyVariables;
			if (flag_D == 1)
			{
				printf("\x1b[35;1m \t> \x1b[31;1mYes, empty.\n\x1b[0m");
				printf("\x1b[35;1m \t> Count of empty variables: \x1b[32;1m%d -> %d\x1b[35;1m.\n\x1b[0m", temp, countOfEmptyVariables);
			}
		}
	}
	else
	{
		if (flag_D == 1)
		{
			printf("\x1b[35;1m \t> \x1b[33mNo.\n\x1b[0m");
			printf("\x1b[35;1m \t> Count of variables: \x1b[33m%d -> %d\x1b[35;1m.\n\x1b[0m", countOfVariables, countOfVariables);
		}
	}

	//is function?
	if (flag_D == 1)
	{
		printf("\x1b[35;1m > Is function?\n\x1b[0m");
	}
	if (strstr(givenString, "(") && strstr(givenString, ")") && strstr(givenString, ";") && strstr(givenString, "void") || (strstr(givenString, "int") || strstr(givenString, "long int") || strstr(givenString, "short int") || strstr(givenString, "long long int") || strstr(givenString, "unsigned int") || strstr(givenString, "long") || strstr(givenString, "long long") || strstr(givenString, "unsigned long") || strstr(givenString, "unsigned short") || strstr(givenString, "unsigned long long") || strstr(givenString, "float") || strstr(givenString, "double") || strstr(givenString, "lonng double") || strstr(givenString, "char") || strstr(givenString, "signed char") || strstr(givenString, "unsigned char") || strstr(givenString, "string")) && !strstr(givenString, "main") && !strstr(givenString, "printf") && !strstr(givenString, "=")) //���
	{
		int temp = countOfFunctions;
		//was seen?
		getName(givenString, 1, 4, 0);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
		for (int i = 0; i < countOfFunctions; i++)
		{
			if (strstr(namesOfFunctions[i], compareString))
			{
				seen = 1;
			}
		}
		int temp = countOfFunctions;
		if (seen != 1)
		{
			countOfFunctions++;
		}
		AmountOfFunctions++;
		keysForFunctions[countOfFunctions - 1]++; //seen
		if (flag_D == 1)
		{
			printf("\x1b[35;1m \t> \x1b[32;1mYes.\n\x1b[0m");
			printf("\x1b[35;1m \t> Count of functions: \x1b[32;1m%d -> %d\x1b[35;1m.\n\x1b[0m", temp, countOfFunctions);
		}

		getName(givenString, 1, 0, countOfFunctions - 1);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef

		if (flag_D == 1)
		{
			printf("\x1b[35;1m \t> It's name is \"\x1b[33;1m%s\x1b[35;1m\".\n\x1b[0m", namesOfFunctions[countOfFunctions - 1]);
		}
		//misspelled?
		if (flag_D == 1)
		{
			printf("\x1b[35;1m > Misspelled?\n\x1b[0m");
		}
		//findWhitespace(givenString, 0);
		//if (islower(givenString[posOfWhitespace + 1]))
		if (islower(namesOfFunctions[countOfFunctions - 1][0]) != 0)
		{
			temp = countOfMisspelledFunctions;
			countOfMisspelledFunctions++;
			getName(givenString, 1, 1, countOfMisspelledFunctions - 1);//0 - var, 1 - f(); 0 - regular, 1 - misspelled, 2 - unused, 3 - empty/selfdef
			if (flag_D == 1)
			{
				printf("\x1b[35;1m \t> \x1b[31;1mYes, it's first letter is lower\x1b[35;1m.\n\x1b[0m");
				printf("\x1b[35;1m \t> Count of misspelled functions: \x1b[32;1m%d -> %d\x1b[35;1m.\n\x1b[0m", temp, countOfMisspelledFunctions);
			}
		}
		else
		{
			if (flag_D == 1)
			{
				temp = countOfMisspelledFunctions;
				printf("\x1b[35;1m \t> \x1b[32;1mNo, it's first letter is upper.\n\x1b[0m");
				printf("\x1b[35;1m \t> Count of misspelled functions: \x1b[32;1m%d -> %d\x1b[35;1m.\n\x1b[0m", temp, countOfMisspelledFunctions);
			}
		}
	}
	else
	{
		if (flag_D == 1)
		{
			printf("\x1b[35;1m \t> \x1b[33mNo.\n\x1b[0m");
			printf("\x1b[35;1m \t> Count of functions: \x1b[33m%d -> %d\x1b[35;1m.\n\x1b[0m", countOfFunctions, countOfFunctions);
		}
	}
}

void printThings()
{
	//Overall
	setColor(2);
	printf("\n--------\n");
	printf("Stats:\n");
	printf("Count of strings (overall): %d\n", numberOfCurrentString);
	printf("Found \"main()\" at line %d in file \"%s\"\n\n", locationOfMain, nameOfMainFile);
	//Variables
	printf("Count of variables: \x1b[32;1m%d", countOfVariables);
	if (countOfVariables > 0)
	{
		printf("\x1b[0m, their names are:\n");
		for (int i = 0; i < countOfVariables; i++)
		{
			printf("%d - \"%s\"\n", i + 1, namesOfVariables[i]);
		}
	}
	else
	{
		printf(".\n");
	}
	//Misspelled v's
	printf("Incorrectly named variables - ");
	if (countOfMisspelledVariables > 0)
	{
		setColor(1);
		printf("%d", countOfMisspelledVariables);
		setColor(2);
		printf(", their names are:\n");
		for (int i = 0; i < countOfMisspelledVariables; i++)
		{
			printf("%d - \"%s\"\n", i + 1, namesOfMisspelledVariables[i]);
		}
	}
	else
	{
		setColor(3);
		printf("%d", countOfMisspelledVariables);
		setColor(2);
		printf(".\n");
	}
	//Unused v's
	printf("Unused variables - ");
	if (countOfUnusedVariables > 0)
	{
		setColor(1);
		printf("%d", countOfUnusedVariables);
		setColor(2);
		printf(", their names are:\n");
		for (int i = 0; i < countOfUnusedVariables; i++)
		{
			printf("%d - \"%s\"\n", i + 1, namesOfUnusedVariables[i]);
		}
	}
	else
	{
		setColor(3);
		printf("%d", countOfUnusedVariables);
		setColor(2);
		printf(".\n");
	}
	//Empty v's
	printf("Empty variables - ");
	if (countOfEmptyVariables > 0)
	{
		setColor(1);
		printf("%d", countOfEmptyVariables);
		setColor(2);
		printf(", their names are:\n");
		for (int i = 0; i < countOfEmptyVariables; i++)
		{
			printf("%d - \"%s\"\n", i + 1, namesOfEmptyVariables[i]);
		}
	}
	else
	{
		setColor(3);
		printf("%d", countOfEmptyVariables);
		setColor(2);
		printf(".\n");
	}
	printf("\n");

	//Functions
	if (countOfFunctions > 0)
	{
		printf("Count of functions - %d, including \"main\" (at line %d) - %d. Their names are:\n", countOfFunctions, locationOfMain, countOfFunctions + 1);
		for (int i = 0; i < countOfFunctions; i++)
		{
			printf("%d - \"%s\"\n", i + 1, namesOfFunctions[i]);
		}
		//Misspelled f's
		printf("Incorrectly named - ");
		if (countOfMisspelledFunctions > 0)
		{
			setColor(1);
			printf("%d", countOfMisspelledFunctions);
			setColor(2);
			printf(", their names are:\n");
			for (int i = 0; i < countOfMisspelledFunctions; i++)
			{
				printf("%d - \"%s\"\n", i + 1, namesOfMisspelledFunctions[i]);
			}
		}
		else
		{
			setColor(3);
			printf("%d", countOfMisspelledFunctions);
			setColor(2);
			printf(".\n");
		}
		//Unused f's
		printf("Unused - ");
		if (countOfUnusedFunctions > 0)
		{
			setColor(1);
			printf("%d", countOfUnusedFunctions);
			setColor(2);
			printf(", their names are:\n");
			for (int i = 0; i < countOfUnusedFunctions; i++)
			{
				printf("%d - \"%s\"\n", i + 1, namesOfUnusedFunctions[i]);
			}
		}
		else
		{
			setColor(3);
			printf("%d", countOfUnusedFunctions);
			setColor(2);
			printf(".\n");
		}
		//SelfDef. f's
		printf("Self-defined - ");
		if (countOfSelfDefinedFunctions > 0)
		{
			setColor(1);
			printf("%d", countOfSelfDefinedFunctions);
			setColor(2);
			printf(", their names are:\n");
			for (int i = 0; i < countOfSelfDefinedFunctions; i++)
			{
				printf("%d - \"%s\"\n", i + 1, namesOfSelfDefinedFunctions[i]);
			}
		}
		else
		{
			setColor(3);
			printf("%d", countOfSelfDefinedFunctions);
			setColor(2);
			printf(".\n");
		}

		/* Den's Recursive Functions */
		char ListAlreadyPrinted[50][50] = { 0 };
		int CountList = 0;
		printf("\nRecursive functions:\n");
		for (int i = 0; i < CountRoots; ++i)
		{
			DFS(MassOfRoots[i], NULL, ListAlreadyPrinted, &CountList);
			CleanFlags(MassOfRoots[i]);
		}
		return;

		printf("\nGraph of functions calls:\n");
		for (int i = 0; i < countOfFunctions; ++i)
		{
			printf("%s: ", MassOfNodes[i]->Name);
			for (int j = 0; j < MassOfNodes[i]->AmountOfChildren; ++j)
			{
				printf("%s", MassOfNodes[i]->children[j]->Name);
				if (j != MassOfNodes[i]->AmountOfChildren - 1)
					printf(",");
			}
			printf("\n");
	}
	else
	{
		printf("Only \"main\" was found at line %d in file \"%s\", no other functions found.\n", locationOfMain, nameOfMainFile);
	}
	printf("--------\n");
	setColor(0);
}

void sayMessage(char which[99])
{
	if (strstr(which, "FE"))
	{
		setColor(1);
		printf("Файл не открылся.\n");
		setColor(0);
	}

	if (strstr(which, "NS"))
	{
		if (flag_D == 1)
		{
			setColor(2);
			printf("findWhitespace() ничего не нашёл.\n");
			setColor(0);
		}
	}
}

void ending()
{
	setColor(3);
	printf("\n\Конец.\n\n");
	setColor(0);
	int blink = getch();
	FILE* banner = fopen("banner.txt", "r");
	char string[150];
	while (fgets(string, sizeof string, banner) != NULL)
	{
		fputs(string, stdout);
	}
	printf("\n\n");
}
