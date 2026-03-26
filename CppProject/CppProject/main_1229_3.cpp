

class CTest
{
private:
	int	m_i;

public:
	// const 맴버함수
	// 이 함수는 호출해도, 맴버를 건드리지 않습니다.
	// const 가 붙은 맴버함수 안에 코드에서는 실제로 맴버를 수정하는 코드가 없어야한다.
	void SetInt(int _Data)
	{
		m_i = _Data;
	}

	// 단순히 맴버의 값을 반환하는 Get 류 함수들은 const 맴버함수로 선언해두면 좋다.
	int GetInt() const
	{
		return m_i;
	}



public:
	CTest()
		: m_i(0)
	{
	}
};

int main()
{
	const int i = 100;

	//i = 200; // 컴파일 에러

	int* pInt = const_cast<int*>(&i);
	*pInt = 200;

	const int& iRef = i;
	//iRef = 300; // 컴파일 에러



	const CTest inst;
	//inst.SetInt(100);
	inst.GetInt();

	const CTest& TestRef = inst;
	//TestRef.SetInt(300);


	return 0;
}