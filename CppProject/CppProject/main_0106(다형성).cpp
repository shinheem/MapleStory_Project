#include <iostream>
using namespace std;

// 상속과 다형성
class Parent
{
private:
	int		m_Parent;

public:
	// 순수 가상 함수 (추상 클래스 등록)
	virtual void PureVirtualFunc() = 0; // 상속받는 클래스에서 구현하기 위해 어떠한 동작을 하는지 구현하지 않음
};

class Child1
	: public Parent
{
private:
	int		 m_C1;

public:
	// 추상화 해제, 순수 가상함수를 구현해줌
	virtual void PureVirtualFunc()
	{

	}
};

class Child2
	: public Parent
{
private:
	float	 m_C2;
};

int main()
{
	// 객체지향
	// 1. 캡슐화
	// 2. 상속
	//		- 기능(함수) 재사용
	// 3. 다형성
	//		- 하나의 자료형이 여러가지를 나타낼 수 있음
	//		- 포인터를 이용 부모 클래스 포인터로 자식 클래스 객체를 가리킬 수 있음
	//		ex) 동물 자료형에서 파생된 고양이를 동물이라고 부를 수 있음
	// 4. 추상화
	//		- 파생되는 클래스들의 공통 부모 클래스. 구체성이 없고 개념적인 공통사항을 묶은 클래스
	//		  추상 클래스는 객체생성이 불가능해야 한다.
	//포인터 + 다형성
	
	Parent p;
	Child1 c1;
	Child2 c2;

	Parent* pParent = &p;
	Child1* pChild1 = &c1;
	Child2* pChild2 = &c2;

	// 포인터가 오버해서 가리키지 않는 상황에서는 포인터 변수에 저장 가능
	pParent = &c1; // 컴파일 에러가 안남
	pParent = &c2; // 컴파일 에러가 안남

	//pChild1 = &p; // 컴파일 에러가 발생
	//pChild2 = &p; // 컴파일 에러가 발생


	// 동물 클래스로 파생된 클래스들을 모두 지정 가능함
	vector<동물*> vecAnimals;

	for (int i = 0; i < vecAnimals.size(); i++)
	{
		vecAnimals[i]->Move(Position);
	}

	동물*



	return 0;
}