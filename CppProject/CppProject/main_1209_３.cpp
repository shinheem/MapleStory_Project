
#include <Windows.h>

#define BLEED		0x1 
#define COLD		0x2
#define POISION		0x4
#define ELECTRIC	0x8

#define FIRE		0x10
#define ATT_DOWN	0x20
#define DEF_DOWN	0x40
#define BLESS		0x80

#define DEX_UP		0x100
#define AGI_UP		0x200


int main()
{
	// 정수 bool(1) char(1), short(2), int(4), long(4)
	// 실수 float(4), double(8)

	// bool 자료형은 참 또는 거짓 상태를 저장하기위한 전용 자료형
	// bool 자료형으로 선언된 변수에 값을 넣을때, 0 이 아닌수는 참의 대표숫자인 1로 변경되어서 저장된다.
	// 인텔리센스 서포팅으로 인해서 저장된 값이 0 인경우 false 로, 1 인경우 true 로 문자열 형태로 보여진다.	
	char Char = 1;
	short s = 0;
	bool Bool = 100;

	// Bool 에 저장했던 100 이 나오지 않고 1이 나온다.
	s = Bool;



	// 연산자 
	// 비트 연산자
	// &(곱)
	int a = 10;
	int b = 15;
	int c = a & b;

	// |(합)
	c = a | b;

	// 비트 XOR
	// ^ - 비트단위로 진행, 같으면 0, 다르면 1
	c = a ^ b;

	// 비트 반전
	// ~, 피연산자 1개
	c = ~c;





	// 자신에게 연산을 수행하고 자신에게 다시 대입하는 경우를 축약해서 표현
	int j = 0;
	j += 3;

	// 상태를 저장하는 변수
	unsigned int Status = 0;

	// 특정 칸에 비트값을 집어넣기
	Status |= BLEED; // Status = Status | BLEED;
	Status |= COLD;	// Status = Status | COLD;

	// 특정 칸에 비트값이 1인지 확인하기
	if (Status & BLEED)
	{

	}

	// 특정 칸에 비트값을 제거하기
	Status &= ~COLD; // COLD 상태자리에 비트가 있으면 제거하고, 없으면 아무일도 없다.
	Status ^= COLD;  // COLD 상태자리에 비트가 있으면 제거하고, 없으면 넣어준다

	return 0;
}

// 단축키 F12 : 선언 위치로 이동