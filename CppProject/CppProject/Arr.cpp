#include "Arr.h"

Arr::Arr()
	: m_Data(nullptr)
	, m_Capacity(0)
	, m_Size(0)
{
}

Arr::~Arr()
{
	if (nullptr != m_Data)
	{
		delete[] m_Data;
	}
}

void Arr::push_back(const int& _Data)
{
	if (m_Capacity <= m_Size)
	{
		// 데이터가 처음 입력되는 경우
		if (m_Capacity == 0)
		{
			// 기본 공간을 2칸을 확보한다.
			m_Capacity = 2;
			m_Data = new int[m_Capacity];
		}

		// 데이터가 이미 여러개 입력 되어있는 경우
		else
		{
			int* pNew = new int[m_Capacity * 2];

			for (int i = 0; i < m_Size; ++i)
			{
				pNew[i] = m_Data[i];
			}

			delete[] m_Data;

			m_Data = pNew;

			m_Capacity *= 2;
		}
	}

	m_Data[m_Size++] = _Data;
}

int& Arr::GetData(int _Index)
{
	return m_Data[_Index];
}

int& Arr::operator[](int _Index)
{
	return m_Data[_Index];
}
