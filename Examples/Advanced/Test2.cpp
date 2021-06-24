#include "Test2.h"
#include "Test.h"
char func4(int n)
{
	char m[50] = "lllllllllllllll";
	m[n] = 'o';
	int i = 0;
	func7();
	func8();
	func9(i);
	return m[n];
}
int func6(int *n)
{
	++*n;
	func6(n);
	return(*n);
}

void func7()
{
	func8();
	return;
}

void func8()
{
	int a = 0;
	func4(a);
	return;
}

int func9(int i)
{
	char aaa = 'F';
	func1(&aaa);
	return 0;
}