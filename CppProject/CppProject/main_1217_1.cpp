

// C 에서 구조체(struct) - 사용자 정의 자료형
// C++ 에서는 class - 사용자 정의 자료형

// 객체지향
// 1. 캡슐화
// 2. 상속
// 3. 다형성
// 4. 추상화

// C 에서의 struct 에서 c++ class 로 변경하면서 추가된 기능들
// C++ 에서 struct 와 class 차이, 맴버의 기본 접근 제한 지정자가 
// class 는 private, struct 는 public 으로 취급된다.

// 은닉성 - private, protected, public

// private: 외부에 맴버를 공개하지 않는다.
// public: 외부에 맴버를 공개한다.




namespace MY_SPACE
{
	int g_Global = 0;

	class MyType
	{
	private: // 외부에 맴버를 공개하지 않는다.
		int		a;
		float	f;

		// 맴버함수, 클래스 내부에 구현된 함수
		// 해당 클래스를 지원해주는 전용 함수
	public:
		// 맴버함수를 호출할때에는 반드시 호출 객체가 있어야 한다.
		// 맴버 함수 안에는 this 지역변수가 숨어있다.
		// this 는 맴버 함수를 호출한 객체의 주소를 받는다.
		void SetInt(int _Data)
		{
			// 맴버 함수 안에서 맴버를 접근할 때, 앞에 this-> 가 생략되어있다.
			this->a = _Data;
		}

		void SetFloat(float _Data)
		{
			this->f = _Data;
		}

		int GetInt()
		{
			return this->a;
		}

		// 객체가 생성될때 호출
		// 생성자

		// 기본 생성자 - 입력인자를 받지않는 생성자
		MyType()
			// 초기화, 맴버가 const 로 선언된 경우 이니셜라이저로만 초기값을 세팅할 수 있다.
			: a(10), f(1.11f)
		{
			// 대입
			this->a = 10;
			this->f = 1.11f;
		}

		// 생성자 오버로딩, 인자 개수 or 타입을 다르게해서, 생성자를 여러 버전을 만들어 둘 수 있다.
		MyType(int _i, float _f)
			: a(_i)
			, f(_f)
		{

		}

		// 객체가 소멸할때 호출
		// 소멸자
		~MyType()
		{

		}
	};

	int Add(int a, int b)
	{
		return a + b;
	}
}

int g_Global = 0;


// 같은 이름의 namespace 는 언제든지 작성 가능, 추가 누적됨
namespace MY_SPACE
{
	int aaa = 0;
}

// namespace 무효화
//using namespace MY_SPACE;
using MY_SPACE::aaa;
using MY_SPACE::MyType;

int main()
{
	// c++ 에 추가된것
	// namespace
	// 변수명 중복을 피하기 위해서
	g_Global = 100;

	MY_SPACE::g_Global = 200;


	// 기본생성자가 아니라, 오버로딩된 다른 생성자를 호출한다.
	MyType type;

	MyType type1(100, 3.14f);

	MyType type2(100, 3.14f);

	type2.SetInt(100);


	int c = MY_SPACE::Add(10, 20);

	aaa = 100;


	// 레퍼런스




	return 0;
}