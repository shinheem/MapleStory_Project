#include <random>
#include <time.h>
#include <stdlib.h>

void MemoryCopy(void* pDst, const void* pSrc, int Size)
{
	unsigned char* pDestnation = (unsigned char*)pDst;
	unsigned char* pSource = (unsigned char*)pSrc;

	for(int i = 0; i < Size; i++)
	{ 
		pDestnation[i] = pSource[i];
	}
}

//void MemoryCopy(float* pDst, float* pSrc, int Count)
//{
//	for (int i = 0; i < Count; i++)
//	{
//		pDst[i] = pSrc[i];
//	}
//}

int main()
{
	// stdlib.h에서 제공하는 메모리 카피 함수
	// memcpy();
	
	// 포인터 변수를 아무것도 가리키지 않은 상태로 초기화
	// nullptr 주소(0번지) 로 접근하면 런타임 에러가 발생한다.
	char* pChar = nullptr;

	int Data = 0;

	// 강제 형 변환(캐스팅)
	pChar = (char*) & Data;

	/*int a = 100;
	int b = 0;
	b = a;
	MemoryCopy(&b, &a);*/

	srand(time(nullptr));

	// 배열의 이름 크기 자체는 자료형 * 배열의 갯수이지만 
	// 포인터변수에 배열의 이름을 넣으면 배열의 시작주소만 저장한다. 
	int iArr[10] = {}; 
	for (int i = 0; i < 10; ++i)
	{
		iArr[i] = rand() % 1000 + 1;
	}

	int OtherArr[10] = {};

	MemoryCopy(OtherArr, iArr, sizeof(int) * 10);


	// void* - void 포인터
	// void 포인터는 주소를 저장하는 포인터 변수면서 특정 타입의 주소만 받도록 하지 않고
	// 모든 자료형의 주소를 받을 수 있는 포인터 변수이다.
	// 주소로 접근하거나 주소연산을 실행할 수 없다.
	// 현재 저장하고있는 주소로 접근했을 경우, 해당 공간이 어떤 변수가 있을지 알 방도가 없기 때문
	void* pVoid = nullptr;

	int iData = 0;
	pVoid = &iData;

	int fData = 0.f;
	pVoid = &fData;

	
	float fArr[10] = {};
	for (int i = 0; i < 10; ++i)
	{
		fArr[i] = (float)(rand() % 1000 + 1);
	}

	float fOtherArr[10] = {};

	int a = sizeof(fOtherArr);

	MemoryCopy(fOtherArr, fArr, sizeof(float) * 10);

	return 0;
}