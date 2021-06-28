#include "RibsGBFunctions.h"

bool belongsToTheWordArray(char* word, char wordArray[][10], unsigned wordsCnt)
{
	bool result = false;
	for (unsigned i = 0; i < wordsCnt; ++i)
	{
		if (!strcmp(word, wordArray[i]))
		{
			result = true;
			break;
		}
	}
	return result;
} // SHOULD BE OK

bool isEnlistedInPairList(char* reviewedCaller, char* reviewedCalled, CallPair ExaminatedPairList[], int listElemCount)
{
	bool result = false;
	for (unsigned i = 0; i < listElemCount; ++i)
	{
		if (!strcmp(ExaminatedPairList[i].Caller, reviewedCaller) && !strcmp(ExaminatedPairList[i].Called, reviewedCalled))
		{
			result = true;
			break;
		}
	}
	return result;
}

void EnlistMutuallyCalledHeaders(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, CallPair callPairList[MAX_CALL_LIST_SIZE],
								CallPair mutualCalls[MAX_CALL_LIST_SIZE], int *mutualCallsCnt)
{
	unsigned callOccasionCnt = 0;


	for (unsigned i = 0; i < amountOfFiles; ++i)
	{
		char filename[MAX_NAME_LEN] = { 0 };
		strcpy(filename, fileList[i]);

		char headerName[MAX_NAME_LEN] = { 0 };

		if (filename[strlen(filename) - 1] == 'h')
		{
			strcpy(headerName, filename);
		}

		if (strlen(headerName))
		{
			FILE_IN = fopen(headerName, "rt");

			char input[MAX_STR_LEN][MAX_STR_QUANTITY] = { 0 };
			unsigned strTotal = 0;

			while (!feof(FILE_IN))
			{
				fgets(input[strTotal++], MAX_STR_LEN, FILE_IN);
			}

			char wordlist[MAX_WORDS][MAX_WORD_LEN] = { 0 };
			unsigned int current_word = 0;
			unsigned int current_char = 0;

			// MAKING A WORDLIST 
			for (unsigned j = 0; j < strTotal; ++j)
			{
				for (unsigned k = 0; k < strlen(input[j]); ++k)
				{
					if (input[j][k] == ' ' || input[j][k] == '\n')
					{
						current_word++;
						current_char = 0;
					}
					else
					{
						wordlist[current_word][current_char++] = input[j][k];
					}
				}
			}
			// /MAKING WORDLIST

			for (unsigned j = 0; j < current_word; ++j)
			{
				if (!strcmp(wordlist[j], "#include") && strcmp(headerName, ""))
				{

					char calledHeaderName[MAX_WORD_LEN - 2] = { 0 };
					// ÇÀÏÈÑÛÂÀÅÌ ÈÌß ÂÛÇÂÀÍÍÎÃÎ ÔÀÉËÀ ÁÅÇ <ÑÊÎÁÎÊ>
					for (unsigned k = 1; k < strlen(wordlist[j + 1]) - 1; ++k)
					{
						calledHeaderName[k - 1] = wordlist[j + 1][k];
					}
					char Buff[MAX_WORD_LEN - 2] = { 0 };
					int k = 0;
					for (int i = 4; i < strlen(headerName); ++i)
					{
						Buff[k] = headerName[i];
						++k;
					}
					if (strcmp(calledHeaderName, Buff))
					{
						strcpy(callPairList[callOccasionCnt].Caller, Buff);
						strcpy(callPairList[callOccasionCnt].Called, calledHeaderName);
						callOccasionCnt++;
					}

				}
			}
		}
	}
	for (unsigned i = 0; i < callOccasionCnt; ++i)
	{
		for (unsigned j = 1; j < callOccasionCnt; ++j)
		{
			if (!strcmp(callPairList[i].Caller, callPairList[j].Called) && !strcmp(callPairList[i].Called, callPairList[j].Caller))
			{
				strcpy(mutualCalls[*mutualCallsCnt].Caller, callPairList[i].Caller);
				strcpy(mutualCalls[*mutualCallsCnt].Called, callPairList[i].Called);
				++*mutualCallsCnt;
			}
		}
	}
}

