#include "Source.h"

int func2(int n, int b);
int func3(char m[50], int b)
{
	int a = b;
	a += 4;
	m[4] = a + b;
	func0(2 * (a + b + 3));
	return a;
}

int func2(int n, int b)
{
	char mas[30] = "ffff";
	int a = func3(mas, 3) + b;
	n = b + n;
	return n;
}