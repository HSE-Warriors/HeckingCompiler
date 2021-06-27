#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "correction.h" 

void insert(NODEE** head, int flag)
{
	NODEE* tmp = (NODEE*)malloc(sizeof(NODEE));
	NODEE* new_next;

	if (*head == NULL)
	{
		tmp->type_flag = flag;
		tmp->next = NULL;
		*head = tmp;
	}
	else
	{
		tmp->next = *head;
		tmp->type_flag = flag;
		*head = tmp;
	}
}

int del(NODEE** head)
{
	int flag;
	if (*head == NULL)
		return -1;
	NODEE* new_head = (*head)->next;
	flag = (*head)->type_flag;
	free(*head);
	*head = new_head;
	return flag;
}

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

int chek_dirs(char buf[], char type[50][40], int cnt)
{
	for (int i = 0; i < cnt; ++i)
		if (!(strcmp(buf, &type[i][4])))
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

void dir_correction(FILE* out, char buf[], char dir[50][40], int* cnt_dir)
{
	int i = 1, j = 0;

	char file[200];
	char* old_file = &file[4];
	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	fprintf(out, "%c", buf[0]);

	while (buf[i] == ' ' || buf[i] == '\t')
		++i;

	if (buf[i + 2] == 'd')
	{
		for (j = 0; j < i; ++j)
			buf[j] = '\0';
		return;
	}

	if (buf[i + 1] == 'n')
	{
		while (buf[i] >= 'A' && buf[i] <= 'z')
			fprintf(out, "%c", buf[i++]);

		fprintf(out, " ");

		while (buf[i] == ' ' || buf[i] == '\t')
			++i;

		if (buf[i] == '\"')
		{
			strcpy(file, "New_");

			j = 4;

			fprintf(out, "%c", buf[i]);
			++i;

			while (buf[i] == ' ' || buf[i] == '\t')
				++i;

			while ((buf[i] >= 'A' && buf[i] <= 'z') || buf[i] == '.' || (buf[i] >= '0' && buf[i] <= '9'))
			{
				file[j++] = buf[i];
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == ' ' || buf[i] == '\t')
					++i;
			}
			file[j] = '\0';

			if (chek_dirs(old_file, dir, *cnt_dir - 1))
			{
				strcpy(dir[*cnt_dir], file);
				old_file = &file[j - 1];
				strcpy(old_file, "cpp\0");
				strcpy(dir[*cnt_dir + 1], file);
				*cnt_dir += 2;
			}

			while (buf[i] == ' ' || buf[i] == '\t')
				++i;

			fprintf(out, "%c", buf[i]);
		}
		else
		{
			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t')
				++i;

			while ((buf[i] >= 'A' && buf[i] <= 'z') || buf[i] == '.')
			{
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == ' ' || buf[i] == '\t')
					++i;
			}

			fprintf(out, ">");
		}
	}
	else if (buf[i] == 'p')
	{
		while (buf[i] >= 'A' && buf[i] <= 'z')
			fprintf(out, "%c", buf[i++]);

		while (buf[i] == ' ' || buf[i] == '\t')
			++i;

		fprintf(out, " ");

		while (buf[i] >= 'A' && buf[i] <= 'z')
			fprintf(out, "%c", buf[i++]);
	}
	else
	{
		while (buf[i] != ' ' && buf[i] != '\t')
			fprintf(out, "%c", buf[i++]);

		fprintf(out, " ");

		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		while ((buf[i] >= 'A' && buf[i] <= 'z') || buf[i] == '.')
			fprintf(out, "%c", buf[i++]);

		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		if (buf[i] != '\0')
			fprintf(out, " ");

		while ((buf[i] >= 'A' && buf[i] <= 'z') || buf[i] == '.' || (buf[i] >= '0' && buf[i] <= '9'))
			fprintf(out, "%c", buf[i++]);
	}

	if (com[0] != '\0')
		fprintf(out, " // %s", com);

	fprintf(out, "\n");

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void struct_correction(FILE* out, char buf[], char type[50][40], int cnt_types)
{
	int i = 0, j = 0;

	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i] == 't')
	{
		while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
			fprintf(out, "%c", buf[i++]);

		fprintf(out, " ");

		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;
	}

	while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
		fprintf(out, "%c", buf[i++]);

	fprintf(out, " ");

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
	{
		type[cnt_types][j++] = buf[i];
		fprintf(out, "%c", buf[i++]);
	}
	type[cnt_types][j] = '\0';

	if (com[0] != '\0')
		fprintf(out, " // %s", com);

	fprintf(out, "\n");

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void func_correction(FILE* out, char buf[])
{
	int i = 0, j = 0;

	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
		++i;

	while (buf[i] != '\n' && buf[i] != ' ' && buf[i] != '\t')
		fprintf(out, "%c", buf[i++]);

	while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
		++i;

	fprintf(out, " ", buf[i]);

	while (buf[i] != '\n' && buf[i] != ' ' && buf[i] != '\t' && buf[i] != '(')
		fprintf(out, "%c", buf[i++]);

	while (buf[i] != '(')
		++i;

	fprintf(out, "(", buf[i++]);
	++j;

	while (1)
	{
		while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
			++i;

		if (buf[i] == ')')
		{
			fprintf(out, "%c", buf[i++]);
			break;
		}

		while (buf[i] != '\n' && buf[i] != ' ' && buf[i] != '\t' && buf[i] != ')')
			fprintf(out, "%c", buf[i++]);

		while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
			++i;

		while ((buf[i] < 'A' || buf[i] > 'z') && buf[i] != ')')
		{
			fprintf(out, "%c", buf[i++]);

			while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
				++i;

			++j;
		}

		if (j >= 0)
		{
			j = 0;
			if(buf[i] != ')')
				fprintf(out, " ");
		}

		while (buf[i] != '\n' && buf[i] != ' ' && buf[i] != '\t' && buf[i] != '[' && buf[i] != ',' && buf[i] != ')')
			fprintf(out, "%c", buf[i++]);

		if (buf[i] == '[')
		{
			fprintf(out, "[");

			++i;
			j = 1;
			while (j != 0)
			{
				while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
					++i;

				while (buf[i] >= '0' && buf[i] <= '9')
					fprintf(out, "%c", buf[i++]);

				while (buf[i++] != ']');
				--j;

				fprintf(out, "]");

				while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
					++i;

				if (buf[i] == '[')
				{
					fprintf(out, "[");
					++j;
					++i;
				}
			}
		}

		while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
			++i;

		if (buf[i] == ',')
			fprintf(out, "%c ", buf[i++]);
	}

	while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
		++i;

	if (buf[i] == ';')
		fprintf(out, ";", buf[i]);

	if (com[0] != '\0')
		fprintf(out, " // %s", com);

	fprintf(out, "\n");

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void var_correction(FILE* out, char buf[])
{
	char str[200];
	char chek[200];
	int i = 0, j = 0, cnt_stars = 0, k = 0;

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n' && buf[i] != '*')
	{
		chek[k++] = buf[i];
		fprintf(out, "%c", buf[i++]);
	}

	chek[k] = '\0';

	if (!(strcmp(chek, "struct")) || !(strcmp(chek, "union")) || !(strcmp(chek, "enum")))
	{
		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n' && buf[i] != '*')
			fprintf(out, "%c", buf[i++]);
	}

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i] == '*')
	{
		cnt_stars = 1;

		while (1)
		{
			++i;
			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] == '*')
				++cnt_stars;

			if ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
				break;
		}

		while (--cnt_stars >= 0)
			fprintf(out, "*");
	}

	fprintf(out, " ");

	while (buf[i] != ';')
		str[j++] = buf[i++];

	str[j++] = buf[i++];

	str_correction(out, str);

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void condition_correction(FILE* out, char buf[], int* chek)
{
	int i = 0, j = 0, k = 0, f1 = 0, f2 = 0, o = 0;

	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n' && buf[i] != '(')
		fprintf(out, "%c", buf[i++]);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	fprintf(out, " %c", buf[i++]);
	++j;

	while (j != 0)
	{
		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		if (buf[i] == '(')
		{
			fprintf(out, "%c", buf[i]);
			++j;
			++i;
		}
		else if (buf[i] == ')')
		{
			fprintf(out, "%c", buf[i]);
			++i;
			--j;
		}
		else if (buf[i] == '!')
		{
			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] == '(')
			{
				fprintf(out, "%c", buf[i++]);
				++j;
			}

			if (buf[i] == '=')
				fprintf(out, "%c", buf[i++]);
		}
		else if (buf[i] == '[')
			fprintf(out, "%c", buf[i++]);
		else if (buf[i] == ']')
		{
			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] != '[' && buf[i] != ',' && buf[i] != ']' && buf[i] != ')')
				fprintf(out, " ");
		}
		else if (buf[i] == '\'' || buf[i] == '\"')
		{
			fprintf(out, "%c", buf[i++]);
			while (buf[i] != '\'' && buf[i] != '\"')
			{
				while (buf[i] == '\t' || buf[i] == '\n')
					++i;

				while (buf[i] != '\t' && buf[i] != '\n' && buf[i] != '\'' && buf[i] != '\"')
					fprintf(out, "%c", buf[i++]);
			}

			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] != ')' && buf[i] != '|' && buf[i] != '&')
				fprintf(out, " ");
		}
		else
		{
			if (buf[i] == '*')
			{
				k = i - 1;
				f1 = 0;
				while (buf[k] == ' ' || buf[k] == '\t' || buf[k] == '\n')
					--k;

				if (buf[k] == '=' || buf[k] == '+' || buf[k] == '-' || buf[k] == '/' || buf[k] == '+' || buf[k] == ',' || buf[k] == '(' || buf[k] == '[')
				{
					while (1)
					{
						++f1;
						while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
							++i;

						if (buf[i] != '*')
							break;

						++i;
					}

					while (--f1 > 0)
						fprintf(out, "*");
				}
				else
				{
					fprintf(out, "%c", buf[i++]);

					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);

					fprintf(out, "%c ", buf[i++]);
				}
			}
			else if (buf[i] == '+')
			{
				k = 1;
				while (1)
				{
					++i;
					while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
						++i;

					if (buf[i] != '+')
						break;

					++k;
				}

				if (k == 2)
				{
					if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',' || buf[i] == '*' || (buf[i] >= 'A' && buf[i] <= 'z'))
						fprintf(out, "++");
					else
						fprintf(out, "++ ");
				}
				else
				{
					fprintf(out, " +");
					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);
					fprintf(out, " ");
				}
			}
			else if (buf[i] == '/')
			{
				fprintf(out, " %c ", buf[i++]);
				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;
			}
			else if (buf[i] == '-')
			{
				o = i;
				k = 1;
				while (1)
				{
					++i;
					while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
						++i;

					if (buf[i] != '-')
						break;
					++k;
				}

				if (k == 2)
				{
					if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',' || buf[i] == '*' || (buf[i] >= 'A' && buf[i] <= 'z'))
						fprintf(out, "--");
					else
						fprintf(out, "-- ");
				}
				else
				{
					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);
					else
					{
						k = i - 1;
						while ((buf[k] == ' ' || buf[k] == '\t' || buf[k] == '\n'))
							--k;
						if ((buf[k] == '[' || buf[k] == '(' || buf[k] == '='))
							fprintf(out, "-");
						else
							fprintf(out, "- ");
					}
				}
			}
			else if (buf[i] == '&' && buf[i + 1] == '&' || buf[i] == '|')
			{
				fprintf(out, " %c%c ", buf[i], buf[i]);
				i += 2;
			}
			else if (buf[i] == '&')
				fprintf(out, "%c", buf[i++]);
			else if ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
			{
				while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9') || buf[i] == '.')
					fprintf(out, "%c", buf[i++]);

				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;

				if (buf[i] == '+')
				{
					k = 0;
					while (1)
					{
						++i;
						while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
							++i;

						if (buf[i] != '+')
							break;

						++k;
					}

					if (k == 2)
					{
						if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',')
							fprintf(out, "++");
						else
							fprintf(out, "++ ");
					}
					else
					{
						fprintf(out, " +");
						if (buf[i] == '=')
							fprintf(out, "%c", buf[i++]);
						fprintf(out, " ");
					}
				}
				if (buf[i] == '-')
				{
					k = 0;
					while (1)
					{
						++i;
						while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
							++i;

						if (buf[i] != '-')
							break;

						++k;
					}

					if (k == 2)
					{
						if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',')
							fprintf(out, "--");
						else
							fprintf(out, "-- ");
					}
					else
					{
						fprintf(out, " -");
						if (buf[i] == '=')
							fprintf(out, "%c", buf[i++]);
						fprintf(out, " ");
					}
				}
				if (buf[i] == '&' || buf[i] == '|')
				{
					fprintf(out, " %c%c ", buf[i], buf[i]);
					i += 2;
				}
				if (buf[i] == ',')
					fprintf(out, "%c ", buf[i++]);
				else if (buf[i] == '=')
				{
					fprintf(out, " %c", buf[i++]);

					while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
						++i;

					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);

					fprintf(out, " ");
				}
				else if (buf[i] == '<' || buf[i] == '>')
				{
					fprintf(out, " %c", buf[i++]);

					while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
						++i;

					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);

					fprintf(out, " ");
				}
			}
			else if (buf[i] == '=')
			{
				k = 1;
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
					++i;

				if (buf[i] == '=')
					fprintf(out, "%c", buf[i++]);

				fprintf(out, " ");
			}
			else if (buf[i] == '<' || buf[i] == '>')
			{
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
					++i;

				if (buf[i] == '=')
					fprintf(out, "%c", buf[i++]);

				fprintf(out, " ");
			}
		}
	}

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i] == ';')
	{
		fprintf(out, ";");
		*chek = 1;
	}

	if (com[0] != '\0')
		fprintf(out, " // %s", com);

	fprintf(out, "\n");

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void for_correction(FILE* out, char buf[], int* chek)
{
	int i = 0, j = 0;

	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	while (buf[i] != '(')
		++i;

	fprintf(out, "for %c", buf[i++]);

	while (buf[i] != ';')
	{
		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
			fprintf(out, "%c", buf[i++]);

		fprintf(out, " ");

		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		if (buf[i] == '=')
		{
			while (!((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9')))
			{
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;
			}

			fprintf(out, " ");
		}

		while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
			fprintf(out, "%c", buf[i++]);

		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;
	}

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i + 1] == ';')
	{
		fprintf(out, "%c", buf[i++]);
	}
	else
		fprintf(out, "%c ", buf[i++]);

	while (buf[i] != ';')
	{
		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
			fprintf(out, "%c", buf[i++]);

		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		if (!((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9')) && buf[i] != ';')
		{
			fprintf(out, " ");

			while (!((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9')) && buf[i] != ';')
			{
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;
			}

			fprintf(out, " ");
		}

		while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
			fprintf(out, "%c", buf[i++]);

		if (buf[i] == '[')
		{
			while (buf[i] != ']')
			{
				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;

				while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
					fprintf(out, "%c", buf[i++]);
			}

			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] == '[')
			{
				while (buf[i] != ']')
				{
					while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
						++i;

					while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
						fprintf(out, "%c", buf[i++]);
				}

				fprintf(out, "%c", buf[i++]);

				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;
			}
		}
	}

	++i;
	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i] != ')')
		fprintf(out, "; ", buf[i]);
	else
		fprintf(out, ";", buf[i]);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	while (buf[i] != ')')
	{
		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		if (buf[i] == '+' || buf[i] == '-')
		{
			fprintf(out, " %c%c", buf[i], buf[i]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n' || buf[i] == '+' || buf[i] == '-')
				++i;
		}
		else
		{
			j = i + 1;

			while (buf[j] == ' ' || buf[j] == '\t' || buf[j] == '\n')
				++j;

			if (buf[i] == buf[j])
				fprintf(out, "%c%c ", buf[i], buf[i]);
			else
				fprintf(out, " %c= ", buf[i]);

			i = j + 1;

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
				fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;
		}

		while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
			fprintf(out, "%c", buf[i++]);

		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;
	}

	fprintf(out, ")");

	++i;

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i] == ';')
	{
		fprintf(out, ";");
		*chek = 1;
	}

	if (com[0] != '\0')
		fprintf(out, " // %s", com);

	fprintf(out, "\n");

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void case_correction(FILE* out, char buf[])
{
	int i = 0, j = 0;

	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	while ((buf[i] >= 'A' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
		fprintf(out, "%c", buf[i++]);

	fprintf(out, " ");

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	while ((buf[i] >= 'A' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
		fprintf(out, "%c", buf[i++]);

	fprintf(out, " ");

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	fprintf(out, "%c", buf[i]);

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void big_com_correction(FILE* out, char buf[], int cnt)
{
	int i = 0;

	while (buf[i] != '\0')
	{
		put_tabs(out, cnt);
		fprintf(out, "//");

		while (buf[i] != '\n' && buf[i] != '\0')
			fprintf(out, "%c", buf[i++]);

		if (buf[i] == '\n')
			fprintf(out, "\n");

		while (buf[i] == '\n')
			++i;
	}

	for (int j = 0; j < i; ++j)
		buf[j] = '\0';
}

void small_com_correction(char buf[], char com[])
{
	int i = 0, j = 0, k = 0;
	while (1)
	{
		while (buf[i] != '/' && buf[i] != '\0')
			++i;

		if (buf[i] == '/')
		{
			k = i++;
			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] == '*')
			{
				buf[k] = ' ';
				buf[i++] = ' ';

				while (1)
				{
					while (buf[i] == ' ' || buf[i] == '\t')
					{
						com[j++] = buf[i];
						buf[i++] = ' ';
					}

					if (buf[i] == '*')
					{
						k = i++;
						while (buf[i] == ' ' || buf[i] == '\t')
						{
							com[j++] = buf[i];
							buf[i++] = ' ';
						}

						if (buf[i] == '/')
						{
							buf[k] = ' ';
							buf[i] = ' ';
							com[j] = '\0';
							break;
						}

						if (buf[i] != '\n')
							com[j++] = buf[i];
						buf[i++] = ' ';
					}
					else
					{
						if (buf[i] != '\n')
						{
							com[j++] = buf[i];
						}
						buf[i++] = ' ';
					}
				}
			}
		}
		else if (buf[i++] == '\0')
			break;
	}
}

void break_correction(FILE* out, char buf[])
{
	fprintf(out, "break;\n");

	for (int j = 0; j < 20; ++j)
		buf[j] = '\0';
}

void str_correction(FILE* out, char buf[])
{
	int i = 0, j = 0, k = 0, f1 = 0, f2 = 0, o = 0;

	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	while (1)
	{
		while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			++i;

		if (buf[i] == ';')
		{
			fprintf(out, "%c", buf[i]);
			break;
		}
		else if (buf[i] == '{')
		{
			fprintf(out, "{ ");
			++i;
			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			while (buf[i] != '}' && buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
				fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			fprintf(out, " ");

			fprintf(out, "%c", buf[i++]);
		}
		else if (buf[i] == ',')
			fprintf(out, "%c ", buf[i++]);
		else if (buf[i] == '(')
			fprintf(out, "%c", buf[i++]);
		else if (buf[i] == ')')
			fprintf(out, "%c", buf[i++]);
		else if (buf[i] == '!')
		{
			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] == '(')
			{
				fprintf(out, "%c", buf[i++]);
				++j;
			}

			if (buf[i] == '=')
				fprintf(out, "%c", buf[i++]);
		}
		else if (buf[i] == '[')
			fprintf(out, "%c", buf[i++]);
		else if (buf[i] == ']')
		{
			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] != '[' && buf[i] != ',' && buf[i] != ']' && buf[i] != ')' && buf[i] != ';')
				fprintf(out, " ");
		}
		else if (buf[i] == '\'' || buf[i] == '\"')
		{
			fprintf(out, "%c", buf[i++]);
			while (buf[i] != '\'' && buf[i] != '\"')
			{
				while (buf[i] == '\t' || buf[i] == '\n')
					++i;

				while (buf[i] != '\t' && buf[i] != '\n' && buf[i] != '\'' && buf[i] != '\"')
					fprintf(out, "%c", buf[i++]);
			}

			fprintf(out, "%c", buf[i++]);

			while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
				++i;

			if (buf[i] != ')' && buf[i] != '|' && buf[i] != '&' && buf[i] != ';' && buf[i] != ',')
				fprintf(out, " ");
		}
		else
		{
			if (buf[i] == '*')
			{
				k = i - 1;
				f1 = 0;
				while (buf[k] == ' ' || buf[k] == '\t' || buf[k] == '\n')
					--k;

				if (buf[k] == '=' || buf[k] == '+' || buf[k] == '-' || buf[k] == '/' || buf[k] == '+' || buf[k] == ',' || buf[k] == '(' || buf[k] == '[')
				{
					while (1)
					{
						++f1;
						while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
							++i;

						if (buf[i] != '*')
							break;

						++i;
					}

					while (--f1 > 0)
						fprintf(out, "*");
				}
				else
				{
					fprintf(out, "%c", buf[i++]);

					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);

					fprintf(out, "%c ", buf[i++]);
				}
			}
			else if (buf[i] == '+')
			{
				o = i;
				k = 1;
				while (1)
				{
					++i;
					while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
						++i;

					if (buf[i] != '+')
						break;

					++k;
				}

				if (k == 2)
				{
					if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',' || buf[i] == '*' || (buf[i] >= 'A' && buf[i] <= 'z'))
						fprintf(out, "++");
					else
						fprintf(out, "++ ");
				}
				else
				{
					fprintf(out, " +");
					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);
					while (o != 0 && (buf[o] == ' ' || buf[o] == '\t' || buf[o] == '\n'))
						--o;
					if (!(o == 0 && (buf[o] == ' ' || buf[o] == '\t' || buf[o] == '\n')))
						fprintf(out, " ");
				}
			}
			else if (buf[i] == '/')
			{
				fprintf(out, " %c ", buf[i++]);
				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;
			}
			else if (buf[i] == '-')
			{
				o = i;
				k = 1;
				while (1)
				{
					++i;
					while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
						++i;

					if (buf[i] != '-')
						break;
					++k;
				}

				if (k == 2)
				{
					if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',' || buf[i] == '*' || (buf[i] >= 'A' && buf[i] <= 'z'))
						fprintf(out, "--");
					else
						fprintf(out, "-- ");
				}
				else
				{
					fprintf(out, "-");
					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);
					while ((buf[o] == ' ' || buf[o] == '\t' || buf[o] == '\n') && buf[o] != 0)
						--o;
					if (buf[o] == '(' || buf[o] == '[' || buf[o] == '=')
					{

					}
					else if (buf[o] == ' ' || buf[o] == '\t' || buf[o] == '\n' || buf[o] == '-')
					{

					}
					else
						fprintf(out, " ");
				}
			}
			else if (buf[i] == '&' && buf[i + 1] == '&' || buf[i] == '|')
			{
				fprintf(out, " %c%c ", buf[i], buf[i]);
				i += 2;
			}
			else if (buf[i] == '&')
				fprintf(out, "%c", buf[i++]);
			else if (buf[i] == '%')
				fprintf(out, " %c ", buf[i++]);
			else if ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
			{
				while ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9') || buf[i] == '_' || buf[i] == '.')
					fprintf(out, "%c", buf[i++]);

				while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
					++i;

				if (buf[i] == '+')
				{
					k = 1;
					while (1)
					{
						++i;
						while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
							++i;

						if (buf[i] != '+')
							break;

						++k;
					}

					if (k == 2)
					{
						if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',' || buf[i] == ';')
							fprintf(out, "++");
						else
							fprintf(out, "++ ");
					}
					else
					{
						fprintf(out, " +");
						if (buf[i] == '=')
							fprintf(out, "%c", buf[i++]);
						fprintf(out, " ");
					}
				}
				if (buf[i] == '-')
				{
					k = 1;
					while (1)
					{
						++i;
						while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
							++i;

						if (buf[i] != '-')
							break;

						++k;
					}

					if (k == 2)
					{
						if (buf[i] == ']' || buf[i] == ')' || buf[i] == ',' || buf[i] == ';')
							fprintf(out, "--");
						else
							fprintf(out, "-- ");
					}
					else
					{
						fprintf(out, " -");
						if (buf[i] == '=')
							fprintf(out, "%c", buf[i++]);
						fprintf(out, " ");
					}
				}
				if (buf[i] == '&' || buf[i] == '|')
				{
					fprintf(out, " %c%c ", buf[i], buf[i]);
					i += 2;
				}
				if (buf[i] == ',')
					fprintf(out, "%c ", buf[i++]);
				else if (buf[i] == '=')
				{
					k = 1;
					fprintf(out, " %c", buf[i++]);

					while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
						++i;

					if (buf[i] == '=')
					{
						fprintf(out, "%c", buf[i++]);
					}

					fprintf(out, " ");
				}
				else if (buf[i] == '<' || buf[i] == '>')
				{
					fprintf(out, " %c", buf[i++]);

					while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
						++i;

					if (buf[i] == '=')
						fprintf(out, "%c", buf[i++]);

					fprintf(out, " ");
				}
			}
			else if (buf[i] == '=')
			{
				k = 1;
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
					++i;

				if (buf[i] == '=')
					fprintf(out, "%c", buf[i++]);

				fprintf(out, " ");
			}
			else if (buf[i] == '<' || buf[i] == '>')
			{
				fprintf(out, "%c", buf[i++]);

				while (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')
					++i;

				if (buf[i] == '=')
					fprintf(out, "%c", buf[i++]);

				fprintf(out, " ");
			}
			else if (buf[i] == '%')
				fprintf(out, " %c ", buf[i++]);
		}
	}

	if (com[0] != '\0')
		fprintf(out, " // %s", com);

	fprintf(out, "\n");

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void return_correction(FILE* out, char buf[])
{
	int i = 0, j = 0;
	char str[200];

	char com[200];

	com[0] = '\0';

	small_com_correction(buf, com);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	while ((buf[i] >= 'A' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9'))
		fprintf(out, "%c", buf[i++]);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i] == ';')
	{
		fprintf(out, ";\n");
	}
	else
	{
		fprintf(out, " ");

		while (buf[i] != ';')
			str[j++] = buf[i++];

		str[j++] = buf[i++];

		str_correction(out, str);
	}

	for (j = 0; j < i; ++j)
		buf[j] = '\0';
}

