#include <stdio.h>




// 함수
// 1. 코드를 모듈화, 재활용
// 2. 새로운 기능을 만들때 편리
// 반환타입, 함수 이름, 입력 인자
int Add(int a, int b)
{
	int c = a + b;

	return c;
}


void Test3()
{
	int k = 0;
}

void Test2()
{
	int j = 0;

	Test3();
}

void Test1()
{
	int i = 0;

	Test2();
}

// Num 숫자를 Count 횟수만큼 누적해서 곱해서 리턴
int Pow(int Num, int Count)
{
	int result = 1;
	for (int i = 0; i < Count; ++i)
	{
		result *= Num;
	}
	return result;
}

// 프로그램 시작
int main()
{
	Test1();

	// 함수 - Function
	int j = Add(100, 300);

	j = Add(1000, 1000);

	// 지역 변수
	// 함수 안에 선언된 변수
	// 해당 함수가 호출되면, 그 스택안에 존재한다.

	// 함수가 호출될때 사용하는 메모리 영역 이름을 스택 메모리 영역이라고 한다.
	// 함수가 호출될때 마다 스택이 생기고, 종료될때 해당 함수가 사용하던 스택이 해제되기 때문에
	// 가장 나중에 호출된 함수가 가장 먼저 종료되고, 이 구조가 마치 자료구조 스택과 일치하기 때문

	// 자료구조 스택은 데이터를 후입선출 or 선입후출 구조로 데이터를 관리하는 알고리즘이다.


	// 제곱근 구하는 함수
	int Num = Pow(2, 10);


	return 0;
}



// F10 - 프로시저 단위로 실행
// F11 - 최소 작업단위로 코드 실행