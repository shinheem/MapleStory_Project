#include <iostream>
#pragma(1)
using namespace std;
// 가상함수
// 순수 가상함수
// 
// 가상 소멸자
// 객체를 동적할당해서 사용할 결루 delete를 이용해서 직접 메모리 해제를 시키야 한다,
// 다형성을 이용하기 위해서 부모포인터 타입으로 자식클래스 타입 객체를 가리키다가,
// delete하는 경우 부모 포인터 타입에 클래스의 소멸자 호출로 정적 바인딩이 된다.
// 이것을 방지하기 위해서 소멸자를 처음부터 가상함수로 만든다.



class AAA
{
private:
	// __vptr // 가상함수 선언 시 포인터 맴버 변수가 암묵적으로 생성
	int m_A;

public:
	// 순수 가상 함수 => 클래스를 추상클래스로 선언하기 위한 함수
	// 구현은 하지 않고 가상함수를 = 0 과 같이 선언
	// 파생 클래스(자식 클래스)가 반드시 구현해야 하는 인터페이스 제시
	virtual void PureFunc() = 0;

	virtual void Output()
	{
		cout << m_A << endl;
	}

public:
	AAA()
		: m_A(0)
	{
	}

	virtual ~AAA()
	{ }
};

class BBB
	: public AAA
{
private:
	int m_B;

public:
	virtual void Output()
	{
		cout << m_B << endl;
	}
};

class CCC
	: public BBB
{
private:
	int m_c;

public:
	virtual void Output()
	{
		cout << m_c << endl;
	}
};


int main()
{

	AAA a;
	a.Output();

	BBB b;
	b.Output();


	AAA* pA = nullptr;

	pA = &a;
	pA = &b;

	// 가상함수 선언 시 컴파일 방식이 변경되어 정적 바인딩 -> 동적 바인딩으로 변경
	pA->Output(); 

	return 0;
}