#include "pch.h"
#include "CRightBossAloneScript.h"

#include "TimeMgr.h"
#include "AssetMgr.h"
#include "LevelMgr.h"

#include "GameObject.h"

#include "APrefab.h"

CRightBossAloneScript::CRightBossAloneScript()
	: CScript(SCRIPT_TYPE::RIGHTBOSSALONESCRIPT)
    , m_TargetPos(Vec3(0.f, 0.f, 0.f))
    , m_StartPos(Vec3(0.f, 0.f, 0.f))
    , m_Time(0.f)
{

}

CRightBossAloneScript::~CRightBossAloneScript()
{

}

void CRightBossAloneScript::Begin()
{
    m_TargetPos = GetOwner()->Transform()->GetRelativePos();
    m_StartPos = m_TargetPos;
    m_StartPos.x -= 500.f; // 왼쪽 밖에서 시작
    GetOwner()->Transform()->SetRelativePos(m_StartPos);

    if (GetOwner()->FlipbookRender())
    {
        GetOwner()->FlipbookRender()->Play(0, 7.f, -1);
    }
}

void CRightBossAloneScript::Tick()
{
    switch (m_State)
    {
    case BOSS_STATE::APPEAR:
        if (m_Time < 1.0f)
        {
            m_Time += DT * 0.1f; // 요청하신 0.1f 속도
            Vec3 curPos = Vec3::Lerp(m_StartPos, m_TargetPos, m_Time);
            GetOwner()->Transform()->SetRelativePos(curPos);
        }
        else
        {
            m_State = BOSS_STATE::IDLE;
            m_AccTime = 0.f;
        }
        break;

    case BOSS_STATE::IDLE:
        m_AccTime += DT;
        if (m_AccTime >= 2.0f)
        {
            int iRand = rand() % 3;
            m_State = (BOSS_STATE)((int)BOSS_STATE::ATTACK1 + iRand);
            GetOwner()->FlipbookRender()->Play((int)m_State - 1, 7.f, 0);
            m_AccTime = 0.f;
        }
        break;

    case BOSS_STATE::ATTACK1:
    {
        if (m_AccTime == 0.f)
        {
            Ptr<APrefab> pPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\RightHead_Alone_Attack1.pref", L"Prefab\\RightHead_Alone_Attack1.pref");
            if (pPrefab != nullptr)
            {
                GameObject* pObj = pPrefab->Instantiate();

                pObj->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack);
                pObj->Transform()->SetRelativePos(Vec3(-4.f, -206.f, 99.f));
                pObj->Transform()->SetRelativeScale(Vec3(268.f, 77.f, 1.f));

                // 현재 레벨에 등록
                LevelMgr::GetInst()->GetCurLevel()->AddObject((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack, pObj);

                // 이펙트 스크립트에서 Play(0)를 하므로 여기서 또 안 해줘도 되지만 안전하게 두셔도 됩니다.
                if (pObj->FlipbookRender())
                    pObj->FlipbookRender()->Play(0, 10.f, 0);
            }
            pPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\RightHead_Alone_Attack1.pref", L"Prefab\\RightHead_Alone_Attack1.pref");
            if (pPrefab != nullptr)
            {
                GameObject* pObj = pPrefab->Instantiate();

                pObj->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack);
                pObj->Transform()->SetRelativePos(Vec3(48.f, 93.f, 99.f));
                pObj->Transform()->SetRelativeScale(Vec3(437.f, 77.f, 1.f));

                // 현재 레벨에 등록
                LevelMgr::GetInst()->GetCurLevel()->AddObject((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack, pObj);

                // 이펙트 스크립트에서 Play(0)를 하므로 여기서 또 안 해줘도 되지만 안전하게 두셔도 됩니다.
                if (pObj->FlipbookRender())
                    pObj->FlipbookRender()->Play(0, 10.f, 0);
            }
            pPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\RightHead_Alone_Attack1.pref", L"Prefab\\RightHead_Alone_Attack1.pref");
            if (pPrefab != nullptr)
            {
                GameObject* pObj = pPrefab->Instantiate();

                pObj->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack);
                pObj->Transform()->SetRelativePos(Vec3(346.f, 93.f, 99.f));
                pObj->Transform()->SetRelativeScale(Vec3(437.f, 77.f, 1.f));

                // 현재 레벨에 등록
                LevelMgr::GetInst()->GetCurLevel()->AddObject((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack, pObj);

                // 이펙트 스크립트에서 Play(0)를 하므로 여기서 또 안 해줘도 되지만 안전하게 두셔도 됩니다.
                if (pObj->FlipbookRender())
                    pObj->FlipbookRender()->Play(0, 10.f, 0);
            }
        }

        // 시간 누적 (중요: 그래야 다음 프레임에 또 안 만듦)
        m_AccTime += DT;

        // 보스 본체의 애니메이션이 끝났는지 확인
        if (GetOwner()->FlipbookRender()->CheckFinish())
        {
            m_State = BOSS_STATE::IDLE;
            GetOwner()->FlipbookRender()->Play(0, 7.f, -1);
            m_AccTime = 0.f;
        }
    }
    break;
    case BOSS_STATE::ATTACK2:
    {
        if (m_AccTime == 0.f)
        {
            Ptr<APrefab> pPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\RightHead_Alone_Attack2.pref", L"Prefab\\RightHead_Alone_Attack2.pref");
            if (pPrefab != nullptr)
            {
                GameObject* pObj = pPrefab->Instantiate();
                pObj->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack);

                GameObject* pPlayer = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Maple_Player").Get();
                if (pPlayer)
                {
                    Vec3 vPos = pPlayer->Transform()->GetRelativePos();
                    Vec3 vScale = pPlayer->Transform()->GetRelativeScale();

                    // 1. 플레이어의 발바닥 위치로 이동
                    vPos.y -= (vScale.y / 2.f);

                    // 2. 비석의 중심이 중앙에 있다면, 비석 세로 크기(800)의 절반만큼 위로 올림
                    // 이렇게 해야 비석의 하단 끝이 플레이어의 발바닥 위치에 딱 붙습니다.
                    vPos.y += 400.f;

                    vPos.z = 99.f;
                    pObj->Transform()->SetRelativePos(vPos);
                }

                LevelMgr::GetInst()->GetCurLevel()->AddObject((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack, pObj);

                if (pObj->FlipbookRender())
                    pObj->FlipbookRender()->Play(0, 10.f, 0);
            }
        }
        m_AccTime += DT;

        if (GetOwner()->FlipbookRender()->CheckFinish())
        {
            m_State = BOSS_STATE::IDLE;
            GetOwner()->FlipbookRender()->Play(0, 7.f, -1);
            m_AccTime = 0.f;
        }
    }
    break;
    case BOSS_STATE::SUMMON:
        if (GetOwner()->FlipbookRender()->CheckFinish())
        {
            m_State = BOSS_STATE::IDLE;
            GetOwner()->FlipbookRender()->Play(0, 7.f, -1);
            m_AccTime = 0.f;
        }
        break;
    }
}