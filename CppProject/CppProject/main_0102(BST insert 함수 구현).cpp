#include <time.h>
#include <random>

#include <map>
#include "BST.h"
using namespace std;


int main()
{
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

	for (mapIter = testMap.begin(); mapIter != testMap.end(); ++mapIter)
	{
		cout << (*mapIter).first << endl;
	}


	//				100
	//            /		\
	//			50		150
	//		    /\       /\
	//		  25  70  111  200
	BST<int, float> myBST;
	myBST.insert(Make_Pair(100, 1.1f));

	myBST.insert(Make_Pair(50, 2.2f));

	myBST.insert(Make_Pair(150, 3.3f));
	myBST.insert(Make_Pair(25, 4.4f));
	myBST.insert(Make_Pair(111, 5.5f));
	myBST.insert(Make_Pair(70, 6.6f));
	myBST.insert(Make_Pair(200, 7.7f));

	BST<int, float>::iterator myiter = myBST.find(111);

	int Data = (*myiter).first;
	float fData = (*myiter).second;

	// 이진탐색트리에서는 중위순회가 중요
	// 중위순회로 접근하면, 정렬된 데이터 순서로 접근이 가능

	// 이진탐색트리의 시작을 가리키는 begin iterator 는 중위순외 기준 가장 첫번째 이다.
	// 이진탐색트리의 iterator 가 다음 데이터 혹은 이전데이터로 가리키는 방식은 중위순회 기준이다.
	myiter = myBST.begin();


	return 0;
}
