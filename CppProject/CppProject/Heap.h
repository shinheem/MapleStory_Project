#pragma once

#include "Arr.h"

enum HEAP_MODE
{
	MIN,
	MAX,
};


template<typename T>
class Heap
	: public Arr<T>
{
private:
	HEAP_MODE	m_Mode;

protected:
	// 동적배열(Arr) 에서 제공하는 데이터 추가함수를 비공개처리
	using Arr<T>::push_back;
public:
	// Heap 에서 자체적으로 정의한 push 함수를 이용해서 데이터 추가
	void push(const T& _Data);

	// 가장 작은값을 반환해준다.
	T Get();

	void SetMode(HEAP_MODE _Mode);

private:
	bool Compare(int _ParentIdx, int _ChildIdx)
	{
		if (HEAP_MODE::MIN == m_Mode)
			return Arr<T>::GetData(_ParentIdx) < Arr<T>::GetData(_ChildIdx);
		else
			return Arr<T>::GetData(_ParentIdx) > Arr<T>::GetData(_ChildIdx);
	}


public:
	Heap()
		: m_Mode(HEAP_MODE::MIN)
	{
	}

	~Heap()
	{
	}
};

template<typename T>
void Heap<T>::push(const T& _Data)
{
	// 데이터를 추가한다.
	// 부모클래스 Arr 에 구현되어있는 push_back 을 재활용해서, 데이터를 가장 뒤에 채운다.
	Arr<T>::push_back(_Data);

	// 최근에 추가한 데이터를 부모와 비교해서 올린다.
	int CurIdx = Arr<T>::size() - 1;

	// 루트 노드가 아니고, 부모와 자식 관계가 올바른 상황이 아니라면
	while (CurIdx != 0 && !Compare((CurIdx - 1) / 2, CurIdx))
	{
		// 부모 노드와 현재노드의 값을 교환한다.
		int Temp = Arr<T>::GetData(CurIdx);
		Arr<T>::GetData(CurIdx) = Arr<T>::GetData((CurIdx - 1) / 2);
		Arr<T>::GetData((CurIdx - 1) / 2) = Temp;

		// 현재 데이터 인덱스를 부모 위치로 변경한다.
		CurIdx = (CurIdx - 1) / 2;
	}
}

template<typename T>
T Heap<T>::Get()
{
	// 데이터가 최소 1개 이상 있어야 한다.
	assert(Arr<T>::size() > 0);

	// 가장 작은값(루트 == 0번 인덱스) 을 복사해둔다.
	T Data = Arr<T>::GetData(0);

	// 가장 뒤에있는 데이터를 루트로 복사
	Arr<T>::GetData(0) = Arr<T>::back();

	// 맨 뒤 데이터를 제거
	Arr<T>::pop_back();

	// 루트로 올린 데이터를 제자리를 찾아갈 수 있도록 자식과 비교해서 내린다.
	int CurIdx = 0;
	while (true)
	{
		// 왼쪽, 오른쪽 자식 인덱스, 둘중 더 올바른 자식을 찾아야 한다.
		int LeftChild = CurIdx * 2 + 1;
		int RightChild = CurIdx * 2 + 2;
		int MinIdx = 0;

		// 왼쪽 자식이 없으면, 더 내려갈 수 없다.
		if (Arr<T>::size() <= LeftChild)
			break;

		// 오른쪽 자식이 없으면, 두 자식중 왼쪽 자식이랑 비교한다.
		if (Arr<T>::size() <= RightChild)
			MinIdx = LeftChild;
		else // 자식이 둘다 있으면, 둘을 비교해서 더 맞는 자식을 찾는다.
			Compare(LeftChild, RightChild) ? MinIdx = LeftChild : MinIdx = RightChild; 

		// 두 자식중, 더 알맞은 노드랑 비교했는데 트리 구조에 위배되지 않는다면
		if (Compare(CurIdx, MinIdx))
			break;

		T Temp = Arr<T>::GetData(CurIdx);
		Arr<T>::GetData(CurIdx) = Arr<T>::GetData(MinIdx);
		Arr<T>::GetData(MinIdx) = Temp;

		// 현재 인덱스를 자식인덱스로 교환해서 계속 내려가는 작업을 반복한다.
		CurIdx = MinIdx;
	}

	// 가장 작은 값을 반환한다.
	return Data;
}

template<typename T>
void Heap<T>::SetMode(HEAP_MODE _Mode)
{
	// 이미 동일한 모드인 경우 무시
	if (m_Mode == _Mode)
		return;

	// 데이터 개수가 1개밖에 없으면, 재 구성할 필요 없음
	if (Arr<T>::size() <= 1)
	{
		m_Mode = _Mode;
		return;
	}

	// 힙이 보유한 데이터가 2개 이상이라면, 변경된 모드로 다시 구성해야 한다.
	// 임시 지역 배열을 만들어서 Heap 이 보유한 데이터를 전부 꺼내서 배열에 넣는다.
	Arr<T> arrTemp;
	arrTemp.reserve(Arr<T>::size());

	int size = Arr<T>::size();
	for (int i = 0; i < size; ++i)
	{
		arrTemp.push_back(Get());
	}

	// 힙 모드를 변경한다.
	m_Mode = _Mode;

	// 꺼낸 데이터를 역순으로 heap 에 다시 넣어서, 최대한 재 구성할때 연산이 덜 발생하게 한다.
	for (int i = 0; i < size; ++i)
	{
		push(arrTemp[size - (i + 1)]);
	}
}
