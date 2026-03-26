#include <time.h>
#include <random>

// 안정정렬, 불안정 정렬

// 버블 정렬 (안정 정렬)

// 선택 정렬 (불안정 정렬)

// 삽입 정렬 (안정 정렬)


// 퀵 소트 (불안정 정렬) nlogn
// 병합정렬 - (안정 정렬) nlogn
// 힙소트 - (불안정 정렬) nlogn

// 선택 정렬
template<int Size>
void SelectionSort(int(&Arr)[Size])
{
	for (int i = 0; i < Size - 1; ++i)
	{
		int Idx = i;

		for (int j = i + 1; j < Size; ++j)
		{
			if (Arr[Idx] > Arr[j])
			{
				Idx = j;
			}
		}

		if (Idx != i)
		{
			int Temp = Arr[i];
			Arr[i] = Arr[Idx];
			Arr[Idx] = Temp;
		}
	}
}

// 삽입 정렬
template<int Size>
void InsertionSort(int(&Arr)[Size])
{
	for (int j = 1; j < Size; ++j)
	{
		for (int i = 0; i < j; ++i)
		{
			// 데이터를 삽입할 지점
			if (Arr[j] < Arr[i])
			{
				// memcpy 는 원본데이터와 목적지 데이터의 영역이 겹칠때 문제가 생길 수 있다.
				// memmove 함수는 원본 데이터를 따로 복사해둔 뒤에, 목적지에 보내기 때문에, 
				// 복사할 데이터와 목적지의 메모리 영역이 겹쳐있어도 문제가 없다.
				int Temp = Arr[j];
				memmove(Arr + i + 1, Arr + i, sizeof(int) * (j - i));
				Arr[i] = Temp;
				break;
			}
		}
	}
}

int main()
{
	srand(time(nullptr));

	int Arr[10] = {};

	for (int i = 0; i < 10; ++i)
	{
		Arr[i] = rand() % 1000 + 1;
	}

	//SelectionSort(Arr);

	InsertionSort(Arr);


	return 0;
}