#include <iostream>
using namespace std;

class Parent
{
private:
	int		m_Parent;

public:
	// 가상함수 선언 시 클래스의 크기가 변경
	// 가상함수 선언 시 __vptr이라는 포인터 맴버 변수 자동으로 추가
	void Output() // 동적 바인딩을 위한 순수 가상함수 선언
	{
		cout << "Parent Output" << m_Parent << endl;
	}

public:
	Parent()
		:m_Parent(0)
	{}
};

class Child
	: public Parent
{
private:
	int		m_Child;

public:
	// 부모의 모든 기능이 다 맘에 들지는 않는다.
	// 오버 라이딩
	void Output()
	{
		cout << "Child Output" << m_Child << endl;
	}

public:
	Child()
		:m_Child(0)
	{ }
};

int main()
{
	int size = sizeof(Parent);

	Parent parent;
	Parent p1;
	Parent p2;
	parent.Output();

	Child child;
	child.Output();
	//child.Parent::Output();



	Parent* pParent = &child;


	pParent->Output();

	pParent = &parent;
	
	pParent->Output();

	int input = 0;
	cin >> input;

	if (input == 1)
	{
		pParent = &parent;
	}
	else if (input == 2)
	{
		pParent = &child;
	}

	// 정적 바인딩
	// 컴파일 타임에 호출 함수 확정
	pParent->Output(); // 부모 클래스를 상속받은 자식 클래스의 주소를 담고있는 Parent포인터에서 자식 Output호출
	
	// 정적 바인딩 --> 동적 바인딩
	// 상속구조의 클래스들은 가상함수가 1개라도 있으면 타입정보가 생성된다.
	// 타입 정보안에는 해당 클래스에 오버라이딩한 가상함수들의 주소가 들어있다.
	// 가상함수가 1개 이상 있으면, 가상함수 테이블 포인터가 맴버로 메모리 구조 가장 앞에 추가가 된다.
	// 가상함수 테이블 포인터는, 해당 클래스의 타입 정보안에 있는 가상함수 테이블의 주소를 가리킨다.
	// 가상함수를 호출할때는 포인터가 가리키는 객체에 접근해서, 가상함수 테이블 포인터를 참조하고, 거기에 등록되어있는 함수를 호출한다.
	// 각 객체들은 생성될때, 가상함수 테이블의 주소를 자신의 클래스에 맞는 테이블의 주소를 입력받으면서 생성된다.
	// pParent 포인터에 담겨있는 주소가 가리키는 실제 객체가 만약 Child 객체의 주소였다면,
	// pParent 주소로 접근하면, Child 객체일 것이고, 그 맨앞에 있는 가상함수 테이블 포인터가 가리키는 주소는 Child 클래스 
	// 테이블 이다. 여기에 등록된 Child::Output 함수가 호출이 된다.

	return 0;
}