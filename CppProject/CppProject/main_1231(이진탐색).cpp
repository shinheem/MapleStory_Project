#include <time.h>
#include <random>

#include <set>
#include <map>
using namespace std;

#include "Heap.h"

// 그래프 -> 트리 -> 이진트리 -> 완전이진트리(배열) -> 힙 -> 힙 소트
//							->  이진탐색트리

// 이진탐색 - 분할 정복 알고리즘
// 이진트리 + 이진탐색 ==> 이진탐색트리
template<int Size>
int BinarySearch(int(&_Arr)[Size], int _Find)
{
	int Start = 0;
	int End = Size - 1;

	while (true)
	{
		// 탐색 범위 에서 중간 인덱스를 구함
		int Idx = (Start + End) / 2;

		// 중간 지점 값을 확인해서, 찾는 숫자랑 일치하면, 해당 인덱스 반환
		if (_Arr[Idx] == _Find)
			return Idx;

		// Start 와 End 가 만났다, 그런데도 찾는 숫자가 아니다 --> 없었다
		if (Start >= End)
			break;

		// 찾는 숫자가 중간에 있는 값보다 크다, 오른쪽으로 탐색범위 이동
		if (_Arr[Idx] < _Find)
			Start = Idx + 1;
		// 찾는 숫자가 중간에 있는 값보다 작다, 왼쪽으로 탐색범위 이동
		else
			End = Idx - 1;
	}

	return -1;
}


int main()
{
	// 이진탐색
	// 데이터가 정렬되어있다.

	// 순차탐색    이진탐색
	// O(N)		   O(logN)	

	Heap<int> minHeap;


	// 무작위 데이터로 힙을 채운다.
	//srand(time(nullptr));
	for (int i = 0; i < 10; ++i)
	{
		minHeap.push(rand() % 1000 + 1);
	}

	// 힙에서 최소값을 계쏙 뽑아서 배열에 순서대로 넣는다.
	int Data[10] = {};
	for (int i = 0; i < 10; ++i)
	{
		Data[i] = minHeap.Get();
	}

	int Idx = BinarySearch(Data, 502);

	// 이진탐색 + 이진트리 == 이진탐색트리
	// 이진탐색을 시도할 수 있도록 만들어진 이진트리

	//			동적 배열		연결형 리스트		 이진탐색 트리					해시테이블   
	// 입력		  O(1)				O(1)			O(LogN) - 정렬이 필요          O(1)
	// 탐색       O(N)			    O(N)			O(LogN) - 이진탐색			  O(1)


	// 이진탐색 STL 컨테이너 set
	set<int> intSet;

	//				100
	//            /		\
	//			50		150
	//		    /\       /\
	//		  25  70  111  200
	intSet.insert(100);
	intSet.insert(50);
	intSet.insert(150);
	intSet.insert(25);
	intSet.insert(111);
	intSet.insert(70);
	intSet.insert(200);

	set<int>::iterator iter = intSet.find(25);

	int Test = 0;
	if (iter != intSet.end())
	{
		Test = *iter;
	}

	//				100
	//            /		\
	//			50		150
	//		    /\       /\
	//		  25  70  111  200
	map<int, float> testMap;
	testMap.insert(make_pair(100, 1.1f));
	testMap.insert(make_pair(50, 2.2f));
	testMap.insert(make_pair(150, 3.3f));
	testMap.insert(make_pair(25, 4.4f));
	testMap.insert(make_pair(111, 5.5f));
	testMap.insert(make_pair(70, 6.6f));
	testMap.insert(make_pair(200, 7.7f));

	map<int, float>::iterator mapIter = testMap.find(25);
	if (mapIter != testMap.end())
	{
		(*mapIter).first;
		(*mapIter).second;
	}
	return 0;
}
