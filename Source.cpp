#include "Source2.h"
#include "Source1.h"

int func0(int n);
int func1(char m[5]);


int func0(int n)
{
	char M[5] = { 0 };
	func1(M);
	return 2;
}

int func1(char m[5])
{
	int a = 0;
	int b = 1;
	char s[50] = "jojo";
	func2(a, b);
	func3(s, a);
	func4(b);
	return b;
}

int func5(int n)
{
	return n + 2;
}