void DetectCustomFunction(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, CallPair customFuncList[MAX_FUNC_QUANTITY],
						  int* customFuncCnt)
{
	char arithmeticTypesList[ARITHMETIC_TYPES_CNT][10] =
	{
	  "void",
	  "int",
	  "float",
	  "double",
	  "char",
	  "char*",
	  "bool"
	};
	char optionalTypesList[OPTIONAL_TYPES_CNT][10] =
	{
	  "signed",
	  "unsigned",
	  "short",
	  "long"
	};
	for (unsigned i = 0; i < amountOfFiles; ++i)
	{
		char filename[MAX_NAME_LEN] = { 0 };
		strcpy(filename, fileList[i]);

		char codeFilename[MAX_NAME_LEN] = { 0 };

		if (filename[strlen(filename) - 1] == 'p' || filename[strlen(filename) - 1] == 'c')
		{
			strcpy(codeFilename, filename);
		}

		if (strlen(codeFilename))
		{
			FILE_IN = fopen(codeFilename, "rt");

			char input[MAX_STR_LEN][MAX_STR_QUANTITY] = { 0 };
			int strTotal = 0;

			while (!feof(FILE_IN))
			{
				fgets(input[strTotal], 100, FILE_IN);
				strTotal++;
			}

			char wordlist[MAX_WORDS][MAX_WORD_LEN] = { 0 };
			unsigned int current_word = 0;
			unsigned int current_char = 0;

			char funcTypeSpec[MAS_LENG][4] = { 0 };
			unsigned funcTypeSpecCnt = 0;
			char funcTypeStr[MAS_LENG] = { 0 };
			char funcName[MAS_LENG] = { 0 };

			bool typeListed = false;

			// MAKING A WORDLIST 
			for (unsigned j = 0; j < strTotal; ++j)
			{
				for (unsigned k = 0; k < strlen(input[j]); ++k)
				{
					if (input[j][k] == ' ' || input[j][k] == '\n')
					{
						current_word++;
						current_char = 0;
					}
					else
					{
						wordlist[current_word][current_char++] = input[j][k];
					}
				}
			}
			// /MAKING WORDLIST

			for (unsigned j = 0; j < current_word; ++j)
			{
				if (belongsToTheWordArray(wordlist[j], arithmeticTypesList, ARITHMETIC_TYPES_CNT) || belongsToTheWordArray(wordlist[j], optionalTypesList, OPTIONAL_TYPES_CNT))
				{
					strcpy(funcTypeSpec[funcTypeSpecCnt], wordlist[j]);
					++funcTypeSpecCnt;
				}

				if (funcTypeSpecCnt)
				{
					typeListed = true;
				}

				if (!(belongsToTheWordArray(wordlist[j], arithmeticTypesList, ARITHMETIC_TYPES_CNT) || belongsToTheWordArray(wordlist[j], optionalTypesList, OPTIONAL_TYPES_CNT))
					&&
					typeListed)
				{
					// INTEGRATING THE ARRAY INTO A STRING
					for (unsigned k = 0; k < funcTypeSpecCnt; ++k)
					{
						for (unsigned l = 0; l < strlen(funcTypeSpec[k]); ++l)
						{
							funcTypeStr[strlen(funcTypeStr)] = funcTypeSpec[k][l];
						}
						funcTypeStr[strlen(funcTypeStr)] = ' ';
					}
					// /INTEGRATING THE ARRAY INTO A STRING

					//strcpy(funcName, wordlist[j]);
					int leng = 0;
					if (strchr(wordlist[j], '('))
					{
						while (wordlist[j][leng] != '(')
						{
							funcName[leng] = wordlist[j][leng];
							++leng;
						}
						leng = 0;
						typeListed = false;
						for (int b = 0; b < MAS_LENG; ++b)
						{
							for (int v = 0; v < 4; ++v)
								funcTypeSpec[b][v] = 0;
						}
					}
					funcTypeSpecCnt = 0;
				}

				if (strlen(funcName) && strlen(funcTypeStr) && !isEnlistedInPairList(funcTypeStr, funcName, customFuncList, MAX_FUNC_QUANTITY))
				{
					strcpy(customFuncList[*customFuncCnt].Caller, funcTypeStr);
					strcpy(customFuncList[*customFuncCnt].Called, funcName);
					++*customFuncCnt;
				}
				for (int v = 0; v < MAS_LENG; ++v)
					funcName[v] = 0;
				for (int v = 0; v < MAS_LENG; ++v)
					funcTypeStr[v] = 0;
			}
		}
	}
	return;
}

