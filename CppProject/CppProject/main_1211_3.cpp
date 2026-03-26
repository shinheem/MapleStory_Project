#include <stdio.h>

#include "Math.h"

// 변수 종류
// 1. 지역변수 - 스택 영역
// 2. 전역변수 - 데이터 영역, 프로그램 실행 도중 상시 유지되는 변수
// 3. 정적변수 - 데이터 영역, static, 전역변수의 특징을 기본적으로 지님, 선언된 곳에서만 사용 가능
//			  - 함수 안에 선언한 경우, 선언한 함수 안에서만 접근가능
// 4. 외부변수 - 전역변수이기 때문에 데이터 영역을 사용, extern 선언을 이용해서 다른 파일에서도 사용할 수 있게된 전역변수

// 정적변수
static int Global_1 = 0;

int Global_2 = 0;

void TestFunc()
{
	Global_1 = 100;
}

int main()
{
	Global_1 = 200;

	Global_2 = 200;

	int c = Mul(10, 20);

	TestFunc();

	Extern = 0;

	return 0;
}