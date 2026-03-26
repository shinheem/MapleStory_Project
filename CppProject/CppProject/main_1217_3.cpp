#include <iostream>

//using namespace std;
using std::cout;
using std::endl;

// 연산자 오버로딩
class MyClass
{
private:
	int		m_i;
	float	m_f;

public:
	// 맴버함수
	void SetInt(int a)
	{
		m_i = a;
	}

	// 디폴트 대입 연산
	// 입력 인자로 MyClass 타입으로 파라미터를 설정하면, 복사비용이 커질 수 있기 때문에, 
	// 원본의 주소를 받는게 효율이 좋다.
	// MyClass* 로 설정하면 호출시에 변수앞에 &를 붙여야 한다. 대입 호출구문이 대입을 수행하는 것 처럼 보이지 않는다.
	// MyClass& 로 입력 파라미터 타입을 설정하면 문제를 해결할 수 있다.
	// 원본을 수정할 의도가 없음을 나타내기 위해서 const MyClass& 타입을 사용해서 입력인자를 받아온다.
	void operator = (const MyClass& _Other)
	{
		m_i = _Other.m_i;
		m_f = _Other.m_f;
	}

	MyClass& operator << (const char* _String)
	{
		printf(_String);

		return *this;
	}

	MyClass& operator << (void(*pFuncPointer)(void))
	{
		pFuncPointer();

		return *this;
	}

	// 클래스의 맴버끼리 더하기 연산 오퍼레이터 
	MyClass& operator + (const MyClass& _Other)
	{
		MyClass Return;
		Return.m_i = m_i + _Other.m_i;
		Return.m_f = m_f + _Other.m_f;

		return Return;
	}


	MyClass()
		
	{
	}
};

MyClass MyCout;

void MyEndl()
{
	printf("\n");
}



int main()
{
	MyClass a;
	MyClass b;
	MyClass c;

	//a.SetInt(100);
	c = a + b + c;

	a << "Hi";

	int i1 = 0;
	int i2 = 0;
	int i3 = i1;

	cout << "Hello" << "  Hi" << endl;
	MyCout << "Hello" << "   Hi" << MyEndl;
	//MyCout.operator<<("Hello").operator<<("   Hi").operator<<(MyEndl);

	return 0;
}
