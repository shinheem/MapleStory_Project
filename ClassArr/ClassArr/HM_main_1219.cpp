
template<typename T>
class CArr
{
private:
	T* m_Arr;	// 힙메모리 영역 시작주소를 담을 맴버
	int	 m_Capasity; // 배열의 최대 크기
	int	 m_Size; // 현재 들어가있는 값

public:
	// 1. 데이터 입력 함수 push_back 만들기
	// 힙메모리 공간 m_Capasity만큼 할당
	void push_back(T _Data)
	{
		if(m_Capasity <= m_Size)
		{
			if(m_Capasity == 0)
			{

				m_Capasity = 2;
				m_Arr = new T[m_Capasity];


			} 
			else
			{


				T* temp = new T[++m_Capasity * 2];

				for (int i = 0; i < m_Size; ++i)
				{
					temp[i] = m_Arr[i];
				}

				delete[] m_Arr;

				m_Arr = temp;

				

			}
		}
		

		m_Arr[m_Size] = _Data;

		++m_Size;
	}

	// 2. 데이터를 꺼내는 GetData 함수 만들기
	T& GetData(int _Index)
	{

		return m_Arr[_Index];
	}

	// 3. GetData와 동일한 기능을 operator[] 연산자 함수로 구현하기
	T& operator[](int _Index)
	{

		return m_Arr[_Index];
	}

public:
	// 생성자
	CArr()
		: m_Arr(nullptr)
		, m_Capasity(0)
		, m_Size(0)
	{

	}

	// 소멸자
	~CArr()
	{
		if (nullptr != m_Arr) {
			delete[] m_Arr;
		}
		
	}
};

int main()
{
	CArr<int> Iarr;

	Iarr.push_back(10);
	Iarr.push_back(20);
	Iarr.push_back(30);

	int ai = Iarr.GetData(0);
	ai = Iarr.GetData(1);
	ai = Iarr.GetData(2);

	int bi = Iarr[0];
	bi = Iarr[1];
	bi = Iarr[2];

	CArr<float> farr;

	farr.push_back(10.f);
	farr.push_back(20.f);
	farr.push_back(30.f);

	float af = farr.GetData(0);
	af = farr.GetData(1);
	af = farr.GetData(2);

	float bf = farr[0];
	bf = farr[1];
	bf = farr[2];

	return 0;
}