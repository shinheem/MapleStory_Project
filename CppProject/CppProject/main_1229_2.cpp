#include <iostream>
#include <list>

#include "List.h"

using namespace std;

int main()
{
	//list<int> stlList;
	//stlList.push_back(10);
	//stlList.push_back(20);
	//stlList.push_back(30);

	//list<int>::iterator listIter = stlList.begin();
	//for (; listIter != stlList.end(); ++listIter)
	//{
	//	cout << *listIter << endl;
	//}

	//listIter = stlList.begin();
	//listIter = stlList.erase(listIter);



	// erase 구현 테스트
	//{
	//	List<int> myList;
	//	myList.push_back(10);
	//	myList.push_back(20);
	//	myList.push_back(30);

	//	List<int>::iterator iter;

	//	iter = myList.begin();
	//	++iter;
	//	++iter;
	//	iter = myList.erase(iter);

	//	for (iter = myList.begin(); iter != myList.end(); ++iter)
	//	{
	//		cout << *iter << endl;
	//	}
	//}


	// Insert 구현 테스트
	{
		


		List<int> myList;
		myList.push_back(10);
		myList.push_back(20);
		myList.push_back(30);

		List<int>::iterator iter;

		List<int> myList1(myList);

		iter = myList1.begin();
		++iter;
		//myList.insert(iter, 15);



		for (iter = myList1.begin(); iter != myList1.end(); ++iter)
		{
			cout << *iter << endl;
		}
	}


	return 0;
}