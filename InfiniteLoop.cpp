#define _CRT_SECURE_NO_WARNINGS
#define MAXSIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "InfiniteLoopFunctions.h"

int main()
{
	char FORpossibleInfiniteLoopVariants[17][20] = { "for(;;)","for( ;;)","for(; ;)","for(;; )","for( ; ;)","for(; ; )","for( ;; )","for( ; ; )",
	"for (;;)","for ( ;;)","for (; ;)","for (;; )","for ( ; ;)","for (; ; )","for ( ;; )","for ( ; ; )","for  (;;)" };
	char Files[50][20] = { 0 };
	int AmountOfFiles = 1;//
	char Buffers[200] = { 0 };
	strcpy(Files[0],"Cycle.cpp");//
	int FlagMoreOrLess = 0;
	char InsideBrackets[20] = { 0 };
	char WhileBody[30] = { 0 };
	int counter = 0;
	char buff[50] = { 0 }; //buff - массив, содержащий фрагмент между точками с запятой в цикле for
	char buff2[50] = { 0 }; //buff2 - массив, содержащий фрагмент между второй точкой с запятой и ) в цикле for
	for (int v = 0; v < AmountOfFiles; ++v)
	{
		FILE* r;
		int StringCounter = 0;
		if (strstr(Files[v], ".cpp"))
		{
			r = fopen(Files[v], "r");
			while (!feof(r))
			{
				int Level_Of_Cycling = 0;
				int Number_Different_Types = 0;
				int FlagInit = 0;
				++StringCounter;
				for (int i = 0; i < 200; ++i)
					Buffers[i] = 0;
				for (int i = 0; i < 30; ++i)
					WhileBody[i] = 0;
				fgets(Buffers, 200, r); 
				if (strstr(Buffers, "while"))
				{
					strcpy(Buffers, DeleteComment(Buffers));
					if (!Number_Different_Types)
					{
						strcpy(WhileBody, DeleteSpaces(Buffers));
						if (!strcmp(WhileBody, "1") || !strcmp(WhileBody, "!0") || !strcmp(WhileBody, "!NULL"))// в условии 1, !0, !NULL
						{
							Number_Different_Types = 1;
						}
						else
						{
							int Flag_Already_Check = 0;
							if (strstr(WhileBody, "int") || strstr(WhileBody, "float") || strstr(WhileBody, "double") || strstr(WhileBody, "char")) //если внутри условия есть инициализация переменной
							{
								Number_Different_Types = CheckValue(WhileBody); //смотрим, не равно ли это 0 и от этого ставим 1 либо 0
								FlagInit = 1;
							}
							if (!Number_Different_Types && !FlagInit)
							{
								if (strchr(WhileBody, '=') && !strstr(WhileBody, "==")) //некорректное приравнивание к определенному значению, проверяем, не равно ли оно 0
								{
									int i = 0;
									while (WhileBody[i] != '=')
									{
										++i;
									}
									++i;
									while (WhileBody[i] != ')')
									{
										if ((WhileBody[i] > 48 && WhileBody[i] <= 57))
										{
											Number_Different_Types = 1;
											break;
										}
										++i;
									}
									if (!Number_Different_Types)
										FlagInit = 1;
								}
							}
							if (!Number_Different_Types && !FlagInit)
							{
								if (!(strchr(WhileBody, '>') || strchr(WhileBody, '<') || strchr(WhileBody, '=')))
								{
									if (!((WhileBody[0] == '-') || (WhileBody[0] >= 48 && WhileBody[0] <= 57)))//проверяем, переменная ли это
									{ //(смотрим на начало, там не должно быть числа, либо -)
										Number_Different_Types = FindInitVar(WhileBody, Files, AmountOfFiles); //если все же переменная, то ищем её инициализацию
										if (Number_Different_Types) //в зависимости от значения при инициализации смотрим на ее изменения в цикле и от этого ставим уровень
											Number_Different_Types = 2;
										else
											Number_Different_Types = 3;
										int FlagScanfGetchar = FindScanfGetchar(Files[v], StringCounter, WhileBody);
										if (FlagScanfGetchar)
										{
											Level_Of_Cycling = 3;
											Number_Different_Types = 10; //пропускаем switch
										}
										int FractPart = 0;
										int MinBorderOfDiapozone = 0;
										int MaxBorderOfDiapozone = RAND_MAX; //проверяем, задается ли переменная через рандомайзер
										int NumberRand = FindRand(Files[v], StringCounter, WhileBody, &FractPart, &MinBorderOfDiapozone, &MaxBorderOfDiapozone);
										if (!NumberRand)
										{
											if (!Number_Different_Types)
												Number_Different_Types = 4; //в диапазоне нет ни 0, ни числа из if (var == number) с break
										}
										else
										{
											if (NumberRand == 1)// есть одно из описанных выше
											{
												Number_Different_Types = 5;
											}
											else
												Number_Different_Types = 10;
										}
									}
									else
									{
										int i = 0;
										while (i < strlen(WhileBody))
										{
											if (!(WhileBody[i] == '-' || WhileBody[i] == '0' || WhileBody[i] == '.')) //проверяем их на равенство 0
											{
												Number_Different_Types = 1;
												break;
											}
											++i;
										}
									}
								}
								//
							}
						}
					}
				}
				else
				{
					if (strstr(Buffers, "for"))
					{
						strcpy(Buffers, DeleteComment(Buffers));
						int FlagNotCheck = 0;
						counter = 0;
						for (int i = 0; i < strlen(Buffers); ++i)
						{
							if (Buffers[i] == ';')
								++counter;
						}
						for (int i = 0; i < 17; ++i)
						{
							if (strstr(Buffers, FORpossibleInfiniteLoopVariants[i]))//если это один из for(;;)
							{
								Number_Different_Types = 1;
								FlagNotCheck = 1;
								break;
							}
						}
						int j = strchr(Buffers, ';') - Buffers + 1;
						int l = 0;
						if (!FlagNotCheck)
						{
							for (int n = 0; n < 50; ++n)
								buff[n] = 0;
							while (Buffers[j] != ';')
							{
								buff[l] = Buffers[j];
								++j;
								++l;
							}
							++j;
							l = 0;
							while (Buffers[j] != ')')
							{
								buff2[l] = Buffers[j];
								++j;
								++l;
							}
						}
						if (!Number_Different_Types && !FlagNotCheck)
						{
							if (strchr(buff, '=') && !(strstr(buff, "==") || strstr(buff, "<=") || strstr(buff, ">=")))
							{
								j = 0;
								while (buff[j] != '=')
								{
									++j;
								}
								++j;
								while (buff[j] == ' ')
								{
									++j;
								}
								while (j < strlen(buff))
								{
									if (buff[j] > 48 && buff[j] <= 57) //если у нас есть цикл формата for( ;x=a; )
									{
										Number_Different_Types = 1;
										break;
									}
									++j;
								}
							}
						}
						if (!Number_Different_Types && !FlagNotCheck)
						{
							if (!strchr(buff2, '=') && (strchr(buff2, '+') || strchr(buff2, '-'))) //проверка на ситуацию for(~~;~~;i+1)
							{
								if (!strstr(buff2, "++") && !strstr(buff2, "--"))
									Number_Different_Types = 1;
							}
						}
						//
					}
				}
				if (Number_Different_Types)
				{
					if ((strchr(Buffers, ';') && strstr(Buffers, "while")) || (strstr(Buffers, "for") && counter == 3)) // в конце ;
					{
						Number_Different_Types = 0; //== 0 -> пропускаем switch
						Level_Of_Cycling = 1;
					}
				}
				if (Number_Different_Types)
				{
					int TempLevel = 0;
					TempLevel = CheckBreak(Files[v], StringCounter);
					if (!TempLevel)
					{
						Level_Of_Cycling = 0;
						Number_Different_Types = 10; //пропускаем switch
					}
					switch (Number_Different_Types)
					{
					case 1: //явно бесконечные циклы
						Level_Of_Cycling = 1;
						TempLevel = FindIfWithBreak(Files[v], StringCounter, 1, Files[v], &v); //Передача &v и вторая Files[v] являются затычкой для работы функции
						if (TempLevel > Level_Of_Cycling)
							Level_Of_Cycling = TempLevel;
						break;
					case 2: //переменная из условия while, на момент иницилизации != 0
						TempLevel = FindChangesOfVarInCycle(Files[v], StringCounter, WhileBody);
						if (TempLevel)
							Level_Of_Cycling = 3;
						else
							Level_Of_Cycling = 2;
						TempLevel = FindIfWithBreak(Files[v], StringCounter, 1, Files[v], &v); 
						if (TempLevel == 3)
							Level_Of_Cycling = 3;
						break;
					default:
						break;
					}
				}
				switch (Level_Of_Cycling)
				{
				case 0:
					break;
				case 1:
					printf("There is infinite loop starting with a string %d -- 1\n", StringCounter);
					printf("NDT - %d\n", Number_Different_Types);
					break;
				case 2:
					printf("There is possible infinite loop starting with a string %d -- 2\n", StringCounter);
					printf("NDT - %d\n", Number_Different_Types);
					break;
				case 3:
					printf("Cycle in the string %d may be an infinite loop -- 3\n", StringCounter);
					printf("NDT - %d\n", Number_Different_Types);
					break;
				}
			}
		}
	}
	return 0;
}