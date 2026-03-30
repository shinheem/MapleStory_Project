#pragma once
#include "CScript.h"
class CInventoryScript :
    public CScript
{
private:
    Ptr<GameObject> m_pInventoryUI;

public:
    virtual void Tick() override;
    virtual void Begin() override;

    void SetInventoryUI(Ptr<GameObject> _UI) { m_pInventoryUI = _UI; }

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CInventoryScript)
public:
    CInventoryScript();
    virtual ~CInventoryScript();

};

