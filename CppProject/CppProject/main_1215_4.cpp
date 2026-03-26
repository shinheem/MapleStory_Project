#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

// 유저 정보 입력 프로그램
struct UserInfo
{
	wchar_t ID[20];
	wchar_t Adress[256];
	int Age;
};

// 전역변수
// 데이터 영역 - 프로그램 시작시, 종료시
UserInfo g_UserInfo[100] = {};
int		 g_UserCount = 0;

// _Src1 이 더 우열이 높으면 -1 반환
// _Src2 이 더 우열이 높으면 1 반환
// 두 문자열이 모두 일치하면 0 반환
int StrCmp(const wchar_t* _Src1, const wchar_t* _Src2)
{
	// 두 문자열 중, 두 길이가 작은 문자열의 길이를 가져온다.
	int LeftLen = wcslen(_Src1);
	int RightLen = wcslen(_Src2);

	int Len = 0;

	// 삼항 연산자
	LeftLen < RightLen ? Len = LeftLen : Len = RightLen;

	/*if(LeftLen < RightLen)
	{
		Len = LeftLen;
	} 
	else
	{
		Len = RightLen;
	}*/

	for (int i = 0; i < Len; i++)
	{
		if (_Src1[i] < _Src2[i])
		{
			return -1;
		}
		else if (_Src1[i] > _Src2[i])
		{
			return 1;
		}

		if (LeftLen == RightLen)
			return 0;
		else if (LeftLen < RightLen)
			return -1;
		else
			return 1;


	}

	//while (1)
	//{
	//	//_Src2와 _Src1의 문자가 같을 때  result + 0
	//	if (_Src2[i] == _Src1[i])
	//	{
	//		result += 0;
	//	}
	//	//_Src2의 문자가 _Src1의 문자보다 클 때  result + 1
	//	else if (_Src2[i] > _Src1[i])
	//	{
	//		result++;
	//	} 
	//	// 이외에 모든 상황에 result - 1
	//	else
	//	{
	//		result--;
	//	}
	//}


	return 0;
}

// 문제점
// 1. 프로그램이 실행 도중에, 가변적인 유저 수에 대응이 안됨
// 2. 프로그램이 종료되면 입력한 정보가 사라진다. 
// ==> 파일 입출력을 통해 유저정보를 파일로 저장 필요

void InputUserInfo()
{
	system("cls");
	printf(" * 유저 정보 입력 * \n\n");

	printf("ID는 19자 이내로 작성할 수 있습니다.\n");
	printf("사용하실 ID 를 입력해주세요 : ");
	wscanf_s(L"%s", g_UserInfo[g_UserCount].ID, 20);


	printf("\n\n사용자의 주소를 입력해주세요 : ");
	wscanf_s(L"%s", g_UserInfo[g_UserCount].Adress, 256);

	printf("\n\n사용자의 나이를 입력해주세요 : ");
	scanf_s("%d", &(g_UserInfo[g_UserCount].Age));

	g_UserCount++;
}

void SearchUserInfo(const wchar_t* _UserID)
{
	system("cls");
	for (int i = 0; i < g_UserCount; i++)
	{
		if (StrCmp(g_UserInfo[i].ID, _UserID) == 0)
		//if (*g_UserInfo[i].ID == *_UserID) // 해당 코드는 문자열의 첫글자만 비교하기 때문에 맞지 않는코드
		{
			wprintf(L"ID : %s\n", g_UserInfo[i].ID);
			wprintf(L"Adress : %s\n", g_UserInfo[i].Adress);
			printf("Age : %d\n", g_UserInfo[i].Age);
			break;
		}
		else
		{
			wprintf(L"\n입력하신 ID와 일치하는 정보가 없습니다.");
		}
	}
}

int main()
{
	//_wsetlocale(LC_ALL, L"Korean");
	//_setmode(_fileno(stdin), _O_U16TEXT);
	//_setmode(_fileno(stdout), _O_U16TEXT);

	fflush(stdin);
	wchar_t szTest[100] = {};
	wscanf_s(L"%s", szTest, 100);

	wchar_t userId[20] = {};

	int Input = 0;

	while (1)
	{
		wprintf(L"1. 유저 정보 입력\n");
		wprintf(L"2. 유저 정보 확인\n");
		wprintf(L"3. 종료\n");

		wscanf_s(L"%d", &Input);

		bool Exit = false;

		switch (Input)
		{
		case 1:

			InputUserInfo();

			break;

		case 2:
			printf("현재까지 입력된 유저 수 : %d\n", g_UserCount);
			printf("조회하실 유저 ID 를 입력해주세요 : ");
			wscanf_s(L"%s", userId, 20);
			SearchUserInfo(userId);

			
			printf("엔터를 누르시면 목록으로 돌아갑니다.\n");
			scanf_s("%d", &Input);

			system("cls");

			break;

		case 3:
			Exit = true;
			break;
		default:
			break;
		}

		if (Exit)
			break;

		system("cls");
	}

	free(g_UserInfo);


	return 0;
}

