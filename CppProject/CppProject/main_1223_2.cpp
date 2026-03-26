#include <iostream>
#include <vector>
#include "Arr.h"

using namespace std;

class Test
{
public:
	Test& Output()
	{
		return *this;
	}
};


int main()
{
	vector<int> vecInt;
	vector<int>::iterator viter = vecInt.end();
	//--viter;

	Arr<int> arrInt;
	// 후위 operator++ 은 iterator 가 다음 요소를 가리키고, 내부에서 복사한 객체를 반환한다.
	Arr<int>::iterator myiter = arrInt.end();
	//--myiter;



	// Swap Test
	{
		Arr<int> arrInt;
		Arr<int> arrInt2;

		arrInt.push_back(10);
		arrInt.push_back(20);

		arrInt2.push_back(30);
		arrInt2.push_back(40);
		arrInt2.push_back(50);

		arrInt.swap(arrInt2);

		for (int i = 0; i < arrInt.size(); ++i)
		{
			cout << arrInt[i] << endl;
		}

		for (int i = 0; i < arrInt2.size(); ++i)
		{
			cout << arrInt2[i] << endl;
		}
	}


	// operator= Test
	{
		Arr<int> arrInt2;

		arrInt2.push_back(100);
		arrInt2.push_back(200);
		arrInt2.push_back(300);

		Arr<int> arrInt = arrInt2;
		for (int i = 0; i < arrInt.size(); ++i)
		{
			cout << arrInt[i] << endl;
		}
	}





	return 0;
}