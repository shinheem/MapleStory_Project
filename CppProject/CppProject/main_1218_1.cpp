

int Add(int a, int b)
{
	int c = a + b;
	return c;
}



class CTest
{
private:
	int		m_i;
	float	m_f;

public:
	void SetData(int _i, float _f)
	{
		m_i = _i;
		m_f = _f;
	}

	// 디폴트 대입 연산자
	void operator = (const CTest& _Other)
	{
		m_i = _Other.m_i;
		m_f = _Other.m_f;
	}

	// 반환타입을 레퍼런스로 해버리면? 
	// CTest Result;는 지역변수이기 때문에 주소를 참조하는 레퍼런스 변수를 반환값으로 주면
	// 이미 없어진 스택의 주소를 참조하는 것이기 때문
	// 반환 타입이 클래스 타입이라면 리턴될 때 새로운 객체가 레지스터 메모리에 맴버의 값과 함께 저장이 되고 대입연산자로 인해 대입
	CTest operator + (const CTest& _Other)
	{
		CTest Result;

		Result.SetData(m_i + _Other.m_i, m_f + _Other.m_f);

		return Result;
	}



public:
	CTest()
		: m_i(0)
		, m_f(0.f)
	{
	}

	// 디폴트 복사 생성자
	CTest(const CTest& _Other)
		: m_i(_Other.m_i)
		, m_f(_Other.m_f)
	{
	}
};

struct tTest
{
	int& iRef;
};


int main()
{
	// 메모리크기가 8byte인 것을 확인할 수 있음
	int size = sizeof(tTest);
	int k = 0;
	int& iRef = k;
	// 자료형의 사이즈로 확인해보면 참조하고 있는 자료형의 사이즈로 표기됨
	size = sizeof(iRef);


	int a = 10;
	int b = 20;
	int c = a + b;

	CTest t1;
	t1.SetData(10, 10.f);

	CTest t2;
	t2.SetData(20, 20.f);

	// 복사 생성자
	// 객체가 생성될때 생성자가 호출되어야 하고, 객체끼리 복사가 이루어 져야 하기 때문에
	// 기본생성자로 초기화한 후 대입연사자를 호출시키는 두번에 걸친 동작 수행은 최적화로 보기 어렵다.
	// 따라서 컴파일러는 동일한 효과를 발생시키면서 한번에 처리가 가능한 복사생성자 호출로 컴파일한다.
	CTest t3 = t2;

	// 명시적은 복사생성자 호출
	//CTest t3(t2);

	// 기본생성자로 객체를 생성시킨 뒤에, 복사를 진행하는 구문
	//CTest t3;
	//t3 = t2;


	CTest t4 = t1 + t2;

	int ii1 = sizeof(int);
	int ii2 = sizeof(int*);
	int ii3 = sizeof(int&);
	int ii4 = sizeof(char&);


	int a1 = 0;
	int& a2 = a1;

	int* pInt = &a1;

	pInt = &a2;

	pInt = &a1;

	pInt = &a2;


	return 0;
}