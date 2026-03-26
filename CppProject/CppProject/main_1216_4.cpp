#include <stdlib.h>
#include <stdio.h>

// 자료구조
// 동적배열
// 장점 - 인덱싱으로 데이터를 접근할 경우 가장 빠르다.
// 단점 - 데이터가 중간에 삽입되거나, 삭제되는경우 효율이 안좋다.
//        최악의 경우는 맨 앞에 데이터가 삭제되거나 추가되는 경우

// 연결형 리스트(Linked List)
// 장점 - 데이터가 중간에 삽입 삭제될때 가장 빠른구조
// 단점 - 데이터 인덱싱이 느리다

struct Node
{
	int		Data;
	Node*	pNext;
};

struct List
{
	Node*	pHead;
	int		Size;
};

void Push_Back(List* _List, int _Data)
{
	// 노드 하나를 동적할당 한다.
	Node* pNewNode = (Node*)malloc(sizeof(Node));
	pNewNode->Data = _Data;
	pNewNode->pNext = nullptr;

	// 리스트가 데이터를 처음 입력받는 경우
	if (_List->pHead == nullptr)
	{
		_List->pHead = pNewNode;
	}

	// 리스트에 입력된 데이터가 1개 이상이다.
	else
	{
		// 리스트가 현재 보유한 데이터 중에서 가장 마지막 노드를 찾아낸다.
		Node* pLastNode = _List->pHead;

		while (pLastNode->pNext != nullptr)
		{
			pLastNode = pLastNode->pNext;
		}

		// 마지막 노드가 새로 입력된 노드를 가리킨다(연결해준다)
		pLastNode->pNext = pNewNode;
	}

	// 데이터 카운팅
	++_List->Size;
}

void Push_Front(List* _List, int _Data)
{
	// 노드 하나를 동적할당 한다.
	Node* pNewNode = (Node*)malloc(sizeof(Node));
	pNewNode->Data = _Data;
	pNewNode->pNext = nullptr;

	// 첫번째 입력인 경우
	if (nullptr == _List->pHead)
	{
		// 새로 생성한 노드를 리스트가 헤드 노드로 가리킨다.
		_List->pHead = pNewNode;
	}

	// 이미 리스트가 여러개의 데이터를 보유하고 있는 경우
	else
	{
		// 새로 생성한 노드가 기존 헤드를 자신의 다음으로 가리키고
		pNewNode->pNext = _List->pHead;

		// 리스트는 새로 생성한 노드를 첫음 노드로 가리킨다.
		_List->pHead = pNewNode;
	}

	// 데이터 카운팅
	++_List->Size;
}

// 맨 뒤에 있는 데이터 삭제
void Pop_Back(List* _List)
{
	// 리스트가 데이터가 하나도 가진게 없으면, 리턴
	if (nullptr == _List->pHead)
		return;

	// 데이터가 1개인 경우
	if (1 == _List->Size)
	{
		// 헤드노드를 삭제한다.
		free(_List->pHead);
		_List->pHead = nullptr;
	}

	// 데이터가 2개 이상
	else
	{
		Node* pNode = _List->pHead;

		// 맨 마지막 이전노드를 찾는다.
		while (pNode->pNext->pNext != nullptr)
		{
			pNode = pNode->pNext;
		}

		free(pNode->pNext);
		pNode->pNext = nullptr;
	}

	// 데이터 카운팅
	--_List->Size;
}

// 맨 앞에 있는 데이터 삭제
void Pop_Front(List* _List)
{
	// 리스트가 데이터가 하나도 가진게 없으면, 리턴
	if (nullptr == _List->pHead)
		return;

	// 데이터가 1개인 경우
	if (1 == _List->Size)
	{
		// 헤드노드를 삭제한다.
		free(_List->pHead);
		_List->pHead = nullptr;
	}

	// 데이터가 2개 이상
	else
	{
		Node* pNext = _List->pHead->pNext;

		free(_List->pHead);

		_List->pHead = pNext;
	}

	// 데이터 카운팅
	--_List->Size;
}

void Release(List* _List)
{
	Node* pNode = _List->pHead;

	while (pNode != nullptr)
	{
		Node* pNext = pNode->pNext;
		free(pNode);
		pNode = pNext;
	}
}


int main()
{
	List list = {};
	Push_Back(&list, 10);
	Push_Back(&list, 20);
	Push_Back(&list, 30);

	Pop_Front(&list);


	Node* pNode = list.pHead;
	for (int i = 0; i < list.Size; ++i)
	{
		printf("%d\n", pNode->Data);
		pNode = pNode->pNext;
	}


	// 메모리 해제
	Release(&list);

	return 0;
}