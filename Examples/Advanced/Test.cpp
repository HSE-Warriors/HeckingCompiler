#include "Test2.h"
#include "Test1.h"

int func0(int n);
int func1(char m[5]);


int func0(int n)
{
	char M[5] = { 0 };
	int i = 0;
	func1(M);
	func6(&i);
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
	int veryIMPORTANTvar = 1;
	return n + 2;
}