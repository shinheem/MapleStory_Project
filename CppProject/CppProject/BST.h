#pragma once

#include <iostream>
using namespace std;

#include "List.h"

enum class NODE_TYPE
{
	PARENT,
	LCHILD,
	RCHILD,
	END,
};

enum class NODE_COLOR
{
	RED,
	BLACK
};

template<typename T1, typename T2>
struct Pair
{
	T1	first;	// 데이터를 정렬하는 기준
	T2	second;	// 저장하고자 하는 데이터

	Pair()
		: first()
		, second()
	{
	}

	Pair(const T1& _First, const T2& _Second)
		: first(_First)
		, second(_Second)
	{
	}
};

template<typename T1, typename T2>
struct tNode
{
	Pair<T1, T2> pair;
	tNode* arrNode[(int)NODE_TYPE::END];
	NODE_COLOR redBlack;

public:
	bool IsRoot() { return nullptr == arrNode[(int)NODE_TYPE::PARENT]; }
	bool IsLChild() { return arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] == this; }
	bool IsRChild() { return arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] == this; }
	bool IsLeaf() { return !arrNode[(int)NODE_TYPE::LCHILD] && !arrNode[(int)NODE_TYPE::RCHILD]; }
	bool IsFull() { return arrNode[(int)NODE_TYPE::LCHILD] && arrNode[(int)NODE_TYPE::RCHILD]; }
	bool HasLChild() { return arrNode[(int)NODE_TYPE::LCHILD]; }
	bool HasRChild() { return arrNode[(int)NODE_TYPE::RCHILD]; }
	bool HasChild(NODE_TYPE _Type) { return arrNode[(int)_Type]; }


	tNode* GetParent() { return arrNode[(int)NODE_TYPE::PARENT]; }
	tNode* GetLChild() { return arrNode[(int)NODE_TYPE::LCHILD]; }
	tNode* GetRChild() { return arrNode[(int)NODE_TYPE::RCHILD]; }

	tNode()
		: pair()
		, arrNode{}
	{
	}

	tNode(const Pair<T1, T2>& _Pair)
		: pair(_Pair)
		, arrNode{}
	{
	}
};

template<typename T1, typename T2>
class BST
{
private:
	tNode<T1, T2>* m_Root;
	int					m_Size;

public:
	void insert(const Pair<T1, T2>& _Pair);
	void PreOrder(tNode<T1, T2>* _Node);

	tNode<T1, T2>* FindInorderSuccessor(tNode<T1, T2>* _Node);
	tNode<T1, T2>* FindInorderPredecessor(tNode<T1, T2>* _Node);

	class iterator;
	iterator begin()
	{
		// 왼쪽으로 더이상 내려갈 수 없을때 까지 왼쪽으로 내려가서 마지막 노드
		tNode<T1, T2>* pNode = m_Root;

		while (pNode != nullptr && pNode->HasLChild())
		{
			pNode = pNode->GetLChild();
		}

		return iterator(this, pNode);
	}
	iterator end() { return iterator(this, nullptr); }
	iterator find(const T1& _first);
	iterator erase(const iterator& _iter);

public:
	BST()
		: m_Root(nullptr)
		, m_Size(0)
	{
	}
	~BST()
	{
		if (nullptr == m_Root)
			return;

		// queue 에 루트노드를 넣는다.
		List<tNode<T1, T2>*> queue;
		queue.push_back(m_Root);

		// 큐가 비어있지 않다면 순회를 계속 한다.
		while (0 != queue.size())
		{
			// 큐에서 노드를 꺼낸다.
			tNode<T1, T2>* pNode = queue.front();
			queue.pop_front();

			// 꺼낸 노드의 두 자식을 큐에 넣는다.
			if (pNode->HasLChild())
				queue.push_back(pNode->GetLChild());

			if (pNode->HasRChild())
				queue.push_back(pNode->GetRChild());

			delete pNode;
		}
	}

public:
	class iterator
	{
		friend class BST<T1, T2>;
	private:
		BST<T1, T2>* m_Owner;	// 컨테이너(이진탐색트리)
		tNode<T1, T2>* m_Node;		// 데이터를 저장하고 있는 노드, m_Owner != nullptr, m_Node == nullptr ==> End Iterator

