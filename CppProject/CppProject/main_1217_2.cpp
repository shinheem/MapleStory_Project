#include <stdlib.h>
#include <stdio.h>

class Test
{	
private:
	int		i;
	char	c;

public:

	// 반환값을, 맴버 함수를 호출한 객체 자신의 참조를 반환한다.
	Test& Output()
	{				
		printf("Output\n");
		return *this;
	}

public:
	// 생성자, 소멸자는 프로그래머가 구현하지 않아도, 기본형태로 컴파일러에 의해서 구현이 된다.
	// 객체가 생성될 때 생성자가 호출되어야만 하고, 소멸할때는 소멸자가 호출되야 하는 
	// 규칙을 위배하지 않기 위해서

	// 기본 생성자, 디폴트 생성자
	Test()
	{
	}
		
	// 기본 생성자가 아닌 다른 버전의 생성자를 1개 이상 만들면, 컴파일러는 기본생성자를 
	// 자동으로 만들어주지 않음
	Test(int _i, char _c)
		: i(_i)
		, c(_c)
	{
	}

	// 소멸자
	~Test()
	{

	}
};

int Add();



int main()
{
	// 래퍼런스(참조)
	// C++ 에 추가된 개념
	// 포인터와 내부동작원리는 똑같다
	// 래퍼런스 변수는 실제로 원본 변수의 주소를 가지고,
	// 원본을 수정하는 개념
	int number = 0;

	int* pInt = &number;
	*pInt = 100;

	// iRef 가 number 를 참조한다.
	int& iRef = number;
	iRef = 200;
	
	// Output 함수를 호출시킨 객체 t 가 반환으로 다시 돌아온다. 
	// 그 반환된 t 객체로 output 함수를 연달아 이어서 호출이 가능하다.
	Test t;
	t.Output().Output().Output();

	// 기본 생성자를 이용해서 객체를 만들때 () 를 붙이지 않는다.
	// 반환타입이 Test 이고 함수이름이 test 인 함수의 전방선언 구문과 같아지기 때문
	//Test test();
	Test test;

	return 0;
}