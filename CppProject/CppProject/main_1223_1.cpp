#include <iostream>
#include <vector>
#include <list>
#include <set>

using namespace std;

// 내부(Inner) 클래스, 외부(Outer), 포함 클래스
class MyClass
{
private:
	int m_i;

public:
	MyClass()
		: m_i(0)
	{
	}
	~MyClass()
	{
	}

public:
	// 내부 클래스
	class Inner
	{
	private:
		float m_f;
	public:
		Inner()
			:m_f(0.f)
		{
		}
	};
};


class OtherClass
{

public:
	class Inner
	{
		int m_Arr[100];
	};
};


int main()
{
	// iterator 는 내부 클래스로 구현이 되어있다.
	// 컨테이너와 연관되어있는 iterator 를 내부 클래스로 같이 제공을 한다.

	// 내부클래스 사용이유
	// 1. iterator 가 선언된 컨테이너 클래스와 iterator 와 논리적으로 관련이 있음을 나타냄	
	// 2. 외부클래스(컨테이너 클래스)를 마치 namespace 처럼 활용해서, 
	//    모든 컨테이너들은 자기 전용 iterator 클래스의 이름을 통일성있게 똑같이 만들 수 있음
	// 3. 내부 클래스는 외부 클래스의 private 필드에 접근이 가능
	MyClass my;
	MyClass::Inner inner;
	OtherClass::Inner inner2;



	vector<int> vecInt;

	vecInt.push_back(10);
	vecInt.push_back(20);
	vecInt.push_back(30);

	vecInt.at(0) = 100;
	vecInt[0] = 100;

	// iterator 객체를 생성해서, 벡터객체가 관리하고 있는 데이터 중에서 첫번째 데이터를 가리키는 상태로 만들어준다.
	vector<int>::iterator vecIter = vecInt.begin();

	// iterator 가 가리키고 있는 데이터로 접근한다.
	vecIter.operator*() = 100;
	*vecIter = 100;

	vecIter = vecInt.begin();	// 시작을 가리킴
	vecIter = vecInt.end();		// 마지막 다음을 가리킴(마지막 데이터가 아님)

	vecIter = vecInt.begin();
	// 입력으로 넣어준 iterator 가 가리키는 요소를 지우고, 지워진 다음 데이터를 가리키는 iterator 를 만들어서 반환해준다.
	vecIter = vecInt.erase(vecIter);


	// vector 에서 제공하는 at or operator[] 함수를 이용해서, 모든 데이터 순회해서 출력해보기
	for (int i = 0; i < vecInt.size(); ++i)
	{
		//std::cout << vecInt.at(i) << std::endl;
		std::cout << vecInt[i] << std::endl;
	}

	// iterator 를 이용해서 컨테이너 안에 들어있는 데이터를 처음부터 끝까지 순회해보기
	for (vecIter = vecInt.begin(); vecIter != vecInt.end(); ++vecIter)
	{
		std::cout << *vecIter << std::endl;
	}


	list<int> intList;

	intList.push_back(10);
	intList.push_back(20);
	intList.push_back(30);

	list<int>::iterator listIter = intList.begin();
	for (; listIter != intList.end(); ++listIter)
	{
		std::cout << *listIter << std::endl;
	}

	intList.erase(intList.begin());
	intList.pop_front();



	// iterator 패턴을 이용하면, 컨테이너에 구애받지 않고, 일관적인 코드를 작성할 수 있다.
	// 컨테이너 내부를 알 필요가 없다, 내부 구조를 들어낼 필요가 없다.		 
	set<int> setInt;

	setInt.insert(10);
	setInt.insert(20);
	setInt.insert(30);

	set<int>::iterator setIter = setInt.begin();

	for (; setIter != setInt.end(); ++setIter)
	{
		std::cout << *setIter << std::endl;
	}


	{
		list<int> intList;

		for (int i = 0; i < 100; ++i)
		{
			intList.push_back(i + 1);
		}

		// iterator 를 이용해서 50보다 작은 수를 반복문으로, 컨테이너 안에서 제거하기
		for (list<int>::iterator iter = intList.begin(); iter != intList.end(); )
		{
			if (*iter < 50)
				// erase 의 반환값을 iterator 가 입력받으면, 이미 다음 요소를 가리키는 상태가 됨
				iter = intList.erase(iter);
			else
				++iter;
		}
	}

	return 0;
}

 