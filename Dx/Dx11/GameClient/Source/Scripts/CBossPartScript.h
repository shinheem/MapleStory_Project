#pragma once
#include "CScript.h"

class CBossPartScript :
    public CScript
{
private:

public:
    virtual void Tick() override;

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CBossPartScript)
public:
    CBossPartScript();
    virtual ~CBossPartScript();
};

