#pragma once
#include "CScript.h"
class CMonsterSpawner :
    public CScript
{
private:
    Ptr<APrefab>        m_pMonsterPrefab;   // 소환할 몬스터 프리팹 (스켈레곤 or 스켈로스)
    GameObject*         m_pCurrentMonster;  // 현재 소환된 몬스터 포인터

    SPAWNER_STATE       m_eState;           // 스포너의 현재 상태
    float               m_fAccTime;         // 타이머용 누적 시간
    const float         m_fRespawnDelay;    // 리스폰 간격 (3초)

public:
    virtual void Tick() override;
    virtual void Begin() override;


public:
    void SetMonsterPrefab(Ptr<APrefab> _pPrefab) {
        m_pMonsterPrefab = _pPrefab;
    }

    void Spawn();
private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CMonsterSpawner)
public:
    CMonsterSpawner();
    virtual ~CMonsterSpawner();
};

