#include <stdio.h>



int main()
{
	// 문자
	// 문자는 실제로 숫자 데이터이다.
	// 특정 숫자를 특정 문자로 매칭시킨 표를 아스키코드 표라고 한다.
	// 어떤 숫자가 어떤 문자로 인식될지 이미 다 약속되어있다.
	int i = 67;
	char c = 67; // 67은 대문자 C로 약속되어있다.

	char Str[10] = { 97, 98, 99, };

	short arrShort[10] = { 97, 98, 99, };

	short s = 'C';

	// 문자열 - 리터럴
	// 문자가 연속된 메모리 공간에 나열됨, 끝에 널문자가 존재 해야함
	
	// 리터럴
	// 문자열 중에서 코드상에 작성된 문자열, 프로그램 시작 시 ROM(Read Only Memory) - 코드영역에 저장됨
	// 문자열의 원본 역할을 수행함
	// 동일한 문자열이 코드에 여러번 선언되어도 실제 컴파일된 코드에는 문자열이 하나만 존재한다.


	// 리터럴 문자열로 Name 배열을 초기화
	char Name[10] = "abcdef";// 리터럴 문자열로 Name 배열을 초기화

	Name[0] = 'z';

	char Name2[10] = "abcdef";
	char z = Name2[0];
	Name2[0] = 'k';

	//int iArr[10] = "abcdef"; -> 리터럴로 배열을 초기화 하는 구문은 문자 전용 자료형인 char, wchar_t만 가능

	//short sShort[10] = "abcdef";
	wchar_t wstr[10] = L"abcdef";

	// 리터럴의 시작 주소를 받아옴.
	const char* pString = "abcdef";
	char t = pString[0];
	//String[0] = 'k'; 리터럴을 직접 수정하게 된다 ==> ROM 영역 훼손

	return 0;
}