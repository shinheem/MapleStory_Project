#include "pch.h"
#include "GameObject.h"

#include "LevelMgr.h"
#include "TaskMgr.h"
#include "Source/ScriptMgr.h"


GameObject::GameObject()
	: m_Com{}
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)	
	, m_Active(true)
	, m_bIsBegin(false)
{
}

GameObject::GameObject(const GameObject& _Origin)
	: Entity(_Origin)
	, m_Com{}
	, m_Parent(nullptr)
    , m_LayerIdx(-1)
    , m_Dead(false)
	, m_Active(_Origin.m_Active)
{
	// 원본 오브젝트와 동일한 세팅의 컴포넌트를 복사해서 나한테 넣어준다.
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _Origin.m_Com[i])
			continue;

		AddComponent(_Origin.m_Com[i]->Clone());
	}

	// 원본 오브젝트와 동일한 스크립트를 복사해서 나한테 넣어준다.
	for (const auto& Script : _Origin.m_vecScripts)
	{
		AddComponent(Script->Clone());
	}

	// 원본 오브젝트가 보유한 자식 오브젝트를 복사해서 나한테 넣어준다.
	for (const auto& Child : _Origin.m_vecChild)
	{
		AddChild(Child->Clone());
	}
}

GameObject::~GameObject()
{
	// 자식들에 대한 Ptr 참조를 모두 끊음
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i] = nullptr;
	}
	m_vecChild.clear();

	// 컴포넌트와 스크립트도 마찬가지
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
		m_Com[i] = nullptr;
}

void GameObject::Begin()
{
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Begin();
	}

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void GameObject::Tick()
{
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Tick();
	}	
	
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void GameObject::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_Com[i])
			m_Com[i]->FinalTick();
	}

	// 자신이 소속된 Layer 에 자기자신을 알림(등록)
	RegisterLayer();

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식 오브젝트가 있으면, Vector 에서 제거한다.
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

void GameObject::FinalTick_Editor()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->FinalTick();
	}

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식 오브젝트가 있으면, Vector 에서 제거한다.
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}



void GameObject::RegisterLayer()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();

	Layer* pLayer = pCurLevel->GetLayer(m_LayerIdx);

	pLayer->RegisterObject(this);
}


void GameObject::Render()
{

	if (!m_Active)
		return;

	// 렌더링 관련 기능을 보유한 컴포넌트가 없으면 GameObejct 는 Rendering 될 수 없다.
	if (nullptr != m_RenderCom )
	{
		Transform()->Binding();
		m_RenderCom->Render();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}	
}

void GameObject::AddComponent(Ptr<Component> _Com)
{
	if (!_Com) return; // 안전장치

	if (_Com->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		CScript* pScript = dynamic_cast<CScript*>(_Com.Get());
		if (pScript) m_vecScripts.push_back(pScript);
	}
	else
	{
		if (CRenderComponent* pRender = dynamic_cast<CRenderComponent*>(_Com.Get()))
		{
			assert(!m_RenderCom.Get());
			m_RenderCom = pRender;
		}

		UINT idx = (UINT)_Com->GetType();
		if (!m_Com[idx]) m_Com[idx] = _Com;
	}

	_Com->m_Owner = this;
	_Com->Init();
}

void GameObject::RemoveComponent(Ptr<Component> _Com)
{
	if (nullptr == _Com.Get())
		return;

	// RenderComponent
	if (_Com.Get() == m_RenderCom)
	{
		// RenderComponent 제거 시 m_Com 배열도 초기화
		UINT type = (UINT)_Com->GetType();
		if (m_Com[type].Get() == _Com.Get())
			m_Com[type] = nullptr;

		m_RenderCom = nullptr;
	}
	// Script
	else if (_Com->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		CScript* target = (CScript*)_Com.Get();

		// 기존 코드와 구조 유지, 단 .Get() 사용
		for (auto iter = m_vecScripts.begin(); iter != m_vecScripts.end(); ++iter)
		{
			if (*iter == target)  // m_vecScripts는 vector<CScript*> 형태라고 가정
			{
				m_vecScripts.erase(iter);
				break;
			}
		}
	}
	// 일반 컴포넌트
	else
	{
		UINT type = (UINT)_Com->GetType();
		if (m_Com[type].Get() == _Com.Get())
		{
			m_Com[type] = nullptr;
		}
	}

	_Com->m_Owner = nullptr;
}

void GameObject::AddChild(Ptr<GameObject> _Child)
{
	// 부모 오브젝트가 있는지 확인
	if (_Child->GetParent().Get())
	{
		// 기존 부모 오브젝트와 관계를 해제한다.
		_Child->DisconnectWithParent();
	}

	// 최상위 부모 오브젝트 였다면
	else
	{
		// 레벨 내부에 있던 오브젝트 라면
		if (_Child->m_LayerIdx != -1)
		{
			// Layer 에서 최상위 부모로 가리키던 포인터를 제거
			_Child->DeregisterAsParent();
		}		
	}

	m_vecChild.push_back(_Child); 
	_Child->m_Parent = this;

	// 자식으로 들어오려는 오브젝트가, 원래 이 레벨 소속이 아니라, 외부에서 들어온 경우
	if (_Child->m_LayerIdx == -1)
	{
		_Child->m_LayerIdx = m_LayerIdx;

		// 부모가 될 오브젝트는 레벨 내부 소속인 경우 + 레벨이 Play 모드
		if (m_LayerIdx != -1 && LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState())
		{
			// Play 중인 레벨 안에 있는 어떤 오브젝트의 자식으로서 레벨에 합류했기 때문에, 
			// Begin 호출
			_Child->Begin();
		}
	}

	// 부모 오브젝트가 레벨 소속이면
	if (m_LayerIdx != -1)
	{
		// 현재 레벨에 변경이 발생했음을 알림
		LevelMgr::GetInst()->GetCurLevel()->SetChanged();
	}
}