	public:
		Pair<T1, T2>& operator*()
		{
			// 정삭적인 상태인지 확인
			assert(m_Owner != nullptr && m_Node != nullptr);
			return m_Node->pair;
		}

		Pair<T1, T2>* operator->()
		{
			return &m_Node->pair;
		}


		bool operator ==(const iterator& _Other) { return m_Owner == _Other.m_Owner && m_Node == _Other.m_Node; }
		bool operator !=(const iterator& _Other) { return !((*this) == _Other); }

		iterator& operator ++()
		{
			assert(nullptr != m_Owner && nullptr != m_Node);

			m_Node = m_Owner->FindInorderSuccessor(m_Node);

			return *this;
		}

		iterator operator ++(int)
		{

		}

		iterator& operator --()
		{
			assert(nullptr != m_Owner);

			// end iterator 에게 -- 를 호출한 경우
			if (m_Node == nullptr)
			{
				// 왼쪽으로 더이상 내려갈 수 없을때 까지 왼쪽으로 내려가서 마지막 노드
				tNode<T1, T2>* pNode = m_Owner->m_Root;

				while (pNode != nullptr && pNode->HasRChild())
				{
					pNode = pNode->GetRChild();
				}

				m_Node = pNode;
			}
			else
			{
				m_Node = m_Owner->FindInorderPredecessor(m_Node);
			}

			assert(m_Node); // 이미 iterator 가 begin 이였다

			return *this;
		}

		iterator operator --(int)
		{

		}

	public:
		iterator()
			: m_Owner(nullptr)
			, m_Node(nullptr)
		{
		}
		iterator(BST<T1, T2>* _Owner, tNode<T1, T2>* _TargetNode)
			: m_Owner(_Owner)
			, m_Node(_TargetNode)
		{
		}
		~iterator()
		{
		}
	};
};

template<typename T1, typename T2>
Pair<T1, T2> Make_Pair(const T1& _First, const T2& _Second)
{
	return Pair<T1, T2>(_First, _Second);
}

template<typename T1, typename T2>
void BST<T1, T2>::insert(const Pair<T1, T2>& _Pair)
{
	// 입력으로 들어온 Pair 를 저장할 Node 를 동적할당 함
	tNode<T1, T2>* pNewNode = new tNode<T1, T2>(_Pair);

	// 트리에 데이터가 입력된 적이 없다. (이번이 최초)
	if (nullptr == m_Root)
	{
		pNewNode->redBlack = NODE_COLOR::RED;
		m_Root = pNewNode;
	}

	// 트리에 최소 데이터가 1개 이상이라면
	else
	{
		tNode<T1, T2>* pNode = m_Root;
		NODE_TYPE Type = NODE_TYPE::END;

		while (true)
		{
			// 더 작다 ==> 왼쪽으로 연결되어야 한다.
			if (pNewNode->pair.first < pNode->pair.first)
				Type = NODE_TYPE::LCHILD;
			pNewNode->redBlack = NODE_COLOR::BLACK;
			// 더 크다 ==> 오른쪽으로 연결되어야한다.
			else if (pNode->pair.first < pNewNode->pair.first)
				Type = NODE_TYPE::RCHILD;
			else // 입력된 데이터가, 기존 데이터와 동일하다.
			{
				delete pNewNode;
				return;
			}

			// 자리가 비었다.			
			if (!pNode->HasChild(Type))
			{
				pNewNode->arrNode[(int)NODE_TYPE::PARENT] = pNode;
				pNode->arrNode[(int)Type] = pNewNode;
				break;
			}
			// 이미 연결된 다른 노드가 있다.
			else
			{
				pNode = pNode->arrNode[(int)Type];
			}
		}
	}

	++m_Size;
}

