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
//
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
//
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
//
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
////2
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
//2