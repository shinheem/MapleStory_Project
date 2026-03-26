
#include "Arr.h"

// 상속
// 캡슐화, 상속, 다형성, 추상화
// 기존에 존재하는 자료형(클래스, 구조체)의 기능을 물려받아서 새로운 자료형을 설계하는 것


// 상속 관련 문법 체크
class Parent
{
private: // 외부, 자식 클래스에게도 비공개
	int		m_Parent;
public:	// 외부, 자식 모두 공개
	int		m_Public;
protected: // 외부 비공개, 자식 클래스에게는 허용
	int		m_Protected;
};

//	   부모			private		protected	public
// public 상속 :		private		protected	public
// protected 상속 :	private		protected	protected
// private 상속 :	private		private		private
class Child : public Parent
{
private:
	int		m_Child;

public:
	Child()
	{
		m_Child = 0;
		m_Public = 0;
		//m_Parent = 0; // private 은 자식 클래스에게도 공개하지 않음
		m_Protected = 0;
	}
};

int main()
{
	Parent p;
	Child c;

	// 자식 클래스의 크기는 상속받은 부모의 크기 + 자식에 추가한 맴버 크기
	int size = sizeof(Parent);
	size = sizeof(Child);


	return 0;
}