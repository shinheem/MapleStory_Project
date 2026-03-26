#include <iostream>
#include <vector>
#include "Arr.h"

using namespace std;


int main()
{
	Arr<int> arrInt;
	arrInt.push_back(10);
	arrInt.push_back(20);
	arrInt.push_back(30);

	Arr<int>::iterator iter = arrInt.begin();

	iter = arrInt.erase(iter);

	// 20
	cout << *iter << endl;

	// 20
	// 30
	for (int i = 0; i < arrInt.size(); ++i)
	{
		cout << arrInt[i] << endl;
	}

	// 데이터 다 지우기
	arrInt.erase(arrInt.begin());
	arrInt.erase(arrInt.begin());

	// Insert Test
	{
		Arr<int> arrInt;
		arrInt.push_back(10);
		arrInt.push_back(20);
		arrInt.push_back(30);

		Arr<int>::iterator iter = arrInt.begin();
		++iter;

		arrInt.insert(iter, 5);

		for (int i = 0; i < arrInt.size(); ++i)
		{
			cout << arrInt[i] << endl;
		}
	}

	return 0;
}