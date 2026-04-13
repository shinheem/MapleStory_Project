#include "pch.h"
#include "CMonsterSpawner.h"
#include "CMonsterScript.h"

#include "TimeMgr.h"
#include "LevelMgr.h"
#include "SpawnMgr.h"
#include "AssetMgr.h"

CMonsterSpawner::CMonsterSpawner()
	: CScript(SCRIPT_TYPE::MONSTERSPAWNER)
	, m_pMonsterPrefab()
	, m_pCurrentMonster()
	, m_eState()
	, m_fAccTime()
	, m_fRespawnDelay(30.0f)

{

}

CMonsterSpawner::~CMonsterSpawner()
{

}

void CMonsterSpawner::Begin()
{
    // 1. 프리팹 수동 로드 (UI가 없으므로 경로를 통해 직접 불러옵니다)
    // 스포너 오브젝트의 이름에 따라 다른 프리팹을 로드하도록 분기 처리할 수 있습니다.
    if (m_pMonsterPrefab == nullptr)
    {
        // 예시: 오브젝트 이름에 "Skelegon"이 포함되어 있다면 Skelegon 프리팹 로드
        if (GetOwner()->GetName().find(L"Skelegon") != wstring::npos)
        {
            // Resources::Load 등의 엔진 내부 함수를 사용하세요 (경로는 프로젝트 설정에 맞게 수정)
            m_pMonsterPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\Skelegon.pref", L"Prefab\\Skelegon.pref");
        }
        else if (GetOwner()->GetName().find(L"Skellose") != wstring::npos)
        {
            m_pMonsterPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\Skellose.pref", L"Prefab\\Skellose.pref");
        }
    }

    // 2. 스포너 등록 및 상태 초기화 (기존 코드)
    SpawnMgr::GetInst()->RegisterSpawner(this);

    m_eState = SPAWNER_STATE::IDLE;
    m_pCurrentMonster = nullptr;
    m_fAccTime = 0.f;
}

void CMonsterSpawner::Tick()
{
    switch (m_eState) {
    case SPAWNER_STATE::IDLE:
        Spawn(); // 시작하자마자 소환
        break;

    case SPAWNER_STATE::SPAWNED:
        // 소환된 몬스터가 죽었는지 체크 (IsDead() 또는 nullptr 체크)
        if (m_pCurrentMonster == nullptr || m_pCurrentMonster->IsDead()) {
            m_pCurrentMonster = nullptr;
            m_eState = SPAWNER_STATE::WAITING;
            m_fAccTime = 0.f;
        }
        break;

    case SPAWNER_STATE::WAITING:
        m_fAccTime += DT; // DeltaTime
        if (m_fAccTime >= m_fRespawnDelay) {
            Spawn();
        }
        break;
    }

}

void CMonsterSpawner::Spawn()
{
    if (nullptr == m_pMonsterPrefab) return;

    m_pCurrentMonster = m_pMonsterPrefab->Instantiate();
    if (nullptr == m_pCurrentMonster) return;

    // 1. 기본 정보 설정 (이름, 위치)
    m_pCurrentMonster->SetName(GetOwner()->GetName() + L"_Monster");
    m_pCurrentMonster->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

    // 2. [핵심 순서] 먼저 레벨에 추가한다.
    // 이 시점에 자식들이 부모 레이어(Enemy)로 덮어씌워지지만, 일단 무시하고 진행합니다.
    LevelMgr::GetInst()->GetCurLevel()->AddObject((UINT)LAYER_TYPE::Layer_Enemy, m_pCurrentMonster);

    // 3. [반격] 레벨에 들어간 "직후"에 자식들을 찾아 레이어를 강제로 '재설정'한다.
    // AddObject보다 늦게 실행되므로 우리가 설정한 값이 최종 승리합니다.
    GameObject* pAttack = m_pCurrentMonster->FindChildByName(L"Enermy_MonsterAttack").Get();
    if (pAttack)
    {
        pAttack->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack);
    }

    GameObject* pRange = m_pCurrentMonster->FindChildByName(L"Enermy_Range").Get();
    if (pRange)
    {
        pRange->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_Range);
    }

    // 4. 레이어가 확정된 상태에서 델리게이트 등록 및 초기화 실행
    const vector<Ptr<CScript>>& vecScripts = m_pCurrentMonster->GetScripts();
    for (size_t i = 0; i < vecScripts.size(); ++i)
    {
        vecScripts[i]->Begin();
    }

    m_eState = SPAWNER_STATE::SPAWNED;
}