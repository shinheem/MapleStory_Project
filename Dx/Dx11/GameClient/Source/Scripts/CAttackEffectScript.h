#pragma once
#include "CScript.h"
class CAttackEffectScript :
    public CScript
{
private:

public:
    virtual void Tick() override;
    virtual void Begin() override;

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CAttackEffectScript)
public:
    CAttackEffectScript();
    virtual ~CAttackEffectScript();
};