void put_left_bracket(FILE* out, char buf[])
{
	int i = 0;

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	fprintf(out, "{\n");

	for (int j = 0; j < i; ++j)
		buf[j] = '\0';
}

void put_right_bracket(FILE* out, char buf[])
{
	int i = 0;

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	fprintf(out, "}");

	++i;

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	if (buf[i] == ';')
		fprintf(out, ";");

	fprintf(out, "\n");

	for (int j = 0; j < i; ++j)
		buf[j] = '\0';
}

void after_do_correction(FILE* out, char buf[])
{
	int i = 0, j = 0;

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	fprintf(out, "%c ", buf[i++]);

	while (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		++i;

	char* str = &buf[i];

	condition_correction(out, str, &j);

	for (j = 0; j < 21; ++j)
		buf[i] = '\0';
}

void main_correction(char dirs[50][40], int* cnt_d)
{
	NODEE* stack = NULL;
	NODEE* comment = NULL;

	char types[50][40];
	char* main_file = &dirs[0][4];
	char strin[200];

	int cnt_tabs = 0, flag, chek = 0, cnt = 7;

	strcpy(types[0], "void");
	strcpy(types[1], "int");
	strcpy(types[2], "char");
	strcpy(types[3], "main");
	strcpy(types[4], "longint");
	strcpy(types[5], "double");
	strcpy(types[6], "float");

	strcpy(dirs[0], "New_");

	FILE* in;
	FILE* out;
	int flag_F = 0;

	do {
		printf("Enter the name of project's main file or type \x1b[33m\"help\"\x1b[0m:\n");
		scanf("%s", main_file);
		//fgets(main_file, sizeof main_file, stdin); //Внезапно оно решило считывать перенос строки
		//printf("Got \"%s\":\n", main_file);

		if (strstr(main_file, "help"))
		{
			showHelp();
			flag_F = 1;
		}
		if (strstr(main_file, "exit"))
		{
			exitApp();
		}
		if (strstr(main_file, "list") || strstr(main_file, "ls"))
		{
			showFiles();
			flag_F = 1;
		}

		in = fopen(main_file, "r");

		if (in == NULL)
		{
			if (flag_F != 1)
			{
				unluckee();
			}
		}
	} while (in == NULL);

	out = fopen(dirs[0], "w");

	while (1)
	{
		flag = str_fit(in, out, comment, strin, types, cnt, &cnt_tabs, &chek, stack);

		if (stack != NULL && (stack->type_flag == 4 || stack->type_flag == 5) && flag != 16 && chek == 0)
		{
			del(&stack);
			put_tabs(out, cnt_tabs + 1);
			chek = 0;
		}
		else if (flag == 10)
		{
			del(&stack);
			del(&stack);
			put_tabs(out, cnt_tabs);
		}
		else if (flag == 16)
		{
			put_tabs(out, cnt_tabs - 1);
		}
		else if(flag != 20)
			put_tabs(out, cnt_tabs);

		if (flag == 4 || flag == 5 || flag == 16 || flag == 6 || flag == 30)
			insert(&stack, flag);

		if (strin[0] == '\0')
			break;
		switch (flag)
		{
		case 0:
			dir_correction(out, strin, dirs, cnt_d);
			break;

		case 1:
			fprintf(out, "\n");
			struct_correction(out, strin, types, cnt++);
			break;

		case 2:
			fprintf(out, "\n");
			func_correction(out, strin);
			break;

		case 3:
			var_correction(out, strin);
			break;

		case 4:
			condition_correction(out, strin, &chek);
			break;

		case 5:
			for_correction(out, strin, &chek);
			break;

		case 6:
			fprintf(out, "do\n");
			for (int j = 0; j < 21; ++j)
				strin[j] = '\0';
			break;

		case 10:
			put_right_bracket(out, strin);
			break;

		case 12:
			condition_correction(out, strin, &chek);
			break;

		case 13:
			case_correction(out, strin);
			break;

		case 14:
			break_correction(out, strin);
			break;

		case 15:
			str_correction(out, strin);
			break;

		case 16:
			put_left_bracket(out, strin);
			break;

		case 18:
			after_struct_correction(out, strin, types, cnt++);
			break;

		case 19:
			return_correction(out, strin);
			break;

		case 20:
			big_com_correction(out, strin, cnt_tabs);
			fprintf(out, "\n");
			break;

		case 21:
			fprintf(out, "%s", strin);
			break;

		case 25:
			after_do_correction(out, strin);
			break;

		case 30:
			fprintf(out, "\n");
			struct_correction(out, strin, types, cnt++);
			break;

		default:
			for (int j = 0; j < 21; ++j)
				strin[j] = '\0';
			break;
		}
	}

	fclose(in);
	fclose(out);

	for (int i = 1; i < *cnt_d; ++i) // хедеры
	{
		cnt_tabs = 0, flag;

		FILE* in = fopen(&dirs[i][4], "r");
		FILE* out = fopen(dirs[i], "w");

		while (1)
		{
			flag = str_fit(in, out, comment, strin, types, cnt, &cnt_tabs, &chek, stack);

			if (stack != NULL && (stack->type_flag == 4 || stack->type_flag == 5) && flag != 16 && chek == 0)
			{
				del(&stack);
				put_tabs(out, cnt_tabs + 1);
				chek = 0;
			}
			else if (flag == 10)
			{
				del(&stack);
				del(&stack);
				put_tabs(out, cnt_tabs);
			}
			else if (flag == 16)
			{
				put_tabs(out, cnt_tabs - 1);
			}
			else if (flag != 20)
				put_tabs(out, cnt_tabs);

			if (flag == 4 || flag == 5 || flag == 16 || flag == 6 || flag == 30)
				insert(&stack, flag);

			if (strin[0] == '\0')
				break;
			switch (flag)
			{
			case 0:
				dir_correction(out, strin, dirs, cnt_d);
				break;

			case 1:
				fprintf(out, "\n");
				struct_correction(out, strin, types, cnt++);
				break;

			case 2:
				fprintf(out, "\n");
				func_correction(out, strin);
				break;

			case 3:
				var_correction(out, strin);
				break;

			case 4:
				condition_correction(out, strin, &chek);
				break;

			case 5:
				for_correction(out, strin, &chek);
				break;

			case 6:
				fprintf(out, "do\n");
				for (int j = 0; j < 21; ++j)
					strin[j] = '\0';
				break;

			case 10:
				put_right_bracket(out, strin);
				break;

			case 12:
				condition_correction(out, strin, &chek);
				break;

			case 13:
				case_correction(out, strin);
				break;

			case 14:
				break_correction(out, strin);
				break;

			case 15:
				str_correction(out, strin);
				break;

			case 16:
				put_left_bracket(out, strin);
				break;

			case 18:
				after_struct_correction(out, strin, types, cnt++);
				break;

			case 19:
				return_correction(out, strin);
				break;

			case 20:
				big_com_correction(out, strin, cnt_tabs);
				fprintf(out, "\n");
				break;

			case 21:
				fprintf(out, "%s", strin);
				break;

			case 25:
				after_do_correction(out, strin);
				break;

			case 30:
				fprintf(out, "\n");
				struct_correction(out, strin, types, cnt++);
				break;

			default:
				for (int j = 0; j < 21; ++j)
					strin[j] = '\0';
				break; // пиздец
			}
		}

		fclose(in);
		fclose(out);
	}
}

void exitApp()
{
	setColor2(2);
	printf("Вы вышли из программы.");
	setColor2(0);
	exit(0);
}

void showHelp()
{
	setColor2(2);
	//printf("Команда \"\x1b[0mlist\x1b[33m\" - показать доступные файлы, \nфлаг \"\x1b[0m+show\x1b[33m\" вместе с кол-вом файлов - показать исходный код, \nкоманда \"\x1b[0mexit\x1b[33m\" - выйти из программы;\n");
	printf("Команда \"\x1b[0mlist\x1b[33m\" - показать доступные файлы, \nкоманда \"\x1b[0mexit\x1b[33m\" - выйти из программы.\n");
	setColor2(0);
}

void showFiles()
{
	printf("Доступные файлы:\n");
	setColor2(3);
	//char command[50] = "dir /b *.txt *.c *.cpp *.h";
	char command[50] = "dir /b *.cpp";
	system(command); //пофиг
	setColor2(0);
}

void unluckee()
{
	setColor2(1);
	printf("Unluckee, please try again.\n");
	setColor2(0);
}

void setColor2(int color)
{
	if (color == 1)
	{
		printf("\x1b[31m");
	}

	if (color == 2)
	{
		printf("\x1b[33m");
	}

	if (color == 3)
	{
		printf("\x1b[32;1m");
	}

	if (color == 4)
	{
		printf("\x1b[36m");
	}

	if (color == 5)
	{
		printf("\x1b[35;1m");
	}

	if (color == 0)
	{
		printf("\x1b[0m");
	}
}