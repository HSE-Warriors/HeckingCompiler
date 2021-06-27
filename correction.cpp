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