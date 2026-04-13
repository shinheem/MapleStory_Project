#pragma once
#include "CScript.h"
class CLeftBossAloneScript :
    public CScript
{
private:
   Vec3         m_TargetPos;
   Vec3         m_StartPos;
   float        m_Time;

   BOSS_STATE   m_State;
   float        m_AccTime; // 대기 시간 측정용

   Ptr<APrefab> m_Attack1Prefab; // 장판 프리펩
   Ptr<APrefab> m_Attack2Prefab; // 비석 프리펩

public:
    virtual void Tick() override;
    virtual void Begin() override;

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CLeftBossAloneScript)
public:
    CLeftBossAloneScript();
    virtual ~CLeftBossAloneScript();
};

