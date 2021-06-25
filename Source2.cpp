char func4(int n)
{
	char m[50] = "lllllllllllllll";
	m[n] = 'o';
	return m[n];
}
int func6(int *n)
{
	++*n;
	func6(n);
	return(*n);
}