#pragma once

#include <assert.h>

// 클래스의 경우, 맴버함수를 헤더파일에 구현하면, inline 함수로 취급한다.
// 매크로 함수와 비슷한 동작으로, 컴파일 타임에 구현한 함수를 호출없이 구현해준다.


template<typename T>
struct Node
{
	T		Data;
	Node* Next;
	Node* Prev;

	Node()
		: Data()
		, Next(nullptr)
		, Prev(nullptr)
	{
	}

	Node(const T& _Data)
		: Data(_Data)
		, Next(nullptr)
		, Prev(nullptr)
	{
	}

	Node(const T& _Data, Node<T>* _Next, Node<T>* _Prev)
		: Data(_Data)
		, Next(_Next)
		, Prev(_Prev)
	{
	}
};

template<typename T>
class List
{
private:
	Node<T>* m_Head;
	Node<T>* m_Tail;
	int			m_Size;

public:
	int size() { return m_Size; }

	void push_back(const T& _Data);

	// 데이터를 맨 앞에 추가
	void push_front(const T& _Data);

	T& front() { assert(m_Head); return m_Head->Data; }
	T& back() { assert(m_Tail); return m_Tail->Data; }

	// 맨 뒤 데이터 제거
	void pop_back();

	// 맨 앞 데이터 제거
	void pop_front();

	void reverse();

	void clear()
	{
		Node<T>* pNode = m_Head;

		while (pNode != nullptr)
		{
			Node<T>* pNext = pNode->Next;
			delete pNode;
			pNode = pNext;
		}

		m_Head = m_Tail = nullptr;
		m_Size = 0;
	}


	class iterator;

	iterator begin() { return iterator(this, m_Head); }
	iterator end() { return iterator(this, nullptr); }
	iterator erase(const iterator& _iter);
	void insert(const iterator& _iter, const T& _Data);
	void insert(const iterator& _Where, const iterator& _Begin, const iterator& _End);
	List<T>& operator =(const List<T>& _Other)
	{
		// 보유한 데이터 모두 삭제
		clear();

		// 입력 인자로 들어온 리스트가 보유한 데이터를 새로 입력받음
		List<T>::iterator iter = const_cast<List<T>*>(&_Other)->begin();
		List<T>::iterator enditer = const_cast<List<T>*>(&_Other)->end();

		for (; iter != enditer; ++iter)
		{
			push_back(*iter);
		}

		return *this;
	}

private:
	void reverse_recursive(Node<T>* _Node);

public:
	List();
	//List(const List<T>& _Other);
	~List();

public:
	class iterator
	{
		friend class List<T>;
	private:
		List<T>* m_Owner;
		Node<T>* m_Node;  // m_Owner 가 nullptr 가 아니고 m_Node 는 nullptr 인 경우 end iterator 이다.

	public:
		bool operator == (const iterator& _Other) const { return m_Owner == _Other.m_Owner && m_Node == _Other.m_Node; }
		bool operator != (const iterator& _Other) const { return !((*this) == _Other); }

		T& operator*()
		{
			assert(m_Owner);
			return m_Node->Data;
		}

		iterator& operator ++()
		{
			assert(m_Owner);
			assert(m_Node); // end iterator 인데, ++ 을 호출한 경우

			m_Node = m_Node->Next;

			return *this;
		}

		iterator operator++(int)
		{
			iterator Copy(*this);
			++(*this);
			return Copy;
		}

		iterator& operator --()
		{

			return *this;
		}

		iterator operator--(int)
		{
			iterator Copy(*this);
			--(*this);
			return Copy;
		}

	public:
		iterator()
			: m_Owner(nullptr)
			, m_Node(nullptr)
		{
		}

		iterator(List<T>* _Owner, Node<T>* _Node)
			: m_Owner(_Owner)
			, m_Node(_Node)
		{
		}
	};
};

template<typename T>
List<T>::List()
	: m_Head(nullptr)
	, m_Tail(nullptr)
	, m_Size(0)
{
}

template<typename T>
List<T>::~List()
{
	clear();
}

template<typename T>
void List<T>::push_back(const T& _Data)
{
	// 데이터를 저장시킬 노드를 동적할당 한다. 데이터를 채운다
	Node<T>* pNewNode = new Node<T>(_Data);

	if (0 == m_Size)
	{
		m_Head = pNewNode;
		m_Tail = pNewNode;
	}

	else
	{
		m_Tail->Next = pNewNode;
		pNewNode->Prev = m_Tail;
		m_Tail = pNewNode;
	}

	++m_Size;
}

template<typename T>
void List<T>::push_front(const T& _Data)
{
	Node<T>* pNewNode = new Node<T>(_Data);

	if (m_Size == 0)
	{
		m_Head = m_Tail = pNewNode;
	}
	else
	{
		pNewNode->Next = m_Head;
		m_Head->Prev = pNewNode;
		m_Head = pNewNode;
	}

	++m_Size;
}

