#include "Math.h"

static int Global_1;
//int Global_2;

int Extern = 0;

int Mul(int a, int b)
{
	Global_1 = 0;
	//Global_2 = 100;
	return a * b;
}