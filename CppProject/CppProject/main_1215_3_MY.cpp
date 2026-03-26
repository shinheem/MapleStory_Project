#include <stdio.h>

// 입력으로 들어온 문자열의 문자 개수가 몇개인지 알려주는 함수
// 문자열의 끝에는 0(널문자)
int GetStrLen(const char* _Str)
{

	int result = 0;

	if (_Str[0] == '\0')
	{
		result = 0;
	}

	for (int i = 0; _Str[i] != '\0'; i++)
	{
		result++;
	}


	return result;
}

// Src가 가리키는 문자열에서 _Len에 적힌 숫자만큼 문자를 복사해서 _Dest가 가리키는 곳으로 복사한다.
bool StrCpy(char* _Dest, const char* _Src, int _Len)
{
	bool isTrue = true;

	if (_Src[0] == '\0')
	{
		isTrue = false;
	}

	for (int i = 0; i <= _Len; i++)
	{
		_Dest[i] = _Src[i];
	}

	return isTrue;
}

// _Dst에 문자열 끝에 _Src가 가리키는 문자열을 이어붙이기
bool StrCat(char* _Dest, const char* _Src)
{

	bool isTrue = true;

	int _Destlen = GetStrLen(_Dest);
	int _Srclen = GetStrLen(_Src);

	if (_Destlen + _Srclen > 10)
	{
		isTrue = false;
	}

	for (int i = 0; i < _Srclen; i++)
	{
		_Dest[_Destlen + i] = _Src[i];
		//_Destlen++; //--> 해당 부분의 증가 연산자로 인해 밑에 부분 코드의 인덱스가 오버됨
	} 

	_Dest[_Destlen + _Srclen] = '\0';

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
	StrCpy(Name2, Name, GetStrLen(Name));

	char Test1[10] = "abcdef";

	char Test2[10] = "ghi";

	StrCat(Test1, Test2);

	return 0;
}