template<typename T1, typename T2>
void BST<T1, T2>::PreOrder(tNode<T1, T2>* _Node)
{
	if (nullptr == _Node)
		return;

	//cout << _Node->pair.first << endl;

	PreOrder(_Node->GetLChild());

	//cout << _Node->pair.first << endl;	

	PreOrder(_Node->GetRChild());

	// cout << _Node->pair.first << endl;
}

template<typename T1, typename T2>
tNode<T1, T2>* BST<T1, T2>::FindInorderSuccessor(tNode<T1, T2>* _Node)
{
	// 오른쪽 자식이 있다.
	if (_Node->HasRChild())
	{
		// 오른쪽 자식의 왼쪽으로 내려갈 수 없을때 까지 내려간다.
		tNode<T1, T2>* pNode = _Node->GetRChild();

		while (pNode->HasLChild()) { pNode = pNode->GetLChild(); }

		return pNode;
	}
	// 오른쪽 자식이 없다.
	else
	{
		// 부모 노드와의 관계가 왼쪽 자식일때까지 올라간다.
		tNode<T1, T2>* pNode = _Node;

		// 루트까지 올라가면, 해당 노드가 중위순회에서 가장 마지막 노드다 ==> end iterator
		while (!pNode->IsRoot())
		{
			// 부모 노드와의 관계가 왼쪽 자식일때까지 올라간다.
			if (pNode->IsLChild())
			{
				return pNode->GetParent();
			}
			else
			{
				pNode = pNode->GetParent();
			}
		}

		return nullptr;
	}
}

template<typename T1, typename T2>
tNode<T1, T2>* BST<T1, T2>::FindInorderPredecessor(tNode<T1, T2>* _Node)
{
	// 왼쪽 자식이 있다.
	if (_Node->HasLChild())
	{
		// 왼쪽 자식의 오른쪽으로 내려갈 수 없을때 까지 내려간다.
		tNode<T1, T2>* pNode = _Node->GetLChild();

		while (pNode->HasRChild()) { pNode = pNode->GetRChild(); }

		return pNode;
	}
	// 왼쪽 자식이 없다.
	else
	{
		// 부모 노드와의 관계가 오른쪽 자식이 될 때 까지 
		tNode<T1, T2>* pNode = _Node;

		// 루트까지 올라가면, 해당 노드가 중위순회에서 가장 마지막 노드다 ==> end iterator
		while (!pNode->IsRoot())
		{
			// 부모 노드와의 관계가 오른쪽 자식일때까지 올라간다.
			if (pNode->IsRChild())
			{
				return pNode->GetParent();
			}
			else
			{
				pNode = pNode->GetParent();
			}
		}

		return nullptr;
	}
}

template<typename T1, typename T2>
typename BST<T1, T2>::iterator BST<T1, T2>::find(const T1& _first)
{
	// 데이터가 1개도 없는데, 뭘 찾아달라고 하면, 못찾았다, end iterator 반환
	if (nullptr == m_Root)
		return end();

	tNode<T1, T2>* pNode = m_Root;
	NODE_TYPE Type = NODE_TYPE::END;

	while (true)
	{
		// 더 작다 ==> 왼쪽으로 연결되어야 한다.
		if (_first < pNode->pair.first)
			Type = NODE_TYPE::LCHILD;
		// 더 크다 ==> 오른쪽으로 연결되어야한다.
		else if (pNode->pair.first < _first)
			Type = NODE_TYPE::RCHILD;
		else // 입력된 데이터가, 기존 데이터와 동일하다. ==> 해당 노드를 가리키는 iterator 를 만들어서 반환		
			return iterator(this, pNode);

		// 자리가 비었다. ==> 더이상 못내려간다. 여태까지 일치하는 데이터가 없었다. ==> end iterator 반환
		if (!pNode->HasChild(Type))
			break;
		// 이미 연결된 다른 노드가 있다. ==> 더 아래로 내려가면서 확인한다.
		else
			pNode = pNode->arrNode[(int)Type];
	}

	return end();
}