void DetectCustomTypes(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, CallPair customTypesList[MAX_FUNC_QUANTITY],
	int *customTypesCnt)
{
	for (unsigned i = 0; i < amountOfFiles; ++i)
	{
		FILE_IN = fopen(fileList[i], "rt");

		char input[MAX_STR_LEN][MAX_STR_QUANTITY] = { 0 };
		unsigned strTotal = 0;

		while (!feof(FILE_IN))
		{
			fgets(input[strTotal++], 100, FILE_IN);
		}

		char wordlist[MAX_WORDS][MAX_WORD_LEN] = { 0 };
		unsigned int current_word = 0;
		unsigned int current_char = 0;

		// MAKING A WORDLIST 
		for (unsigned j = 0; j < strTotal; ++j)
		{
			for (unsigned k = 0; k < strlen(input[i]); ++k)
			{
				if (input[j][k] == ' ' || input[j][k] == '\n')
				{
					current_word++;
					current_char = 0;
				}
				else
				{
					wordlist[current_word][current_char++] = input[j][k];
				}
			}
		}
		// /MAKING WORDLIST

		char typeSpec[MAX_WORD_LEN] = { 0 };
		char typeName[MAX_WORD_LEN] = { 0 };
		for (unsigned j = 0; j < current_word; ++j)
		{
			if (!strcmp(wordlist[j], "typedef"))
			{
				if (!strcmp(wordlist[j + 1], "struct") || !strcmp(wordlist[j + 1], "union") || !strcmp(wordlist[j + 1], "enum"))
				{
					strcpy(typeSpec, wordlist[j + 1]);
					strcpy(typeName, wordlist[j + 2]);
				}
				else
				{
					strcpy(typeSpec, wordlist[j + 2]);
					strcpy(typeName, wordlist[j + 1]);
				}
			}
			else if (!strcmp(wordlist[j], "struct") || !strcmp(wordlist[j], "union") || !strcmp(wordlist[j], "enum"))
			{
				strcpy(typeSpec, wordlist[j]);
				strcpy(typeName, wordlist[j + 1]);
			}
			if (strlen(typeSpec) && strlen(typeName))
			{
				strcpy(customTypesList[*customTypesCnt].Caller, typeSpec);
				strcpy(customTypesList[*customTypesCnt].Called, typeName);
				++*customTypesCnt;
				strcpy(typeSpec, "");
				strcpy(typeName, "");
				//MAKE STATS
			}
		}
	}
}

void CountMaxLoopDepth(FILE* FILE_IN, char fileList[][MAX_NAME_LEN], unsigned amountOfFiles, int* maxTabs)
{
	for (unsigned i = 0; i < amountOfFiles; ++i)
	{
		char filename[MAX_NAME_LEN] = { 0 };
		strcpy(filename, fileList[i]);
		int subtractor = 0;
		char codeFilename[MAX_NAME_LEN] = { 0 };

		if (filename[strlen(filename) - 1] == 'p' || filename[strlen(filename) - 1] == 'c')
		{
			strcpy(codeFilename, filename);
		}

		if (strlen(codeFilename))
		{
			FILE_IN = fopen(codeFilename, "r");

			char input[MAX_STR_LEN][MAX_STR_QUANTITY] = { 0 };
			unsigned strTotal = 0;

			while (!feof(FILE_IN))
			{
				fgets(input[strTotal], 100, FILE_IN);
				++strTotal;
			}
			if (strTotal)
			{
				bool isLoopPresent = false;
				int tabsEncountered = 0;
				for (unsigned currentString = 0; currentString < strTotal; ++currentString)
				{
					tabsEncountered = 0;
					if (currentString == 247)
						int a = 0;
					if (strstr(input[currentString], "while") || strstr(input[currentString], "for") || strstr(input[currentString], "do"))
					{
						unsigned crutchChar = 0;
						for (unsigned currentChar = 0; currentChar < strlen(input[currentString]); ++currentChar)
						{
							crutchChar = currentChar;
							if (input[currentString][currentChar] == '\t')
							{
								++tabsEncountered;
							}
						}
					}
					if (!strcmp(input[currentString], "{\n"))
					{
						subtractor = 1;
					}
					if (!strcmp(input[currentString], "}\n"))
					{
						subtractor = 0;
					}
					tabsEncountered -= subtractor;
					if (*maxTabs < tabsEncountered)
					{
						*maxTabs = tabsEncountered;
					}
				}
			}
		}
	}
}