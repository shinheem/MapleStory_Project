#include <stdlib.h>
#include <string.h>

// 지역변수, 전역변수, 정적변수, 외부변수
// 스택		데이터

// 스택
// 데이터
// 코드(ROM)
// 힙 (동적할당)

int main()
{
	void* pVoid = nullptr;
	//char* pChar = pVoid;

	// 동적 할당
	// 힙 메모리 영역에, 필요한 바이트 수 만큼 메모리를 요청하고 할당된 메모리의 시작주소를 받는다.
	// 메모리의 시작 주소는  void*타입이다. 요청한 쪽에서 어떤 용도로 사용할지 모르기 때문
	// 동적할당을 요청한 측에서는, 자신이 요청한 메모리 공간을 어떤 목적으로 사용할 것인지에 따라
	// 알맞은 자료형 포인터를 이용해서 항당받는 곳을 가리키고, 접근한다.

	// Memory Allocate
	// 4바이트 동적할당, int 1개 저장 목적으로 사용
	int * pInt = (int*)malloc(4);
	*pInt = 100;

	// 4바이트 동적할당, char 4개 배열 목적으로 사용
	char* pChar = (char*)malloc(4);
	pChar[0] = 1;
	pChar[0] = 2;
	pChar[0] = 3;
	pChar[0] = 4;

	free(pInt);
	free(pChar);

	// 문자열 비교
	char ID[20] = {};
	char ID_OTHER[20] = {};

	return 0;
}