template<typename T>
void List<T>::pop_back()
{
	assert(0 != m_Size);

	if (1 == m_Size)
	{
		delete m_Head;
		m_Head = m_Tail = nullptr;
	}

	else
	{
		// 리스트가 현재 마지막 노드의 이전노드를, Tail 로 가리키게 한다.
		m_Tail = m_Tail->Prev;

		// 마지막 노드를 삭제한다.
		delete m_Tail->Next;

		// 마지막 이전노드가, 삭제될 곳을 가리키지 않게 한다.
		m_Tail->Next = nullptr;
	}

	--m_Size;
}

template<typename T>
void List<T>::pop_front()
{
	assert(0 != m_Size);

	if (1 == m_Size)
	{
		delete m_Head;
		m_Head = m_Tail = nullptr;
	}

	else
	{
		// 두번째 노드를 새로운 Head 노드로 지정
		m_Head = m_Head->Next;

		// 맨 앞에 노드를 삭제하고
		delete m_Head->Prev;

		// 새로 Head 로 지정된 노드(원래 2번째 였던 노드)의 이전 주소를 nullptr 막는다.
		m_Head->Prev = nullptr;
	}

	--m_Size;
}

template<typename T>
void List<T>::reverse()
{
	reverse_recursive(m_Head);
}


template<typename T>
void List<T>::reverse_recursive(Node<T>* _Node)
{
	if (nullptr == _Node)
	{
		// List 가 head 노드랑 Tail 노드를 반대로 가리키게 한다.
		Node* pTemp = m_Head;
		m_Head = m_Tail;
		m_Tail = pTemp;
	}
	else
	{
		reverse_recursive(_Node->Next);

		Node* pTemp = _Node->Next;
		_Node->Next = _Node->Prev;
		_Node->Prev = pTemp;
	}
}

template<typename T>
typename List<T>::iterator List<T>::erase(const iterator& _iter)
{
	// iterator 가 정상적인지 확인
	assert(this == _iter.m_Owner && _iter != end());

	// 삭제시킬 노드의 다음 노드를 가리키는 iterator 생성
	iterator Next(this, _iter.m_Node->Next);

	// 삭제시킬 노드가 맨 처음이라면
	if (_iter == begin())
	{
		// 삭제시킬 노드의 다음노드가 존재하면
		if (_iter.m_Node->Next != nullptr)
		{
			// 삭제시킬 노드의 다음노드가 이전을 가리키지 않게 한다.
			_iter.m_Node->Next->Prev = nullptr;
			m_Head = _iter.m_Node->Next;
		}

		// 데이터가 1개밖에 없었다면
		else
		{
			m_Head = m_Tail = nullptr;
		}
	}

	// 마지막 데이터를 삭제 요청한 경우
	else if (_iter.m_Node == m_Tail)
	{
		// 삭제시킬 노드의 이전노드가, 삭제할 노드를 가리키지 않게 한다.
		_iter.m_Node->Prev->Next = nullptr;

		// 삭제시킬 노드의 이전 노드를 새로운 Tail 노드로 지정한다.
		m_Tail = _iter.m_Node->Prev;
	}

	// 중간에 있는 데이터를 삭제 요청한 경우
	else
	{
		// 삭제할 노드를 중심으로 좌우 노드를 연결
		_iter.m_Node->Next->Prev = _iter.m_Node->Prev;
		_iter.m_Node->Prev->Next = _iter.m_Node->Next;
	}

	// iterator 가 가리키는 노드 삭제
	delete _iter.m_Node;

	// 데이터 개수 카운팅
	--m_Size;

	// 삭제한 노드의 다음을 가리키는 iterator 반환
	return Next;
}

template<typename T>
void List<T>::insert(const iterator& _iter, const T& _Data)
{
	if (_iter == begin())
	{
		push_front(_Data);
	}
	else if (_iter == end())
	{
		push_back(_Data);
	}
	else
	{
		// insert 할 데이터를 저장하고 있는 노드를 생성시킨다.
		Node<T>* pNewNode = new Node<T>(_Data, _iter.m_Node, nullptr);

		// insert 위치의 노드의 이전노드의 다음을 새로 생성한 노드를 가리키게 한다.
		_iter.m_Node->Prev->Next = pNewNode;

		// 새로 생성한 노드가, insert 위치의 이전노드를 가리킨다.
		pNewNode->Prev = _iter.m_Node->Prev;

		// insert 위치의 노드의 이전을, 새로 생성한 노드를 가리키게 한다.
		_iter.m_Node->Prev = pNewNode;

		// 데이터 카운팅
		++m_Size;
	}
}

template<typename T>
void List<T>::insert(const iterator& _Where, const iterator& _Begin, const iterator& _End)
{
	// Where 는 list 본인 내부를 가리키는 iterator 여야 하고
	// _Begin 에서 _End 까지는 같은 list 내부에서 범위를 지정했어야 한다.
	assert(_Where.m_Owner == this && _Begin.m_Owner == _End.m_Owner);

	List<T>::iterator iter = _Begin;
	for (; iter != _End; ++iter)
	{
		insert(_Where, iter.m_Node->Data);
	}
}
