#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "correction.h" 

void put_tabs(FILE* out, int cnt_tabs)
{
	for (int i = 0; i < cnt_tabs; ++i)
		fprintf(out, "\t");
}

int check_type(char buf[], char type[50][40], int cnt)
{
	for (int i = 0; i < cnt; ++i)
		if (!(strcmp(buf, type[i])))
			return 0;
	return 1;
}

int str_fit(FILE* in, FILE* out, NODEE* com, char buf[], char type[][40], int cnt_types, int* cnt_tabs, int* chek, NODEE* stack)
{
	char tmp;
	int i = 0, f = 0, left = 0;

	if (feof(in) != 0)
		return -1;

	fscanf(in, "%c", &buf[i]);

	if (feof(in) != 0)
		return -1;

	while ((buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n') && !feof(in))
		fscanf(in, "%c", &buf[i]);

	if (buf[i] == '/')
	{
		f = i;
		fscanf(in, "%c", &buf[++i]);

		while ((buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n') && !feof(in))
			fscanf(in, "%c", &buf[i]);
		if (buf[i] == '*')
		{
			insert(&com, 1);
			buf[f] = ' ';
			buf[i] = ' ';

			while (com != NULL)
			{
				fscanf(in, "%c", &buf[++i]);
				if (buf[i] == '*')
				{
					f = i;
					fscanf(in, "%c", &buf[++i]);
					if (buf[i] == '/')
					{
						del(&com);
						if (com == NULL)
						{
							buf[f] = ' ';
							buf[i] = '\0';
						}
					}
				}
				else if (buf[i] == '/')
				{
					fscanf(in, "%c", &buf[++i]);
					if (buf[i] == '*')
						insert(&com, 1);
				}
			}
		}
		else
		{
			while ((buf[i] != '\n') && !feof(in))
				fscanf(in, "%c", &buf[++i]);
			buf[++i] = '\0';
			return 21;
		}

		while ((buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n') && !feof(in))
			fscanf(in, "%c", &buf[i]);

		return 20;
	}
	else if (buf[i] == '#') // 0
	{
		fscanf(in, "%c", &buf[++i]);

		while (buf[i] != '#' && buf[i] != '\n' && !feof(in))
			fscanf(in, "%c", &buf[++i]);

		buf[i] = '\0';

		if (!feof(in))
			fseek(in, -1, SEEK_CUR);

		return 0;
	}
	else if (buf[i] == '{' || buf[i] == '}')
	{
		if (buf[i] == '{')
		{
			*cnt_tabs += 1;
			return 16;
		}
		else
		{
			*cnt_tabs -= 1;
			if (stack != NULL && stack->next != NULL)
			{
				if (stack->next->type_flag == 6)
				{
					fscanf(in, "%c", &buf[++i]);
					while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
						fscanf(in, "%c", &buf[i]);

					while (buf[i] != ';')
						fscanf(in, "%c", &buf[++i]);

					buf[i + 1] = '\0';

					return 25;
				}
				else if (stack->next->type_flag == 1)
				{
					while (buf[i] != ';')
						fscanf(in, "%c", &buf[++i]);

					buf[i + 1] = '\0';

					return 18;
				}
				else if (stack->next->type_flag == 30)
				{
					while (buf[i] != ';')
						fscanf(in, "%c", &buf[++i]);

					buf[i + 1] = '\0';
				}
			}
			return 10;
		}
	}
	else
	{
		while ((buf[i] >= '0' && buf[i] <= '9') || (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || buf[i] == '.' && !feof(in))
			fscanf(in, "%c", &buf[++i]);

		tmp = buf[i];
		buf[i] = '\0';
		if (!feof(in))
			fseek(in, -1, SEEK_CUR);

		if (!(strcmp(buf, "typedef")) || !(strcmp(buf, "struct")) || !(strcmp(buf, "union")) || !(strcmp(buf, "enum"))) // 1
		{
			buf[i] = tmp;
				tmp = buf[0];
				while (buf[i] != '{' && !feof(in) && buf[i] != ';')
					fscanf(in, "%c", &buf[++i]);
				if (buf[i] == ';')
				{
					buf[i + 1] = '\0';
						return 3;
				}
			buf[i] = '\0';
			fseek(in, -1, SEEK_CUR);

			if (tmp == 'u' || tmp == 'e')
				return 30;

			return 1;
		}
		if (!(check_type(buf, type, cnt_types)))
		{
			buf[i] = tmp;
			while (buf[i] != '(' && buf[i] != ';' && !feof(in) && buf[i] != '=')
				fscanf(in, "%c", &buf[++i]);

			if (buf[i] == '(' && buf[i] != '=')
			{
				while (buf[i] != '{' && buf[i] != ';' && !feof(in))
					fscanf(in, "%c", &buf[++i]);
				if (buf[i] == '{')
				{
					buf[i] = '\0';
					fseek(in, -1, SEEK_CUR);
				}
				return 2;
			}

			while (buf[i] != ';' && !feof(in))
				fscanf(in, "%c", &buf[++i]);

			buf[++i] = '\0';
			return 3;
		}
		if (!(strcmp(buf, "while"))) // 4
		{
			fscanf(in, "%c", &buf[i]);
			while (buf[i] != '(' && !feof(in))
				fscanf(in, "%c", &buf[++i]);
			++left;

			while (left != 0)
			{
				fscanf(in, "%c", &buf[++i]);
				if (buf[i] == '(')
					++left;
				else if (buf[i] == ')')
					--left;
			}

			fscanf(in, "%c", &buf[++i]);
			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				fscanf(in, "%c", &buf[i]);



			if (buf[i] != ';')
			{
				fseek(in, -1, SEEK_CUR);
				buf[i] = '\0';
			}
			else
			{
				*chek = 1;
				buf[i + 1] = '\0';
			}

			return 4;
		}
		if (!(strcmp(buf, "for")))
		{
			int left = 0;

			fscanf(in, "%c", &buf[i]);
			while (buf[i] != '(' && !feof(in))
				fscanf(in, "%c", &buf[++i]);
			++left;

			while (left != 0)
			{
				fscanf(in, "%c", &buf[++i]);
				if (buf[i] == '(')
					++left;
				else if (buf[i] == ')')
					--left;
			}

			fscanf(in, "%c", &buf[++i]);
			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				fscanf(in, "%c", &buf[i]);

			if (buf[i] != ';')
			{
				fseek(in, -1, SEEK_CUR);
				buf[i] = '\0';
			}
			else
			{
				*chek = 1;
				buf[i + 1] = '\0';
			}

			return 5;
		}
		if (!(strcmp(buf, "do")))
			return 6;
		if (!(strcmp(buf, "if"))) // 4
		{
			fscanf(in, "%c", &buf[i]);
			while (buf[i] != '(' && !feof(in))
				fscanf(in, "%c", &buf[++i]);
			++left;

			while (left != 0 && !feof(in))
			{
				fscanf(in, "%c", &buf[++i]);
				if (buf[i] == '(')
					++left;
				else if (buf[i] == ')')
					--left;
			}
			return 4;
		}
		if (!(strcmp(buf, "else")))
		{
			fscanf(in, "%c", &buf[i]);
			++left;
			while ((buf[i] < 'a' || buf[i] > 'z') && !feof(in))
			{
				fscanf(in, "%c", &buf[++i]);
				++left;
			}
			if (buf[i] == 'i') // 4
			{
				fscanf(in, "%c", &buf[++i]);
				++left;
				if (buf[i] == 'f')
				{
					fscanf(in, "%c", &buf[++i]);
					++left;
					if (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
					{
						while (buf[i] != '(' && !feof(in))
							fscanf(in, "%c", &buf[i]);
						int c = 1;
						++left;

						while (c != 0 && !feof(in))
						{
							fscanf(in, "%c", &buf[++i]);
							if (buf[i] == '(')
								++c;
							else if (buf[i] == ')')
								--c;
							++left;
						}
						return 4;
					}
				}
			}
			fseek(in, -left, SEEK_CUR);
			return 9;
		}
		if (!(strcmp(buf, "switch"))) // 12
		{
			fscanf(in, "%c", &buf[i]);

			while (buf[i] != '{' && !feof(in))
				fscanf(in, "%c", &buf[++i]);

			buf[i] = '\0';
			fseek(in, -1, SEEK_CUR);
			return 4;
		}
		if (!(strcmp(buf, "case")))
		{
			fscanf(in, "%c", &buf[i]);

			while (buf[i] != ':' && !feof(in))
				fscanf(in, "%c", &buf[++i]);

			return 13;
		}
		if (!(strcmp(buf, "break"))) // 14
		{
			fscanf(in, "%c", &buf[i]);

			while (buf[i] != ';' && !feof(in))
				fscanf(in, "%c", &buf[++i]);

			return 14;
		}
		if (!(strcmp(buf, "return")))
		{
			fscanf(in, "%c", &buf[i]);

			while (buf[i] != ';' && !feof(in))
				fscanf(in, "%c", &buf[++i]);

			return 19;
		}
		fscanf(in, "%c", &buf[i]);
		while (buf[i] != '{' && buf[i] != ';' && !feof(in))
			fscanf(in, "%c", &buf[++i]);
		if (buf[i] == '{')
		{
			buf[i] = '\0';
			fseek(in, -1, SEEK_CUR);
		}
		else
			buf[++i] = '\0';
		return 15;
	}
}