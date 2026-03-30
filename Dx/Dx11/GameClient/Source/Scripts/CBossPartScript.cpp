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
            col->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CBossPartScript::OnPlayerEnter);

            col->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CBossPartScript::OnPlayerExit);
        }
    }
}

void CBossPartScript::Tick()
{
    m_AccTime += DT;
    auto flipbook = GetOwner()->FlipbookRender();

    if (m_bAttacking)
    {
        if (flipbook && flipbook->CheckFinish())
        {
            EndAttack();
        }
        return; // 공격 중엔 재공격 방지
    }

    if (m_bPlayerInRange && m_AccTime >= m_AttackCooldown)
    {
        StartAttack();
    }
}

void CBossPartScript::StartAttack()
{
    m_bAttacking = true;
    m_AccTime = 0.f;

    if (GetOwner()->FlipbookRender())
    {
        auto fb = GetOwner()->FlipbookRender();
        fb->Play(1, 7.f, 1);
    }
}

void CBossPartScript::EndAttack()
{
    m_bAttacking = false;

    if (GetOwner()->FlipbookRender())
    {
        GetOwner()->FlipbookRender()->Play(0, 7.f, -1);
    }
}

void CBossPartScript::OnPlayerEnter(CCollider2D* _other, CCollider2D* _otherCollider)
{
    if (_otherCollider->GetOwner()->GetLayerIdx() == (UINT)LAYER_TYPE::Layer_Player)
    {
        m_bPlayerInRange = true;
    }
}

void CBossPartScript::OnPlayerExit(CCollider2D* _other, CCollider2D* _otherCollider)
{
    if (_otherCollider->GetOwner()->GetLayerIdx() == (UINT)LAYER_TYPE::Layer_Player)
    {
        m_bPlayerInRange = false;
    }
}