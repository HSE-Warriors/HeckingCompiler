#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#define MAXSIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "InfiniteLoopFunctions.h"

void InfiniteLoop(char Files[][40], int AmountOfFiles, char InfiniteLoopSrtings[][100], int *AmountOfInfiniteMessages)
{
	char FORpossibleInfiniteLoopVariants[17][20] = { "for(;;)","for( ;;)","for(; ;)","for(;; )","for( ; ;)","for(; ; )","for( ;; )","for( ; ; )",
	"for (;;)","for ( ;;)","for (; ;)","for (;; )","for ( ; ;)","for (; ; )","for ( ;; )","for ( ; ; )","for  (;;)" };
	char Buffers[200] = { 0 };
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
								else
								{
									if (!(strchr(WhileBody, '=')))
									{
										if (!((WhileBody[0] == '-') || (WhileBody[0] >= 48 && WhileBody[0] <= 57)))
										{
											int i = 0;
											int j = 0;
											int FractionalPart = 0;
											char BuffVarOrNumber[15] = { 0 };
											if (strchr(WhileBody, '>'))
												i = strchr(WhileBody, '>') - WhileBody;
											if (strchr(WhileBody, '<'))
												i = strchr(WhileBody, '<') - WhileBody;
											int Limit = i - 1;
											while (WhileBody[Limit - 1] == ' ')
												--Limit;
											if (WhileBody[i] == '=')
												++i;
											for (int k = 0; k < Limit; ++k)
											{
												BuffVarOrNumber[j] = WhileBody[k];
												++j;
											}
											char VarName[50] = { 0 };
											strcpy(VarName, BuffVarOrNumber);
											int ValueOfVar = FindInitialValueOfVar(BuffVarOrNumber, Files, AmountOfFiles, &FractionalPart);
											int FractPartVar = FractionalPart;
											FractionalPart = 0;
											j = 0;
											for (int n = 0; n < 15; ++n)
												BuffVarOrNumber[n] = 0;
											++i;
											while (WhileBody[i] == ' ')
												++i;
											while (i < strlen(WhileBody))
											{
												BuffVarOrNumber[j] = WhileBody[i];
												++i;
												++j;
											}
											int ValueOfNumber = FindValueOfNumber(BuffVarOrNumber, &FractionalPart);
											if (strchr(WhileBody, '>'))
											{
												FlagMoreOrLess = 1; // 1 - больше
												if (strstr(WhileBody, ">="))
												{
													if (ValueOfVar >= ValueOfNumber)
														Number_Different_Types = 6;
													else
													{
														if (ValueOfVar == ValueOfNumber && FractPartVar >= FractionalPart)
															Number_Different_Types = 6;
													}
												}
												else 
													if (ValueOfVar > ValueOfNumber)
														Number_Different_Types = 6;
													else
													{
														if (ValueOfVar == ValueOfNumber && FractPartVar > FractionalPart)
															Number_Different_Types = 6;
													}
											}
											else
											{
												FlagMoreOrLess = 2; // 2 - меньше
												if (strstr(WhileBody, "<="))
												{
													if (ValueOfVar <= ValueOfNumber)
														Number_Different_Types = 6;
													else
													{
														if (ValueOfVar == ValueOfNumber && FractPartVar <= FractionalPart)
															Number_Different_Types = 6;
													}
												}
												else
												{
													if (ValueOfVar < ValueOfNumber)
														Number_Different_Types = 6;
													else
													{
														if (ValueOfVar == ValueOfNumber && FractPartVar < FractionalPart)
															Number_Different_Types = 6;
													}
												}
											}
											strcpy(WhileBody, VarName);
										}
									}
								}
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
						if (!Number_Different_Types && !FlagNotCheck)
						{
							int UpOrDown = 0; // == 0 -> --, == 1 -> ++
							int Operator = 0; // '<' == 0, '>' == 1, '<=' or '>=' == 2
							if ((strstr(buff2, "--")) || (strstr(buff2, "-=")) || ((strchr(buff2, '-')) && strchr(buff2, '=')) || (strstr(buff2, "/ ")))
								UpOrDown = 0;
							if ((strstr(buff2, "++")) || (strstr(buff2, "+=")) || ((strchr(buff2, '+')) && strchr(buff2, '=')) || (strstr(buff2, "* ")))
								UpOrDown = 1;
							if (strchr(buff, '>'))
								Operator = 0;
							if (strstr(buff, ">="))
								Operator = 1;
							if (strchr(buff, '<'))
								Operator = 2;
							if (strstr(buff, "<="))
								Operator = 3;
							char Body[100] = { 0 };
							int l = 0;
							j = 0;
							while (Buffers[j] != '(')
							{
								++j;
							}
							++j;
							while (Buffers[j] != ')')
							{
								Body[l] = Buffers[j];
								++j;
								++l;
							}
							char FirstNumber[20] = { 0 };
							char SecondNumber[20] = { 0 };
							int CounterNumber = 0;
							int i = 0;
							while (i < strlen(Body))
							{
								if (!FirstNumber[0] && (Body[i] == '-' || (Body[i] >= 48 && Body[i] <= 57)))
								{
									while ((Body[i] >= 48 && Body[i] <= 57) || Body[i] == '.' || Body[i] == '-')
									{
										FirstNumber[CounterNumber] = Body[i];
										++i;
										++CounterNumber;
									}
									CounterNumber = 0;
								}
								if (FirstNumber[0] && (Body[i] == '-' || (Body[i] >= 48 && Body[i] <= 57)))
								{
									while ((Body[i] >= 48 && Body[i] <= 57) || Body[i] == '.' || Body[i] == '-')
									{
										SecondNumber[CounterNumber] = Body[i];
										++i;
										++CounterNumber;
									}
									CounterNumber = 0;
									break;
								}
								++i;
							}
							int Fract = 0;
							int FirstNumb = FindValueOfNumber(FirstNumber, &Fract);
							float FirstNumbF = FirstNumb;
							AddFractPart(&FirstNumbF, Fract);
							Fract = 0;
							int SecondNumb = FindValueOfNumber(SecondNumber, &Fract);
							float SecondNumbF = SecondNumb;
							AddFractPart(&SecondNumbF, Fract);

							int FlagMoreLessEqual = 0; // '==' -> 0, '<' -> 1, '>' -> 2
							if (FirstNumbF == SecondNumbF)
								FlagMoreLessEqual = 0;
							else
							{
								if (FirstNumbF < SecondNumbF)
									FlagMoreLessEqual = 1;
								else
									FlagMoreLessEqual = 2;
							}
							//если внутри есть условие на повышение/понижение, а цикл увеличения переменной в ней идет вниз/вверх соотвественно или for(int i=0;i>=0;i++)
							if (!FlagMoreOrLess)
							{
								if (Operator == 1 && UpOrDown)
									Number_Different_Types = 1;
								if (Operator == 3 && !UpOrDown)
									Number_Different_Types = 1;
							}
							if (FlagMoreLessEqual == 2 && Operator <= 1 && UpOrDown)
								Number_Different_Types = 1;
							if (FlagMoreLessEqual == 1 && Operator >= 2  && !UpOrDown)
								Number_Different_Types = 1;
						}
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
					case 3: //переменная из условия while, на момент иницилизации == 0
						TempLevel = FindChangesOfVarInCycle(Files[v], StringCounter, WhileBody);
						if (TempLevel)
							Level_Of_Cycling = 0;
						else
							Level_Of_Cycling = 3;
						break;
					case 4: //рандомайзер над переменной, не выполняются оба условия
						Level_Of_Cycling = 2;
						/*TempLevel = FindIfWithBreak(Files[v], StringCounter, 1, Files[v], &v);
						if (TempLevel == 3)
							Level_Of_Cycling = 3;*/
						break;
					case 5: //рандомайзер над переменной, выполняется только 1 из условий ИЛИ не изменение переменной из случая while(i >(<) 2) и в больш, и в меньш. стороны
						Level_Of_Cycling = 3;
						break;
					case 6: //изменения переменной из случая while(i >(<) 2) в меньш. и больш. сторону соотвественно
						Level_Of_Cycling = CheckChangesOfVarAndTheirDirection(Files[v], StringCounter, WhileBody, FlagMoreOrLess);
						break;
					default:
						break;
					}
				}
				char BuffersString[200] = { 0 };
				char BuffNumber[10] = { 0 };
				switch (Level_Of_Cycling)
				{
				case 0:
					break;
				case 1:
					strcpy(BuffersString, "There is infinite loop starting with a string ");
					itoa(StringCounter, BuffNumber, 10);
					strncat(BuffersString, BuffNumber, 4);
					strncat(BuffersString, " in file ", 9);
					strncat(BuffersString, Files[v], 30);
					strcpy(InfiniteLoopSrtings[*AmountOfInfiniteMessages], BuffersString);
					++*AmountOfInfiniteMessages;
					break;
				case 2:
					strcpy(BuffersString, "There is possible infinite loop starting with a string ");
					itoa(StringCounter, BuffNumber, 10);
					strncat(BuffersString, BuffNumber, 4);
					strncat(BuffersString, " in file ", 9);
					strncat(BuffersString, Files[v], 30);
					strcpy(InfiniteLoopSrtings[*AmountOfInfiniteMessages], BuffersString);
					++* AmountOfInfiniteMessages;
					break;
				case 3:
					strcpy(BuffersString, "There is possibility to be infinite for loop starting with a string ");
					itoa(StringCounter, BuffNumber, 10);
					strncat(BuffersString, BuffNumber, 4);
					strncat(BuffersString, " in file ", 9);
					strncat(BuffersString, Files[v], 30);
					strcpy(InfiniteLoopSrtings[*AmountOfInfiniteMessages], BuffersString);
					++* AmountOfInfiniteMessages;
					break;
				}
			}
		}
	}
	return 0;
}