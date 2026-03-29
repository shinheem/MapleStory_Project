#pragma once
#include "CScript.h"

class CCollider2D;

class CBossPartScript : public CScript
{
private:
    bool m_bPlayerInRange;
    bool m_bAttacking;
    float m_AttackCooldown;
    float m_AccTime;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    void StartAttack();
    void EndAttack();


    void OnPlayerEnter(CCollider2D* _other, CCollider2D* _otherCollider);
    void OnPlayerExit(CCollider2D* _other, CCollider2D* _otherCollider);
private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CBossPartScript)
public:
    CBossPartScript();
    virtual ~CBossPartScript();
};

