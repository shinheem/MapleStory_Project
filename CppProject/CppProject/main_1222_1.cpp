#include "Arr.h"

#include <iostream>
#include <vector>
#include <list>

using std::vector;
using std::list;

using std::cout;
using std::endl;

int main()
{
	Arr<int> arrInt;

	arrInt.push_back(10);
	arrInt.push_back(20);
	arrInt.push_back(30);

	arrInt.GetData(0) = 200;
	arrInt[1] = 300;

	cout << "Arr 입력 데이터 확인" << endl;
	for (int i = 0; i < arrInt.size(); ++i)
	{
		cout << arrInt[i] << endl;
	}

	// STL (Standard Template Library)

	// =======================================================================
	// vector : 동적배열 구조로 데이터를 저장 및 관리 가능한 컨테이너 클래스 템플릿
	// 메모리 구조를 연속적인 공간으로 사용하게 때문에, 데이터를 맨 앞쪽에 넣는 
	// push_front 기능은 제공되지 않는다(비 효율적이기 때문)
	// =======================================================================
	vector<int> vecInt;

	vecInt.push_back(10);
	vecInt.push_back(20);
	vecInt.push_back(30);

	vecInt.at(0) = 200;
	vecInt[1] = 300;

	vector<int>::iterator veciter = vecInt.begin();
	int Get = *veciter;
	*veciter = 2000;



	cout << "vector 입력 데이터 확인" << endl;
	for (int i = 0; i < vecInt.size(); ++i)
	{
		cout << vecInt[i] << endl;
	}

	// =================================================
	// lsit : 데이터를 노드단위로 저장하고, 서로 연결한 구조
	// 리스트에 저장된 데이터에 접근하기 위해서, iterator 가 필요하다.
	// 인덱싱 기반의 데이터 접근방식을 사용하는 at, [] 함수를 제공하지 않는다.
	// =================================================
	list<int> intList;

	// 100 - 10 - 20 - 30
	intList.push_back(10);
	intList.push_back(20);
	intList.push_back(30);
	intList.push_front(100);

	list<int>::iterator iter;
	iter = intList.begin(); // intList 의 시작을 가리키는 상태
	*iter = 200;
	int dat = *iter;

	++iter;
	++iter;
	dat = *iter;

	//				Vector		List
	// 인덱싱		 O(1)		O(N)
	// 삽입 / 삭제    O(N)	    O(1)



	// iterator 패턴
	// 반복자 - iterator
	// 컨테이너안에 저장되어있는 데이터를 접근할 수 있는 기능
	// 특정 컨테이너에 구애받지 않는, 일관적인 코드를 작성할 수 있다.
	// 컨테이너 내부 구현을 외부에 숨길 수 있다.








	return 0;
}