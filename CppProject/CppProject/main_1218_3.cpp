#include <stdlib.h>

class MyClass
{
public:
	int m_Arr[10];

public:
	MyClass()
		: m_Arr{}
	{

	}

	~MyClass()
	{

	}

};

int main()
{
	// 동적할당
	// 런타임 중에도, 필요한 만큼 메모리를 실시간으로 요청 가능
	// 힙 영역에 400 바이트 요청
	int * pArr = (int*)malloc(400);




	// 프로그램 종료 전에 프로그래머가 직접 할당받은 힙공간을 해제 해야함
	free(pArr);

	MyClass my;

	my.m_Arr[1] = 10;
	my.m_Arr[2] = 20;
	my.m_Arr[9] = 100;

	
	// malloc, free 는 함수 호출 구문
	// 힙 영역에 원하는 만큼의 메모리 크기를 입력해주면, 그만큼 힙 공간에 메모리를 할당하고 시작주소를 반환
	// 힙 영역에 할당된 공간의 주소를 주면, 그곳을 메모리 해제
	 
	 
	// new, delete
	// new :	힙 영역에 내가 사용할 자료형 정보를 전달(자료형 크기만큼 동적할당 하고, 생성자 호출)
	// delete : 힙 영역에 할당된 공간의 주소를, 포인터 변수로 전달
	//			(해당 공간이 무슨 목적으로 사용 됐었는지 까지 알려줌, 소멸자 호출 후 메모리 해제)
	 
	// MyClass* pMy = (MyClass*)malloc(40);
	MyClass* pMy = new MyClass;




	// free(pMy);
	delete pMy;
	
	// 생성자 호출 3번 진행
	// 힙 영역에 원하는 자료형의 객체를 연속된 메모리 구조로 여러개를 동시에 동적할당 할 때
	MyClass* pArrMy = new MyClass[3];

	pArrMy[0];
	pArrMy[1];
	pArrMy[2];

	// 소멸자 호출 3번 진행
	// 배열형태로 할당한 객체들을 메모리 해제할 때
	delete[] pArrMy;


	return 0;
}