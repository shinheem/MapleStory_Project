#include <stdlib.h>
#include <stdio.h>

// 자료구조(Data Structure)
// 동적배열(가변배열)
struct Array
{
	int* pData;		// 동적할당 받은 힙 공간을 가리킬 포인터
	int  Capacity;	// 동적할당 받은 공간에 최대 저장할 수 있는 int 개수
	int  Size;		// 동적할당 받은 공간에 저장시킨 데이터 개수
};

void Init(Array* _Arr)
{
	_Arr->pData = (int*)malloc(sizeof(int) * 2);
	_Arr->Capacity = 2;
	_Arr->Size = 0;
}

void Push_Back(Array* _Arr, int _Data)
{
	// 허용 가능한 개수와, 현재 데이터 개수가 일치함 ==> 더이상 데이터를 받을 공간이 없음
	if (_Arr->Capacity == _Arr->Size)
	{
		// 1. 더 넓은 공간을 할당 받는다.
		int* pNew = (int*)malloc(sizeof(int) * _Arr->Capacity * 2);

		// 2. 새로 할당받은 공간으로, 원래 데이터를 옮겨준다.
		for (int i = 0; i < _Arr->Size; ++i)
		{
			pNew[i] = _Arr->pData[i];
		}

		// 3. 기존 공간을 해제
		free(_Arr->pData);

		// 4. 주소 교체
		_Arr->pData = pNew;
		_Arr->Capacity *= 2;
	}

	_Arr->pData[_Arr->Size++] = _Data;
}


int g_Arr[100] = {};

int main()
{
	// 배열
	int Arr[10] = {};

	Array arr = {};
	Init(&arr);

	int Count = 0;
	scanf_s("%d", &Count);

	for (int i = 0; i < Count; ++i)
	{
		Push_Back(&arr, (i + 1) * 10);
	}

	for (int i = 0; i < arr.Size; ++i)
	{
		printf("%d\n", arr.pData[i]);
	}

	free(arr.pData);
	return 0;
}