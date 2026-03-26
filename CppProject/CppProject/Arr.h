#pragma once

#include <assert.h>

template<typename T>
class Arr
{
private:
	T* m_Data;		// 힙 영역에 저장된 데이터를 가리키는 포인터
	int		m_Capacity;	// 저장 가능한 데이터 개수
	int		m_Size;		// 현재 데이터 저장 개수

public:
	void push_back(const T& _Data);
	T& GetData(int _Index);
	T& operator[](int _Index);
	int size() { return m_Size; }
	T& back() { assert(m_Size > 0); return m_Data[m_Size - 1]; }
	void pop_back() { assert(m_Size > 0);  --m_Size; }

	// 미리 m_Capacity 를 확보해둔다.
	void reserve(int _Size);

	// 미리 m_Size 를 확보해둔다.
	void resize(int _Size);

	// 입력으로 들어온 다른 배열과 내용물을 교환
	void swap(Arr<T>& _Other);

	// 동적배열끼리 대입(저장하고 있는 데이터가 같아져야 한다.)
	Arr<T>& operator = (const Arr<T>& _Other);

	class iterator;

	iterator begin()
	{
		if (m_Size == 0)
			return iterator(this, -1);
		else
			return iterator(this, 0); // 이름없는 객체
	}

	iterator end()
	{
		return iterator(this, -1); // 이름없는 객체
	}

	// iterator 가 가리키는 요소를 삭제하고, 삭제한 데이터 다음 요소를 가리키는 iterator 를 만들어서 반환
	iterator erase(const iterator& _iter);

	// iterator 가 가리키는 지점에 데이터를 추가한다.
	void insert(const iterator& _iter, const T& _Data);


public:
	Arr();
	Arr(const Arr<T>& _Other);
	~Arr();

public:
	class iterator
	{
		friend class Arr<T>;
	private:
		Arr<T>* m_Owner;
		int		m_Idx;   // m_Owner 가 nullptr 이 아니면서, m_Idx 이 -1, 마지막의 다음(end) 를 가리키는 상황

		//public:
		//	Arr<T>* GetOwner() { return m_Owner; }
		//	int GetIdx() { return m_Idx; }

	public:
		bool operator ==(const iterator& _Other) { return m_Owner == _Other.m_Owner && m_Idx == _Other.m_Idx; }
		bool operator !=(const iterator& _Other) { return !((*this) == _Other); }

		T& operator*()
		{
			// iterator 가 데이터를 보유하고 있는 컨테이너를 알고있어야 하고,
			// index 가 -1 이 아니어야 한다.
			assert(nullptr != m_Owner && m_Idx != -1);

			return m_Owner->m_Data[m_Idx];
		}

		iterator& operator++()
		{
			assert(-1 != m_Idx);

			++m_Idx;

			if (m_Owner->m_Size <= m_Idx)
			{
				m_Idx = -1;
			}

			return *this;
		}

		iterator operator++(int _Num)
		{
			// 복사 생성자
			iterator CopyIter(*this);

			++(*this);

			return CopyIter;
		}

		iterator& operator--()
		{
			// begin iterator 인 경우에 -- 를 호출한 경우, 더 이전을 가리킬 수 없다.
			assert(0 != m_Idx);

			// end iterator 인 경우에 -- 를 호출한 경우, 마지막 데이터를 가리키게 해야한다.
			if (-1 == m_Idx)
			{
				// end iterator 에서, 이전인 마지막 데이터를 가리키려면,
				// 최소한 컨테이너안에 데이터가 1개 이상은 있었어야 한다.
				assert(0 != m_Owner->m_Size);

				// 마지막 데이터를 가리킨다.
				m_Idx = m_Owner->m_Size - 1;
			}
			else
			{
				--m_Idx;
			}

			return *this;
		}

		iterator operator--(int)
		{
			iterator copy(*this);

			--(*this);

			return copy;
		}


	public:
		iterator()
			: m_Owner(nullptr)
			, m_Idx(-1)
		{
		}

		iterator(Arr<T>* _Owner, int _Idx)
			: m_Owner(_Owner)
			, m_Idx(_Idx)
		{
		}

		~iterator()
		{
		}
	};
};



template<typename T>
Arr<T>::Arr()
	: m_Data(nullptr)
	, m_Capacity(0)
	, m_Size(0)
{
}

template<typename T>
Arr<T>::Arr(const Arr<T>& _Other)
	: m_Data(nullptr)
	, m_Capacity(0)
	, m_Size(0)
{
	(*this) = _Other;
}

