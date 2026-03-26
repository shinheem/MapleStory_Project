#include <iostream>
#include "Heap.h"

using namespace std;

// enum 보다 더 엄격한 규칙
// enum 열거형 앞에 반드시 소속 enum 타입을 범위지정 해야한다.
// enum class 에 나열된 열거형을 일반 정수타입 변수에 대입시킬 수 없다.(캐스팅 필요)
enum class MY_TYPE
{
	STATE_0,
	STATE_1,
	STATE_2 = 4,
	STATE_3,
};

enum class OTHER_TYPE
{
	OTHER_0,
	OTHER_1,
	OTHER_2,
	OTHER_3,
	STATE_0,
};

// 상태값들을 하나의 정수에 비트단위로 조합을 해서 사용하는 경우
enum class DEBUFF_TYPE
{
	FIRED = 0x1,
	ICED = 0x2,
	POISIONED = 0x4,
	BLEED = 0x8,
};


int main()
{
	HEAP_MODE mode;
	int a = MIN;

	// 열거형 타입을 반드시 적어줘야한다.
	MY_TYPE b = MY_TYPE::STATE_0;
	OTHER_TYPE c = OTHER_TYPE::STATE_0;

	int d = (int)MY_TYPE::STATE_0;

	unsigned int State = (unsigned int)DEBUFF_TYPE::ICED | (unsigned int)DEBUFF_TYPE::BLEED;

	Heap<int> myHeap;
	myHeap.SetMode(MIN);

	myHeap.push(10);
	myHeap.push(17);
	myHeap.push(5);
	myHeap.push(22);
	myHeap.push(2);
	myHeap.push(24);
	myHeap.push(8);

	myHeap.SetMode(HEAP_MODE::MAX);

	int Array[7] = {};
	int Loop = myHeap.size();
	for (int i = 0; i < Loop; ++i)
	{
		Array[i] = myHeap.Get();
	}

	return 0;
}


