
// 함수(코드)가 저장된 코드영역
// 스택 메모리 영역

// 재귀함수 - Recursive
// 함수가 자신을 재호출하는 구조의 함수
// 계층구조 문제 해결
// 스택 오버플로우를 조심해야한다.
// 스택 오버플로우 - 재귀함수의 종료조건이 제대로 설정되지 않아서 무한히 스택이 생성되는 경우
// 재귀함수를 설계할 때에는 종료조건이 확실하게 있어야 한다.
void Add(int a, int b)
{
	static int iCount = 0;

	++iCount;

	if (iCount >= 2)
	{
		return;
	}
	else
	{
		Add(20, 20);
	}
}

// 재귀함수 활용
// 피보나치 수열
int Fibonacci(int Num)
{
	// 반복문
	if (1 == Num || 2 == Num) 
		return 1;

	int temp = 1;
	int temp1 = 1;
	int sum = 0;

	for (int i = 2; i < Num; ++i)
	{
		sum = temp + temp1;
		temp = temp1;
		temp1 = sum;
	}
	//return sum;
}

int Fibonacci_Recursive(int Num)
{
	// 재귀함수
	if (1 == Num || 2 == Num)
		return 1;

	return Fibonacci_Recursive(Num - 2) + Fibonacci_Recursive(Num - 1);
}

// 팩토리얼
long long Factorial(long long Num)
{
	if (Num == 1)
		return 1;

	long long sum = 1;
	for (int i = 1; i < Num; i++)
	{
		sum *= i;
	}
	return sum;
}

long long Factorial_Recursive(long long Num)
{
	if (Num == 1)
		return 1; 

	return Num * Factorial_Recursive(Num - 1);
}

int main()
{
	Add(10, 10);

	int Data = Fibonacci(10);

	Data = Fibonacci_Recursive(10);

	long long Data1 = Factorial(12);

	long long Data2 = Factorial_Recursive(12);

	return 0;
}

// Ctrl + Space : 미완성된 항목의 예상 안내를 받을 수 있다.
// Ctrl + Shift _ Space : 함수 인자 입력 커서 위치에서 누르면, 해당 함수의 정보를 볼 수 있다.