template<typename T>
Arr<T>::~Arr()
{
	if (nullptr != m_Data)
	{
		delete[] m_Data;
	}
}

template<typename T>
void Arr<T>::push_back(const T& _Data)
{
	if (m_Capacity <= m_Size)
	{
		// 데이터가 처음 입력되는 경우
		if (m_Capacity == 0)
		{
			// 기본 공간을 2칸을 확보한다.
			m_Capacity = 2;
			m_Data = new T[m_Capacity];
		}

		// 데이터가 이미 여러개 입력 되어있는 경우
		else
		{
			reserve(m_Capacity * 2);
		}
	}

	m_Data[m_Size++] = _Data;
}

template<typename T>
T& Arr<T>::GetData(int _Index)
{
	return m_Data[_Index];
}

template<typename T>
T& Arr<T>::operator[](int _Index)
{
	return m_Data[_Index];
}

template<typename T>
void Arr<T>::reserve(int _Size)
{
	// 이미 보유한 데이터 개수보다도 적은 숫자로 공간 예약을 하려고 하면 무시한다.
	if (_Size <= m_Size)
	{
		return;
	}

	T* pNew = new T[_Size];

	for (int i = 0; i < m_Size; ++i)
	{
		pNew[i] = m_Data[i];
	}

	delete[] m_Data;

	m_Data = pNew;

	m_Capacity = _Size;
}

template<typename T>
void Arr<T>::resize(int _Size)
{
	if (m_Capacity < _Size)
	{
		reserve(_Size);
	}

	m_Size = _Size;
}

template<typename T>
void Arr<T>::swap(Arr<T>& _Other)
{
	int Temp = m_Size;
	m_Size = _Other.m_Size;
	_Other.m_Size = Temp;

	Temp = m_Capacity;
	m_Capacity = _Other.m_Capacity;
	_Other.m_Capacity = Temp;

	T* pData = m_Data;
	m_Data = _Other.m_Data;
	_Other.m_Data = pData;
}

template<typename T>
Arr<T>& Arr<T>::operator=(const Arr<T>& _Other)
{
	Arr<T> temp = *_Other;



	if (m_Capacity < _Other.m_Size)
	{
		reserve(_Other.m_Size);
	}

	for (int i = 0; i < _Other.m_Size; ++i)
	{
		m_Data[i] = _Other.m_Data[i];
	}

	m_Size = _Other.m_Size;

	return *this;
}

template<typename T>
typename Arr<T>::iterator   Arr<T>::erase(const iterator& _iter)
{
	// iterator 가 erase 호출한 컨테이너 객체 내부요소를 가리키는 iterator 여야 한다.
	// iterator 가 end iterator 가 아니어야 한다.
	assert(this == _iter.m_Owner && end() != _iter);

	// 삭제할 위치로, 뒤에 있는 데이터들을 한칸씩 당겨온다.
	int Loop = m_Size - (_iter.m_Idx + 1);
	for (int i = 0; i < Loop; ++i)
	{
		m_Data[_iter.m_Idx + i] = m_Data[_iter.m_Idx + 1 + i];
	}

	// 데이터 보유개수 카운팅, 1개 줄이기
	--m_Size;

	// 반환시킬 iterator, 삭제된 다음요소를 가리키게 한다.(동적배열 특성상, 삭제된 자리가 다음 데이터가 있는 자리)
	iterator ReturnIter(_iter);

	// 입력으로 들어온 iterator 를 사용할수 없는 상태로 만든다.
	iterator* pIter = const_cast<iterator*>(&_iter);
	pIter->m_Owner = nullptr;
	pIter->m_Idx = -1;

	return ReturnIter;
}

template<typename T>
typename void Arr<T>::insert(const iterator& _iter, const T& _Data)
{
	// 데이터를 추가할 공간이 모자라면, reserve 를 통해서 추가 공간 확보
	if (m_Capacity <= m_Size)
	{
		reserve(m_Capacity * 2);
	}

	// 인서트 할 자리를 마련하기 위해서, 데이터들을 뒤로 한칸씩 민다.
	int Loop = m_Size - _iter.m_Idx;
	for (int i = 0; i < Loop; ++i)
	{
		m_Data[m_Size - i] = m_Data[m_Size - 1 - i];
	}

	// 마련된 자리에 입력된 데이터를 넣어준다.
	m_Data[_iter.m_Idx] = _Data;

	// 데이터 개수 카운팅
	++m_Size;
}
