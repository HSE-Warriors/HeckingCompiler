#define _THIS
#include <that>

int FunctionWithCorrectName(); //correct name, used
int functionWithIncorrectName(); //incorrect name, used
int UnusedFunctionWithCorrectName(); //correct name, unused
int unusedFunctionWithIncorrectName(); //incorrect name, unused

int main(void)
{
	int correctlyNamedEmptyVariable; //correct name, not initialized
	int correctlyNamedInitializedVariable = 10; //correct name, initialized, used
	int IncorrectlyNamedEmptyVariable; //incorrect name, not initialized
	char IncorrectlyNamedInitializedVariable[] = "hi"; //correct name, initialized
	int inited;

	printf("Enter name of the file:\n");
	scanf_s("%s", nameOfFile, 255);

	correctlyNamedInitializedVariable++;

	FunctionWithCorrectName();
	functionWithIncorrectName(); 

	inited = 0;
	int superb;

	return 0;
}