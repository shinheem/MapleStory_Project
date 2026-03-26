#include "Heap.h"
#include <iostream>

#include <vector>

// 완전이진트리(Complete Binary Tree)
// 트리를 구성할때, 왼쪽에서 오른쪽으로 채워가는 형태(한 레벨을 순서대로 채우는 형태)로 데이터를 추가한다.
// 코드로 구현할 경우 배열구조로 구현 가능

// 루트노드를 배열의 0 인덱스에 추가
// 자식으로 접근할때 2 * (Index) + 1 ==> 왼쪽 자식 인덱스
//                  2 * (Index) + 2 ==> 왼쪽 자식 인덱스
//                  (Index - 1) / 2 ==> 부모 인덱스

// 자료구조 힙(완전 이진트리를 이용해서 구현)
// 힙 메모리 영역과는 무관하다.
// 자료구조 힙은 최대 or 최소 값을 빠르게 찾고 싶을때 사용하는 자료구조

// 완전 이진트리 --> 자료구조 힙(Min, Max 빨리 찾기) --> 우선순위 큐(Priority Queue)

// 열거형
// 나열한 순서대로 정수 0 부터 하나씩 증가한 숫자로 취급
// 매크로 전처리기와 유사
// 관련된 상태값을 하나의 열거형으로 묶을 수 있음
// 열거형 타입을 자료형 처럼 사용이 가능, 나열된 타입중 하나를 입력할 수 있음
// 나열된 타입 이외의 숫자가 입력될 수 없음(컴파일 타임에 오류를 방지할 수 있음)
// 컴파일 타임에 열거형 정보를 컴파일러가 인식하기 때문에, 디버깅 할 경우, 
// 저장된 숫자를 enum 상태로 비교해서 볼 수 있음
enum PLAYER_STATE
{
	IDLE,
	MOVE,
	DASH,
	ATTACK,
	HEART,
	HIT,
	DEAD,
};

enum OTHER_TYPE
{
	HIT,

};

#define MYTYPE_1  0
#define MYTYPE_2  1

#define MYTYPE_1 10


using namespace std;

int main()
{
	int a = MYTYPE_2;

	PLAYER_STATE state = HEART;
	state = ATTACK;
	a = HIT;



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