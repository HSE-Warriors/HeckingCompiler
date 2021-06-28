#include "Test.h"
#include "Test1.h"
#include "Test2.h"

int func2(int n, int b);
int func3(char m[50], int b)
{
	int a = b;
	a += 4;
	m[4] = a + b;
	func0(2 * (a + b + 3));
	func6(&a);
	return a;
}

int func10()
{
	func11();
	return 2;
}

void func11()
{
	int n = func10();
	return;
}

int func2(int n, int b)
{
	char mas[30] = "ffff";
	int a = func3(mas, 3) + b;
	n = b + n;
	return n;
}