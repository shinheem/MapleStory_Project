#include <stdio.h>

// 특정 자료형에 다른 네이밍을 부여함
typedef int INT;

// 구조체
// 사용자 정의 자료형 - 프로그래머가 직접 설계하고 만드는 자료형
// 구조체 선언 시 typedef를 동시에 진행하는 경우
// C에서는 구조체를 사용할 때 앞에 struct를 꼭 붙여야 해서, struct 키워드를 포함한 구조체 전체 이름을
// 다른 이름으로 재정의 해서 편하게 사용하기 위함.
// C++ 에서도 해당 발식으로 구조체를 선언하면, C문법도 동식에 만족하기 때문에,
// 두 언어에서 범용적으로 사용할 구조체를 설계할 때 사용함.
typedef struct MY_TYPE
{
	int		i; // 멤버
	float	f;
} myType;

// 구조체 안에, 다른 구조체 자료형을 멤버로 선언할 수 있다.
struct MY_OTHER_TYPE
{
	int		i;
	MY_TYPE	type;
};

int main()
{
	MY_TYPE t = {1, 1}; // 초기화
	t.i = 10;			// 대입
	t.f = 3.14f;		// 대입

	MY_TYPE a = {};

	a.i = 0;
	a.f = 0.0f;



	return 0;
}