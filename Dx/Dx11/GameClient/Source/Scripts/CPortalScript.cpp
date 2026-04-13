#include "pch.h"
#include "CPortalScript.h"

#include "KeyMgr.h"
#include "AssetMgr.h"
#include "LevelMgr.h"
#include "TaskMgr.h"

#include "ASound.h"

CPortalScript::CPortalScript()
	: CScript(SCRIPT_TYPE::PORTALSCRIPT)
    , m_bPlayerContact(false)
    , m_strTargetLevelPath(L"") // 기본값 빈 문자열
{
}

CPortalScript::~CPortalScript()
{
}

void CPortalScript::Begin()
{
    // 1. 플레이어 본체 콜라이더 델리게이트 등록
    Ptr<CCollider2D> col = GetOwner()->Collider2D();
    if (col != nullptr)
    {
        col->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPortalScript::OnBeginOverlap);
        col->AddDynamicOverlap(this, (COLLISION_EVENT)&CPortalScript::OnOverlap);
        col->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CPortalScript::OnEndOverlap);
    }

    // 2. [핵심] 오브젝트 이름을 확인해서 목적지 경로를 스스로 세팅
    wstring strName = GetOwner()->GetName();
    Ptr<ASound> pSound;

    if (strName.find(L"To_Dungeon") != wstring::npos)
    {
        m_strTargetLevelPath = L"Level\\Dungeon_Level.lv";
    }
    else if (strName.find(L"To_Entrance") != wstring::npos)
    {
        m_strTargetLevelPath = L"Level\\Hontail_Entrance_Level.lv";
    }
    else if (strName.find(L"To_Left") != wstring::npos)
    {
        m_strTargetLevelPath = L"Level\\Hontail_Left_Level.lv";
    }
    else if (strName.find(L"To_Right") != wstring::npos)
    {
        m_strTargetLevelPath = L"Level\\Hontail_Right_Level.lv";
    }
    else if (strName.find(L"To_Boss") != wstring::npos)
    {
        m_strTargetLevelPath = L"Level\\Hontail_Boss_Level.lv";
    }
}

void CPortalScript::Tick()
{
    if (m_bPlayerContact && KEY_TAP(KEY::UP))
    {
        if (m_strTargetLevelPath.empty()) return;

        TaskInfo task = {};
        task.Type = TASK_TYPE::ENTER_LEVEL;

        // 1. 문자열을 힙(Heap)에 동적 할당하여 프레임 끝까지 살아남게 합니다.
        // TaskMgr에서 나중에 delete[] 해줘야 합니다.
        wchar_t* pPath = new wchar_t[256];
        wcscpy_s(pPath, 256, m_strTargetLevelPath.c_str());

        task.Param_0 = (DWORD_PTR)pPath;

        TaskMgr::GetInst()->AddTask(task);

        m_bPlayerContact = false;
    }
}

void CPortalScript::OnBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
    // 충돌한 대상이 플레이어 레이어인지 확인
    if (_OtherCollider->GetOwner()->GetLayerIdx() == (UINT)LAYER_TYPE::Layer_Player)
    {
        m_bPlayerContact = true;
    }
}

void CPortalScript::OnOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol)
{

}

void CPortalScript::OnEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() == (UINT)LAYER_TYPE::Layer_Player)
    {
        m_bPlayerContact = false;
    }
}