#include <stdlib.h>

// 함수 템플릿(틀, 주형)
int Add(int a, int b)
{
	return a + b;
}

// 함수 오버로딩(이름이 같은 함수를, 인자의 개수, 입력 인자 타입(반환타입은 영향X)을 
// 다르게해서 여러개의 함수를 등록하는 방법
float Add(float a, float b)
{
	return a + b;
}

template<typename T>
T Mul(T a, T b)
{
	return a * b;
}



// 구조체 템플릿
// 자료구조(Data Structure)
// 동적배열(가변배열)
template<typename T>
struct Array
{
	T* pData;		// 동적할당 받은 힙 공간을 가리킬 포인터
	int  Capacity;	// 동적할당 받은 공간에 최대 저장할 수 있는 int 개수
	int  Size;		// 동적할당 받은 공간에 저장시킨 데이터 개수
};


template<typename T>
void Init(Array<T>* _Arr)
{
	_Arr->pData = (T*)malloc(sizeof(T) * 2);
	_Arr->Capacity = 2;
	_Arr->Size = 0;
}

template<typename T>
void Push_Back(Array<T>* _Arr, T _Data)
{
	// 허용 가능한 개수와, 현재 데이터 개수가 일치함 ==> 더이상 데이터를 받을 공간이 없음
	if (_Arr->Capacity == _Arr->Size)
	{
		// 1. 더 넓은 공간을 할당 받는다.
		T* pNew = (T*)malloc(sizeof(T) * _Arr->Capacity * 2);

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


struct UserInfo
{
	int age;
	int att;
	int def;
};



int main()
{
	int r = Add(10, 20);
	float f = Add(3.11f, 2.22f);

	Mul(2.22f, 1.11f);

	Array<int> arr;
	Init(&arr);
	Push_Back(&arr, 10);
	Push_Back(&arr, 20);
	Push_Back(&arr, 30);

	Array<UserInfo> arrUserInfo;
	Init(&arrUserInfo);
	Push_Back(&arrUserInfo, UserInfo{ 10, 100, 120 });
	Push_Back(&arrUserInfo, UserInfo{ 20, 200, 220 });
	Push_Back(&arrUserInfo, UserInfo{ 30, 300, 320 });

	return 0;
}