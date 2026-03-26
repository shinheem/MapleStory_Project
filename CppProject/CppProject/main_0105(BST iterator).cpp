
#include <map>
#include "BST.h"

using namespace std;

int main()
{
	//				111
	//            /		\
	//			50		150
	//		    /\        \
	//		  25  70      200
	BST<int, float> myBST;
	myBST.insert(Make_Pair(100, 1.1f));
	myBST.insert(Make_Pair(50, 2.2f));
	myBST.insert(Make_Pair(150, 3.3f));
	myBST.insert(Make_Pair(25, 4.4f));
	myBST.insert(Make_Pair(111, 5.5f));
	myBST.insert(Make_Pair(70, 6.6f));
	myBST.insert(Make_Pair(200, 7.7f));

	BST<int, float>::iterator iter = myBST.begin();

	for (; iter != myBST.end(); ++iter)
	{
		cout << iter->first << endl;
	}

	// end 에서 역순으로 가리키기
	{
		//iter = myBST.end();
		//cout << (--iter)->first << endl;
		//cout << (--iter)->first << endl;
		//cout << (--iter)->first << endl;
		//cout << (--iter)->first << endl;
		//cout << (--iter)->first << endl;
		//cout << (--iter)->first << endl;
		//cout << (--iter)->first << endl;
		////--iter;
	}

	iter = myBST.find(100);
	iter = myBST.erase(iter);

	cout << iter->first << endl;


	// iterator를 포인터처럼 사용하기 
	// 가리키는 데이터의 first, second 요소를 접근할때 사용할 수 있는 -> operator 함수 제공
	iter->first;
	iter->second;

	struct Temp
	{
		int first;
		int second;
	};

	Temp t;
	t.first;
	t.second;

	// 구조체 포인터로 맴버 접근하기
	Temp* pTemp = &t;
	(*pTemp).first;
	(*pTemp).second;
	pTemp->first;
	pTemp->second;

	// set, map
	// 자가균형 이진탐색트리
	// (AVL, Red-Black) -- 꽤 난이도가 있어 자료구조를 찾아보고 구현 연습 -- leaf및에 null노드는 블랙 n번째 줄은 red로 설정하여 균형을 맟추는 트리

	return 0;
}