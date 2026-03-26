#include<time.h>
#include<random>

// 선택 정렬
template<int Size> // 함수에 인자로 입력된 배열의 크기값을 컴파일러가 읽어와 인자값으로 넣음
void SelectionSort(int(&Arr)[Size])
{
	int Min_index = 0;
	int Min_Num = 0;

	for (int j = 0; j < Size; ++j)
	{
		for (int i = j; i < Size; ++i)
		{
			if (Arr[Min_index] > Arr[i])
			{
				Min_Num = Arr[i];
				Min_index = i;
			}
		}
		if (Arr[Min_index] < Arr[j])
		{
			Arr[Min_index] = Arr[j];
			Arr[j] = Min_Num;
			Min_index = j + 1;
		}
	}

}

// 삽입 정렬
template<int Size>
void InsertionSort(int(&Arr)[Size])
{
	int temp = 0;
	for (int j = 1; j < Size; ++j)
		for (int i = j - 1; i >= 0; --i)
		{
			if (Arr[i + 1] < Arr[i])
			{
				temp = Arr[i + 1];
				Arr[i + 1] = Arr[i];
				Arr[i] = temp;
			}
			else
			{
				break;
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

	//InsertionSort(Arr);

	return 0;
}