void GameObject::DisconnectWithParent()
{
	if (nullptr == m_Parent)
		return;

	if (m_LayerIdx != -1)
		LevelMgr::GetInst()->GetCurLevel()->SetChanged();

	vector<Ptr<GameObject>>::iterator iter = m_Parent->m_vecChild.begin();

	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}		
	}

	assert(nullptr);
}

void GameObject::RegisterAsParent()
{
	if (m_LayerIdx == -1)
		return;

	LevelMgr::GetInst()->GetCurLevel()->GetLayer(m_LayerIdx)->AddObject(this);
}

void GameObject::DeregisterAsParent()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();

	Layer* pLayer = pCurLevel->GetLayer(m_LayerIdx);

	pLayer->DeregisterAsParent(this);		
}

void GameObject::Destroy()
{
	if (m_Dead) 
		return;

	TaskInfo info = {};

	info.Type = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)this;

	TaskMgr::GetInst()->AddTask(info);
}

void GameObject::SaveToLevelFile(FILE* _File)
{
	// 이름
	SaveWString(_File, GetName());

	// RenderComponent 저장
	if (nullptr != m_RenderCom)
	{
		UINT type = (UINT)m_RenderCom->GetType();
		fwrite(&type, sizeof(UINT), 1, _File);
		m_RenderCom->SaveToLevelFile(_File);
	}

	// 일반 컴포넌트 저장
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_Com[i])
			continue;

		// 이미 m_RenderCom로 저장된 RenderComponent는 제외
		if (m_Com[i].Get() == m_RenderCom.Get())
			continue;

		UINT type = i;
		fwrite(&type, sizeof(UINT), 1, _File);
		m_Com[i]->SaveToLevelFile(_File);
	}

	// 컴포넌트 끝
	UINT ComEnd = (UINT)COMPONENT_TYPE::END;
	fwrite(&ComEnd, sizeof(UINT), 1, _File);

	// 스크립트 저장
	size_t ScriptCount = m_vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);
	for (const auto& Script : m_vecScripts)
	{
		wstring ScriptName = ScriptMgr::GetScriptName(Script.Get());
		SaveWString(_File, ScriptName);
		Script->SaveToLevelFile(_File);
	}

	// 자식 오브젝트 저장
	size_t ChildCount = m_vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);
	for (const auto& Child : m_vecChild)
	{
		Child->SaveToLevelFile(_File);
	}
}

void GameObject::LoadFromLevelFile(FILE* _File)
{
	// 이름
	SetName(LoadWString(_File));

	// 1️⃣ 컴포넌트 로드
	UINT ComType = 0;
	while (true)
	{
		fread(&ComType, sizeof(UINT), 1, _File);
		if (ComType == (UINT)COMPONENT_TYPE::END)
			break;

		Ptr<Component> pComponent = nullptr;

		switch ((COMPONENT_TYPE)ComType)
		{
		case COMPONENT_TYPE::TRANSFORM:      pComponent = new CTransform; break;
		case COMPONENT_TYPE::CAMERA:         pComponent = new CCamera; break;
		case COMPONENT_TYPE::COLLIDER2D:     pComponent = new CCollider2D; break;
		case COMPONENT_TYPE::LIGHT2D:        pComponent = new CLight2D; break;
		case COMPONENT_TYPE::MESHRENDER:     pComponent = new CMeshRender; break;
		case COMPONENT_TYPE::FLIPBOOK_RENDER:pComponent = new CFlipbookRender; break;
		case COMPONENT_TYPE::BILLBOARD_RENDER:pComponent = new CBillboardRender; break;
		case COMPONENT_TYPE::SPRITE_RENDER:  pComponent = new CSpriteRender; break;
		case COMPONENT_TYPE::TILE_RENDER:    pComponent = new CTileRender; break;
		}

		if (nullptr != pComponent)
		{
			AddComponent(pComponent); // m_RenderCom 포함 내부에서 세팅됨
			pComponent->LoadFromLevelFile(_File);
		}
	}

	// 스크립트 로드
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = LoadWString(_File);
		Ptr<CScript> pScript = ScriptMgr::GetScript(ScriptName);
		if (nullptr != pScript)
			AddComponent(pScript.Get());
		pScript->LoadFromLevelFile(_File);
	}

	// s자식 오브젝트 복구
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < ChildCount; ++i)
	{
		Ptr<GameObject> ChildObject = new GameObject;
		AddChild(ChildObject);
		ChildObject->LoadFromLevelFile(_File);
	}
}

Ptr<GameObject> GameObject::FindChildByName(const wstring& _Name)
{
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		// 1. 내 직속 자식들 중에서 이름이 일치하는지 확인
		if (m_vecChild[i]->GetName() == _Name)
		{
			return m_vecChild[i];
		}

		// 2. 내 자식의 자식들(하위 계층)에서도 검색 (재귀 호출)
		Ptr<GameObject> pChild = m_vecChild[i]->FindChildByName(_Name);
		if (pChild != nullptr)
		{
			return pChild;
		}
	}

	// 끝까지 못 찾았으면 nullptr 리턴
	return nullptr;
}