#include <stdio.h>
#include <time.h>
#include <random>


// 함수 문법
// Default 인자
// 기본으로 보장되는 입력값을 미리 설정해서, 입력이 없더라도 동작 할 수 있도록 함
// Default 인자는 뒤에서 부터 세팅이 가능
void Test(int a, int b, int c = 0)
{
	printf("Test 인자 3개받는 함수 호출\n");
}

// 함수 이름 중복문제
// 함수의 이름이 같아도, 입력 인자의 개수 or 타입 이 다를 경우
// 같은 이름의 함수를 여러개 만들 수 있다. ==> 함수 오버로딩
void Test(int a)
{
	printf("Test 인자 1개받는 함수 호출\n");
}

template<int Size>
void BubbleSort(int(&Arr)[Size])
{
	for (int j = 0; j < Size - 1; ++j)
	{
		bool IsSwap = false;

		for (int i = 0; i < (Size - 1) - j; ++i)
		{
			// 배열안에 있는 데이터를 2개씩 비교를 진행
			// 왼쪽에 있는 데이터가 오른쪽 보다 더 크면 교체를 진행
			if (Arr[i] > Arr[i + 1])
			{
				IsSwap = true;

				// Temp 에 왼쪽 데이터를 복사해두고
				int Temp = Arr[i];
				// 왼쪽에 오른쪽 데이터를 복사하고
				Arr[i] = Arr[i + 1];
				// 오른쪽은 아까 왼쪽 데이터를 복사해뒀든 Temp 값을 복사받는다.
				Arr[i + 1] = Temp;
			}
		}

		if (!IsSwap)
		{
			break;
		}
	}
}

int main()
{
	Test(10);
	Test(10, 20, 30);

	int Arr[20] = {};

	// 현재 시간을 시드로 사용해서, 무작위 난수 페이지 설정
	srand(time(0));

	for (int i = 0; i < 20; ++i)
	{
		// 설정된 난수 페이지에서 숫자를 순서대로 뽑아옴
		//Arr[i] = (rand() % 100) + 1;
		Arr[i] = i + 1;
	}

	BubbleSort(Arr);

	// a 와 b 에 있는 데이터를 교체하기
	int a = 10;
	int b = 20;

	int c = a;
	a = b;
	b = c;



	return 0;
}