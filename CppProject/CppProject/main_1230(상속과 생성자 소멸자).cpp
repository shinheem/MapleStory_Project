#include <iostream>
using namespace std;

class Parent
{
protected:
	int		m_Parent;

public:
	void Output()
	{
		cout << "Parent Output" << " m_Parent : " << m_Parent << endl;
	}

public:
	Parent()
		: m_Parent(0)
	{
	}

	Parent(int _i)
		: m_Parent(_i)
	{
	}

	~Parent()
	{

	}
};

class Child : public Parent
{
private:
	int		m_Child;

public:
	using Parent::m_Parent; // 상속받은 부모쪽 맴버 or 맴버 함수의 접근 지정자 변경

public:
	// 오버라이딩, 부모 클래스의 맴버함수를, 자식 클래스에서 재정의함
	void Output()
	{
		cout << "Child Output" << " m_Child : " << m_Child << endl;
	}

public:
	Child()
		: Parent(10)
		, m_Child(0)
	{
	}

	~Child()
	{
		int a = 0;
		int b = 0;

		// 부모 소멸자 호출이 컴파일러에 의해서 추가됨
		// Parent::~Parent();
	}
};

int main()
{
	// 상속 + 생성자 호출
	// 자식 클래스의 생성자가 호출
	// 자식 클래스의 생성자에서 다시 부모쪽 생성자를 호출
	
	// 초기화 순서(생성자 호출 순서)
	// 부모 -> 자식
	// 해제 순서(소멸자 호출 순서)
	// 자식 -> 부모

	Parent p;
	p.Output();

	Child	c;
	c.Output();			// 오버라딩한 Child::Output 호출
	c.Parent::Output();	// 상속받은 Parent::Output 호출


	return 0;
}