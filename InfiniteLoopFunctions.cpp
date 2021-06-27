#include "InfiniteLoopFunctions.h"

char* DeleteSpaces(char stroka[])
{
	char str[30] = { 0 };
	int i = 0;
	while (stroka[i] != '(')
	{
		++i;
	}
	++i;
	int j = 0;
	while (stroka[i] != ')')
	{
		str[j] = stroka[i];
		++j;
		++i;
	}
	i = 0;
	for (int i = 0; i < j; ++i)
	{
		if (str[i] == 32)
		{
			for (int m = i; m < j; ++m)
				str[m] = str[m + 1];
		}
	}
	return str;
}
char* DeleteComment(char string[])
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
void FullBodyOfCycle(char Files[], int StringCounter, char MassiveBody[][200], int* AmountOfStrings)
{
	int CounterOfBracket = 0;
	int NumberOfAnswer = 0;
	FILE* f = fopen(Files, "r");
	char buf[200] = { 0 };
	for (int i = 0; i < StringCounter; ++i)
		fgets(buf, 200, f);
	while (1)
	{
		fgets(buf, 200, f);
		strcpy(buf, DeleteComment(buf));
		strcpy(MassiveBody[*AmountOfStrings], buf);
		++* AmountOfStrings;
		if (strchr(buf, '{')) ++CounterOfBracket;
		if (strchr(buf, '}')) --CounterOfBracket;
		if (!CounterOfBracket)
			break;
	}
	return;
}
int CheckValue(char mas[])
{
	int i = 0;
	while (mas[i] != '=')
	{
		++i;
	}
	++i;
	while (mas[i] == ' ')
	{
		++i;
	}
	if (strstr(mas, "char"))
	{
		if (mas[i] == 48)
			return 0;
		else
			return 1;
	}
	else
	{
		while (mas[i] != ' ' && mas[i] != ';')
		{
			if (!(mas[i] == '-' || mas[i] == '0' || mas[i] == '.'))
				return 1;
			++i;
		}
		return 0;
	}
}
int FindInitVar(char name[], char Files[][20], int counter)
{
	for (int i = 0; i < counter; ++i)
	{
		FILE* str;
		int StringCounter = 0;
		char Massive[200] = { 0 };
		if (strstr(Files[i], ".cpp"))
		{
			str = fopen(Files[i], "r");
			while (!feof(str))
			{
				fgets(Massive, 200, str);
				strcpy(Massive, DeleteComment(Massive));
				if (strstr(Massive, name) && !strchr(Massive, '[') && !strchr(Massive, ']'))
				{
					if (strstr(Massive, "int") || strstr(Massive, "float") || strstr(Massive, "double") || strstr(Massive, "char")) //?//
					{
						return CheckValue(Massive);
					}
				}
			}
		}
	}
	return 0;
}
int CheckBreak(char file[], int string)
{
	FILE* ff = fopen(file, "r");
	char buf[200] = { 0 };
	char PastBuf[200] = { 0 };
	for (int i = 0; i < string; ++i)
		fgets(buf, 200, ff);
	int CounterOfBracket = 0;
	int flag_if = 0;
	int flag_ather = 0;
	while (1)
	{
		fgets(buf, 200, ff);
		strcpy(buf, DeleteComment(buf));
		if (strchr(buf, '{')) ++CounterOfBracket;
		if (strchr(buf, '}')) --CounterOfBracket;
		if (!CounterOfBracket)
			break;
		if (strstr(buf, "break") && CounterOfBracket == 1)
		{
			if (!(strstr(PastBuf, "if")))
			{
				fclose(ff);
				return 0;
			}
		}
		strcpy(PastBuf, buf);
	}
	fclose(ff);
	return 1;
}
int FindChangesOfVarInCycle(char file[], int string, char var[])
{
	char MassiveBody[80][200] = { 0 };
	int StringCounter = 0;
	FullBodyOfCycle(file, string, MassiveBody, &StringCounter);
	for (int j = 0; j < StringCounter; ++j)
	{
		if (strstr(MassiveBody[j], var))
		{
			if (strstr(MassiveBody[j], "++") || strstr(MassiveBody[j], "+=") || strstr(MassiveBody[j], "+ ") || strstr(MassiveBody[j], "* ") || strstr(MassiveBody[j], "*="))
				return 1;
			if (strstr(MassiveBody[j], "--") || strstr(MassiveBody[j], "-=") || strstr(MassiveBody[j], "- ") || strstr(MassiveBody[j], "/ ") || strstr(MassiveBody[j], "/="))
				return 1;
		}
	}
	return 0;
}
int FindValueOfNumber(char MassNumber[], int* fract)
{
	int i = 0;
	int FlagNegative = 0;
	int FlagDouble = 0;
	int number = 0;
	if (MassNumber[0] == '-')
	{
		++i;
		FlagNegative = 1;
	}
	number += MassNumber[i] - 48;
	++i;
	for (int j = i; j < strlen(MassNumber); ++j)
	{
		if (MassNumber[j] == '.' || FlagDouble)
		{
			if (FlagDouble)
			{
				*fract *= 10;
				*fract += MassNumber[j] - 48;
			}
			else
				FlagDouble = 1;
		}
		else
		{
			number *= 10;
			number += MassNumber[j] - 48;
		}
	}
	if (FlagNegative)
		number *= -1;
	return number;
}
int FindInitialValueOfVar(char VarName[], char Files[][20], int counter, int* fract)
{
	char Bufferes[200] = { 0 };
	for (int i = 0; i < counter; ++i)
	{
		FILE* str;
		int StringCounter = 0;
		if (strstr(Files[i], ".cpp"))
		{
			str = fopen(Files[i], "r");
			while (!feof(str))
			{
				fgets(Bufferes, 200, str);
				strcpy(Bufferes, DeleteComment(Bufferes));
				int StringCounter = 0;
				if ((strstr(Bufferes, VarName) && !strchr(Bufferes, '[') && !strchr(Bufferes, ']')) && (strstr(Bufferes, "int") || strstr(Bufferes, "float") || strstr(Bufferes, "double")))
				{
					char Number[30] = { 0 };
					int k = 0;
					int l = 0;
					while (Bufferes[l] != '=')
						++l;
					++l;
					while (Bufferes[l] == ' ')
						++l;
					while (Bufferes[l] != ' ' && Bufferes[l] != ';')
					{
						Number[k] = Bufferes[l];
						++k;
						++l;
					}
					return FindValueOfNumber(Number, fract);
				}
			}
		}
	}
	return 0;
}
int CheckChangesOfVarAndTheirDirection(char file[], int string, char VarName[], int FlagMoreOrLess)
{
	int CounterOfBracket = 0;
	int NumberOfAnswer = 0;
	int FlagChanges = 0;
	char MassiveBody[80][200] = { 0 };
	int StringCounter = 0;
	int FlagNoChanges = 0;
	FullBodyOfCycle(file, string, MassiveBody, &StringCounter);
	for (int j = 0; j < StringCounter; ++j)
		if (strstr(MassiveBody[j], VarName))
			FlagNoChanges = 1;
	if (!FlagNoChanges)
		return 2;
	for (int j = 0; j < StringCounter; ++j)
	{
		if (strchr(MassiveBody[j], '{')) ++CounterOfBracket;
		if (strchr(MassiveBody[j], '}')) --CounterOfBracket;
		if (!CounterOfBracket)
			break;
		if (strstr(MassiveBody[j], VarName) && (strstr(MassiveBody[j], "++") || strstr(MassiveBody[j], "+=") || strstr(MassiveBody[j], "*=") || strstr(MassiveBody[j], "+ ") || strstr(MassiveBody[j], "* ")))
		{
			if (FlagChanges <= 1)
				FlagChanges = 1;
		}
		if (strstr(MassiveBody[j], VarName) && (strstr(MassiveBody[j], "--") || strstr(MassiveBody[j], "-=") || strstr(MassiveBody[j], "/=") || strstr(MassiveBody[j], "- ") || strstr(MassiveBody[j], "/ ")))
		{
			FlagChanges = 2;
		}
	}
	if (FlagMoreOrLess == FlagChanges || !FlagChanges)
		return 2;
	else
		return 3;

}
int FindScanfGetchar(char Files[], int Counter, char VarName[])
{
	int CounterOfBracket = 0;
	int NumberOfAnswer = 0;
	char MassiveBody[80][200] = { 0 };
	int StringCounter = 0;
	FullBodyOfCycle(Files, Counter, MassiveBody, &StringCounter);
	for (int j = 0; j < StringCounter; ++j)
	{
		char Body[50] = { 0 };
		if (strstr(MassiveBody[j], "scanf"))
		{
			int i = 0;
			if (strstr(MassiveBody[j], "if"))
			{
				while (MassiveBody[j][i] != '(')
				{
					++i;
				}
				++i;
			}
			while (MassiveBody[j][i] != '(')
			{
				++i;
			}
			++i;
			int k = 0;
			while (MassiveBody[j][i] != ')')
			{
				Body[k] = MassiveBody[j][i];
				++i;
				++k;
			}
			if (strstr(Body, VarName))
				return 1;
		}
		else
		{
			if (strstr(MassiveBody[j], "getchar") && strchr(MassiveBody[j], '='))
			{
				int k = 0;
				int i = 0;
				while (MassiveBody[j][i] != '=')
				{
					Body[k] = MassiveBody[j][i];
					++k;
					++i;
				}
				if (strstr(Body, VarName))
					return 1;
			}
		}
	}
	return 0;
}
void AddFractPart(float* MainPart, float FractPart)
{
	int counter = 0;
	int c = FractPart;
	while (c)
	{
		++counter;
		c /= 10;
	}
	FractPart /= pow(10, counter);
	*MainPart += FractPart;
	return;
}
int CalculateMinMaxOfRand(int* MinBorderOfDiapozone, int* MaxBorderOfDiapozone, char temp[], int l)
{
	int i = 0;
	int FlagNegNumber = 0;
	while (i < l)
	{
		int TempNumber = 0;
		if (temp[i] == '%' || temp[i] == '+' || temp[i] == '-')
		{
			char Operator = temp[i];
			++i;
			while (temp[i] == ' ')
				++i;
			if (i >= l)
				break;
			while (temp[i] != ' ')
			{
				if (temp[i] == '-')
				{
					++FlagNegNumber;
					break;
				}
				else
				{
					while (temp[i] != 32 && temp[i] != ';' && i < strlen(temp))
					{
						TempNumber *= 10;
						TempNumber += temp[i] - 48;
						++i;
					}
				}
				if (FlagNegNumber)
					break;
				switch (Operator)
				{
				case '%':
					if (*MaxBorderOfDiapozone > TempNumber)
						*MaxBorderOfDiapozone = TempNumber;
					*MinBorderOfDiapozone = 0;
					break;
				case '+':
					*MinBorderOfDiapozone += TempNumber;
					*MaxBorderOfDiapozone += TempNumber;
					break;
				case '-':
					*MinBorderOfDiapozone -= TempNumber;
					*MaxBorderOfDiapozone -= TempNumber;
					break;
				}
				if (i >= l)
					break;
				TempNumber = 0;
			}
		}
		++i;
	}
	if (FlagNegNumber)
		return 0;
	return 1;
}
int FindIfWithBreak(char Files[], int Counter, int StateOfWork, char VarName[], int* FractPart)
{
	int CounterOfBracket = 0;
	int NumberOfAnswer = 0;
	char MassiveBody[80][200] = { 0 };
	int StringCounter = 0;
	FullBodyOfCycle(Files, Counter, MassiveBody, &StringCounter);
	for (int j = 0; j < StringCounter; ++j)
	{
		if (strchr(MassiveBody[j], '{')) ++CounterOfBracket;
		if (strchr(MassiveBody[j], '}')) --CounterOfBracket;
		if (!CounterOfBracket)
			break;
		if (strstr(MassiveBody[j], "if"))
		{
			if (StateOfWork)
			{
				int CounOfBrack = CounterOfBracket;
				for (int k = j; k < StringCounter; ++k)
				{
					if (strchr(MassiveBody[k], '{')) ++CounOfBrack;
					if (strchr(MassiveBody[k], '}')) --CounOfBrack;
					if (!CounOfBrack)
						break;
					if (strstr(MassiveBody[k], "break"))
					{
						if (CounOfBrack == CounterOfBracket + 1)
							return 3;
						if (strstr(MassiveBody[k - 1], "if"))
							return 3;
					}
				}
			}
			else
			{
				int flag = 0;
				int CounOfBrack = CounterOfBracket;
				for (int k = j; k < StringCounter; ++k)
				{
					if (strchr(MassiveBody[k], '{')) ++CounOfBrack;
					if (strchr(MassiveBody[k], '}')) --CounOfBrack;
					if (!CounOfBrack)
						break;
					if (strstr(MassiveBody[k], "break"))
					{
						if (CounOfBrack == CounterOfBracket + 1)
							flag = 1;
						if (strstr(MassiveBody[k - 1], "if"))
							flag = 1;
					}
				}
				if (flag)
				{
					if (strstr(MassiveBody[j], VarName) && strstr(MassiveBody[j], "=="))
					{
						int i = 0;
						int k = 0;
						char temp[100] = { 0 };
						while (MassiveBody[j][k] != '(')
							++k;
						while (MassiveBody[j][k] != '=')
							++k;
						k += 2;
						while (MassiveBody[j][k] == ' ')
							++k;
						if (MassiveBody[j][k] == '-' || (MassiveBody[j][k] >= 48 && MassiveBody[j][k] <= 57))
						{
							while (MassiveBody[j][k] == '-' || (MassiveBody[j][k] >= 48 && MassiveBody[j][k] <= 57))
							{
								temp[i] = MassiveBody[j][k];
								++i;
								++k;
							}
							return FindValueOfNumber(temp, FractPart);
						}
						else
							return 0;
					}
					else
						return 0;
				}
				else
					return 0;
			}
		}
	}
	return 0;
}
int FindRand(char Files[], int Counter, char VarName[], int* FractRand, int* MinBorderOfDiapozone, int* MaxBorderOfDiapozone)
{
	int CounterOfBracket = 0;
	int NumberOfAnswer = 0;
	char MassiveBody[80][200] = { 0 };
	int StringCounter = 0;
	int Flag0 = 0;
	int FlagIfBreak = 0;
	FullBodyOfCycle(Files, Counter, MassiveBody, &StringCounter);
	for (int j = 0; j < StringCounter; ++j)
	{
		if (strchr(MassiveBody[j], '{')) ++CounterOfBracket;
		if (strchr(MassiveBody[j], '}')) --CounterOfBracket;
		if (!CounterOfBracket)
			break;
		if (strstr(MassiveBody[j], VarName) && strchr(MassiveBody[j], '=') && strstr(MassiveBody[j], "rand") && CounterOfBracket == 1)
		{
			int IntValueRand = FindIfWithBreak(Files, Counter, 0, VarName, FractRand);
			int k = 0;
			for (int i = 0; i < strlen(MassiveBody[j]); ++i)
			{
				if (MassiveBody[j][i] == '(' || MassiveBody[j][i] == ')')
					++k;
			}
			if (k > 2 || strchr(MassiveBody[j], '*') || strchr(MassiveBody[j], '/') || strchr(MassiveBody[j], '.'))
				continue;
			else
			{
				int l = 0;
				int FlagOfNotEmptyBody = 0;
				char temp[100] = { 0 };
				k = strchr(MassiveBody[j], '(') - MassiveBody[j] + 1;
				while (MassiveBody[j][k] != ')')
				{
					temp[l] = MassiveBody[j][k];
					++l;
					++k;
				}
				for (int i = 0; i < l; ++i)
				{
					if (temp[i] != 0)
					{
						FlagOfNotEmptyBody = 1;
						break;
					}
				}
				if (FlagOfNotEmptyBody)
				{
					if (!CalculateMinMaxOfRand(MinBorderOfDiapozone, MaxBorderOfDiapozone, temp, l))
						continue;
				}
				int n = 0;
				++k;
				for (int b = 0; b < 100; ++b)
					temp[b] = 0;
				l = 0;
				while (MassiveBody[j][k] != ';')
				{
					temp[l] = MassiveBody[j][k];
					++l;
					++k;
				}
				if (!CalculateMinMaxOfRand(MinBorderOfDiapozone, MaxBorderOfDiapozone, temp, l))
					continue;
				if (0 <= *MaxBorderOfDiapozone && 0 >= *MinBorderOfDiapozone)
				{
					if (!Flag0)
					{
						++NumberOfAnswer;
						Flag0 = 1;
					}
				}
				float MainValueRand = IntValueRand;
				float FractValueRand = *FractRand;
				if (FractRand)
				{
					AddFractPart(&MainValueRand, FractValueRand);
				}
				if (MainValueRand <= *MaxBorderOfDiapozone && MainValueRand >= *MinBorderOfDiapozone)
				{
					if (!FlagIfBreak)
					{
						++NumberOfAnswer;
						FlagIfBreak = 1;
					}
				}
			}
		}
	}
	if (*MinBorderOfDiapozone == 0 && *MaxBorderOfDiapozone == RAND_MAX)
		return 0;
	else
		return NumberOfAnswer;
}