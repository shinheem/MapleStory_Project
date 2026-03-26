#include<stdlib.h>

void DynamicAlloc(int** _ppInt)
{
	*_ppInt = (int*)malloc(4);
}

int main()
{
	int Number = 0;

	// 포인터
	int* pInt = &Number;
	*pInt = 100;

	// 이중 포인터
	int** ppInt = &pInt;
	*(*ppInt) = 200;

	// pData가 힙메모리의 주소를 받아왔기 때문에 함수가 종료되어도 main함수에서 힙메모리 공간의 데이터 조작 가능
	int* pData = nullptr;
	DynamicAlloc(&pData);

	return 0;
}