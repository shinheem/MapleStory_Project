


#define SET_INT(Inst, Data) Inst.m_i = Data

class CTest
{
public:
	int		m_i;
	float	m_f;

public:
	// 클래스 안에 구현한(헤더파일에 작성한) 맴버함수는 인라인 함수로 취급됨
	// 동작 원리는 매크로 함수와 같다, 함수 구현내용을 호출한 쪽에 작성해서 코드를 치환한다.
	// 실제로 함수 호출이 발생하지 않기 때문에 속도면에서 더 빠르다.

	// 전처리가 다음 단계인 컴파일 타임에 수행되기 때문에, 디버깅 정보가 생성되어서, 
	// 마치 실제 함수를 호출한 것 처럼 디버깅이 가능하다. --> 매크로 함수와의 차이점

	// 전역함수와 다르게, 헤더에 작성하더라도, 매크로 처럼 동작하기 때문에
	// 해당 헤더파일을 include 한 cpp 마다 함수 정의가 생겨나지 않는다.

	// Get, Set 종류
	// 자주 호출됨, 코드가 몇줄 안됨, 인라인 함수로 만들기 가장 좋은 상황

	// 헤더에 구현했다고 해서 무조건 inline 함수로 만들어주지 않음, (컴파일러 마다 기준이 있음)
	void SetInt(int _i) // -> inline
	{
		m_i = _i;
	}

	void SetFloat(float _f)
	{
		m_f = _f;
	}

public:
	CTest()
		: m_i(0)
		, m_f(0.f)
	{
	}

	~CTest()
	{
	}
};




int main()
{
	CTest t;

	//t.m_i = 10;
	t.m_i = 10;

	t.SetInt(10);
	t.m_i = 10;


	return 0;
}