template<typename T1, typename T2>
typename BST<T1, T2>::iterator BST<T1, T2>::erase(const iterator& _iter)
{
	iterator NextIter(this, nullptr);

	// 1. 리프 노드를 삭제한 경우
	// - 노드를 지우고, 부모가 지워진 노드를 가리키지 않게 한다.
	if (_iter.m_Node->IsLeaf()) // 단말 노드인가
	{
		// 루트노드인 경우
		if (_iter.m_Node->IsRoot())
		{
			delete _iter.m_Node;
			m_Root = nullptr;
		}

		// 루트노드가 아닌경우
		else
		{
			// 삭제할 노드의 후속자를 가리키는 iterator 를 만든다.
			NextIter = iterator(this, FindInorderSuccessor(_iter.m_Node));

			// 리프노드를 삭제하고, 삭제된 노드를 부모가 가리키지 않게 한다.
			if (_iter.m_Node->IsLChild())
				_iter.m_Node->GetParent()->arrNode[(int)NODE_TYPE::LCHILD] = nullptr;
			else
				_iter.m_Node->GetParent()->arrNode[(int)NODE_TYPE::RCHILD] = nullptr;

			delete _iter.m_Node;
		}

		--m_Size;
	}

	// 2. 자식이 1개 있는 노드를 삭제하려는 경우
	// - 노드를 지우고, 자식 노드랑 부모노드를 연결해준다.
	else if (!_iter.m_Node->IsFull())  // 포화상태가 아니다 ==> 자식이 1개있다.
	{
		// 삭제할 노드의 후속자를 가리키는 iterator 를 만든다.
		NextIter = iterator(this, FindInorderSuccessor(_iter.m_Node));

		// 자식이 1개있고, 루트노드라면
		if (_iter.m_Node->IsRoot())
		{
			// 자식 노드를 루트로 올려야한다.
			if (_iter.m_Node->HasLChild())
				m_Root = _iter.m_Node->GetLChild();
			else
				m_Root = _iter.m_Node->GetRChild();

			m_Root->arrNode[(int)NODE_TYPE::PARENT] = nullptr;
		}
		// 삭제할 노드가 루트노드가 아니다
		else
		{
			NODE_TYPE OwnType = NODE_TYPE::END;
			NODE_TYPE ChildType = NODE_TYPE::END;

			if (_iter.m_Node->IsLChild())
				OwnType = NODE_TYPE::LCHILD;
			else
				OwnType = NODE_TYPE::RCHILD;

			if (_iter.m_Node->HasLChild())
				ChildType = NODE_TYPE::LCHILD;
			else
				ChildType = NODE_TYPE::RCHILD;

			_iter.m_Node->GetParent()->arrNode[(int)OwnType] = _iter.m_Node->arrNode[(int)ChildType];
			_iter.m_Node->arrNode[(int)ChildType]->arrNode[(int)NODE_TYPE::PARENT] = _iter.m_Node->arrNode[(int)NODE_TYPE::PARENT];
		}
		delete _iter.m_Node;
		--m_Size;
	}

	// 3. 자식이 2개 있는 노드를 삭제하려는 경우
	// - 삭제할 노드의 중위후속자 or 중위선행자 노드를 찾아서, 값을 복사받고
	//   후속자 or 선행자 노드를 메모리에서 삭제한다.
	//   이때 후속자 or 선행자 노드는 반드시 리프 노드거나, 자식이 1개있는 경우일 것이기 때문에,
	//   다시 1, 2 번 케이스 처리방식으로 삭제해주면 된다.
	else // 자식이 2개
	{
		// 삭제할 노드의 후속자를 찾아서, 데이터를 덮어 쓴다.
		tNode<T1, T2>* pSuccessor = FindInorderSuccessor(_iter.m_Node);
		_iter.m_Node->pair = pSuccessor->pair;

		erase(iterator(this, pSuccessor));

		// 후속자 노드에 있는 데이터를 삭제할 노드 위치로 복사했기 때문에, 
		// 원래 삭제하려고 했던 노드가 후속자 노드가 됐다.
		NextIter = _iter;
	}

	return NextIter;
}
