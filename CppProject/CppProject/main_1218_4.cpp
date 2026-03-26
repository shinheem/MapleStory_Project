#include "Arr.h"

int main()
{
	Arr array;

	array.push_back(10);
	array.push_back(20);
	array.push_back(30);


	array.GetData(1) = 200;

	int data = array.GetData(0);
	data = array.GetData(1);
	data = array.GetData(2);

	// 연산자 오퍼레이터 맴버함수를 이용해서, GetData 랑 동일한 기능을 수행함
	// 배열의 주소 인덱싱 연산자를 모방해서, 실제 배열은 아니지만, 동적 배열 클래스가 
	// 데이터를 연속적인 메모리 공간에 관리하고 있음을 직관적으로 보여주기 위해서, 해당 연산자 함수를 제공함
	data = array[2];

	// operator[] 함수의 반환 타입이 int& 이기 때문에, 리턴 시키는 데이터 그 자체에 대한 참조를 반환받는다.
	// 반환값을 수정하게 되면, 원본이 수정된다.
	array[0] = 100;
	int k = array[0];


	int Data[10] = { 1, 2, 3, 4, 5, };
	Data[0] = 100;

	return 0;
}