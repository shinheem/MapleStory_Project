#pragma once
#include "CScript.h"

class CMonsterScript :
    public CScript
{
private:


public:
    virtual void Begin() override;
    virtual void Tick() override;

private:
    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}

    CLONE(CMonsterScript);
public:
    CMonsterScript();
    virtual ~CMonsterScript();
};

