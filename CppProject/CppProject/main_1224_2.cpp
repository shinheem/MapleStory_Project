#include <iostream>
#include <list>

#include "List.h"

using namespace std;

int main()
{
	list<int> stlList;
	stlList.push_back(10);
	stlList.push_back(20);
	stlList.push_back(30);

	list<int>::iterator listIter = stlList.begin();
	for (; listIter != stlList.end(); ++listIter)
	{
		cout << *listIter << endl;
	}

	List<int> myList;
	myList.push_back(10);
	myList.push_back(20);
	myList.push_back(30);

	List<int>::iterator iter = myList.begin();
	for (; iter != myList.end(); ++iter)
	{
		cout << *iter << endl;
	}

	return 0;
}