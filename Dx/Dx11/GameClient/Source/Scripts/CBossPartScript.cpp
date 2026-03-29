#include "pch.h"
#include "CBossPartScript.h"
#include "CCollider2D.h"
#include "../../TimeMgr.h"
#include "../../GameObject.h"

CBossPartScript::CBossPartScript()
    : CScript(SCRIPT_TYPE::BOSSPARTSCRIPT)
    , m_bPlayerInRange(false)
    , m_bAttacking(false)
    , m_AttackCooldown(2.f)  // 공격 쿨타임 2초
    , m_AccTime(0.f)
{
}

CBossPartScript::~CBossPartScript()
{
}

void CBossPartScript::Begin()
{
    // 모든 자식 Trigger Collider에 이벤트 등록
    const vector<Ptr<GameObject>>& vecChild = GetOwner()->GetChild();

    for (Ptr<GameObject> child : vecChild)
    {
        CCollider2D* col = child->Collider2D().Get();
        if (col)
        {
            // 플레이어가 Trigger에 들어오면 OnPlayerEnter 호출
            col->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CBossPartScript::OnPlayerEnter);

            // 플레이어가 Trigger에서 나가면 OnPlayerExit 호출
            col->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CBossPartScript::OnPlayerExit);
        }
    }
}

void CBossPartScript::Tick()
{
    m_AccTime += DT;

    // 플레이어가 감지되고 공격 가능하면 공격 시작
    if (m_bPlayerInRange && !m_bAttacking && m_AccTime >= m_AttackCooldown)
    {
        StartAttack();
    }
}

// 공격 시작
void CBossPartScript::StartAttack()
{
    m_bAttacking = true;
    m_AccTime = 0.f;

    if (GetOwner()->FlipbookRender())
    {
        // 공격 애니메이션 (플립북 1번)
        GetOwner()->FlipbookRender()->Play(1, 7.f, -1);
    }
}

// 공격 종료 (Idle 전환)
void CBossPartScript::EndAttack()
{
    m_bAttacking = false;

    if (GetOwner()->FlipbookRender())
    {
        // Idle 애니메이션 (플립북 0번)
        GetOwner()->FlipbookRender()->Play(0, 7.f, -1);
    }
}

// 플레이어 Trigger 입장
void CBossPartScript::OnPlayerEnter(CCollider2D* _other, CCollider2D* _otherCollider)
{
    if (_otherCollider->GetOwner()->GetLayerIdx() == (UINT)LAYER_TYPE::Layer_Player)
    {
        m_bPlayerInRange = true;
    }
}

// 플레이어 Trigger 퇴장
void CBossPartScript::OnPlayerExit(CCollider2D* _other, CCollider2D* _otherCollider)
{
    if (_otherCollider->GetOwner()->GetLayerIdx() == (UINT)LAYER_TYPE::Layer_Player)
    {
        m_bPlayerInRange = false;
    }
}