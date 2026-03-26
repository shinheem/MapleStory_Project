#pragma pack(1)
#include <stdio.h>

// 입력으로 들어온 문자열의 문자 개수가 몇개인지 알려주는 함수
// 문자열의 끝에는 0(널문자)
int GetStrLen(const char* _Str)
{
	int Len = 0;

	while (0 != _Str[Len])
	{
		++Len;
	}


	return Len;
}

// _Dest는 복사받을 목적지 주소, _Src는 복사시킬 원본 문자열 시작 주소, _Len 목적지의 허용 가능한 공간 개수
bool StrCpy(char* _Dest, const char* _Src, int _Len)
{
	int _Srclen = GetStrLen(_Src);

	// 목적지 공간의 허용치가, Src 문자열을 복사받을 만큼 충분하지 않은 경우
	if (_Srclen >= _Len)
		return false;

	for (int i = 0; i <= _Srclen + 1; i++)
	{
		_Dest[i] = _Src[i];
	}

	return true;
}

// _Dst에 문자열 끝에 _Src가 가리키는 문자열을 이어붙이기
bool StrCat(char* _Dest, const char* _Src, int _MaxLen)
{

	bool isTrue = true;

	int _Destlen = GetStrLen(_Dest);
	int _Srclen = GetStrLen(_Src);


	//이어붙인 문자열 길이가 목적지 허용치를 초과하는 경우
	if (_Destlen + _Srclen >= _MaxLen)
		isTrue = false;

	for (int i = 0; i < _Srclen; i++)
	{
		_Dest[_Destlen + i] = _Src[i];
	}

	return isTrue;
}

int main()
{
	// 문자열 끝을 알리는 값은 0(널문자)
	char a = 0;

	// 배열을 리터럴로 초기화할 경우도, 초기화 받을 배열의 요소 개수가
	// 문자열의 개수 + 1칸 더 여유가 있어야 한다. 널문자가 들어갈 여유공간 1칸
	char Name[6] = "abcde";

	// 문자열 갯수 count 함수
	int Len = GetStrLen(Name);

	char Name2[10] = {};

	// 문자열 복사함수
	StrCpy(Name2, Name, 10);

	char Test1[10] = "abcdef";

	char Test2[10] = "ghi";

	StrCat(Test1, Test2, 10);

	// 메모리 정렬
	// 구조체 크기
	// 가장 큰 맴버를 기준으로 메모리 정렬 규칙을 따른다.
	// 가장 큰 맴버 크기의 배수 단위로 메모리상에 맴버들이 배치되어야 한다.
	// #pragma pack(1) 전처리를 사용하면, 컴파일러가 구조체의 크기를 맴버의 합산으로 계산한다.

	struct CharInfo
	{
		char szName[20];
		int Att;
		int Def;

	};

	int size = sizeof(CharInfo);

	struct Test
	{
		char c;
		int i;
		short s;
	};

	int size = sizeof(Test);

	CharInfo info = {};
	StrCpy(info.szName, "Raimond", 20);





	return 0;
}