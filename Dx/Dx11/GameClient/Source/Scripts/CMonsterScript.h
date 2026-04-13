#pragma once
#include "CScript.h"

class CMonsterScript :
    public CScript
{
private:
    MON_STATE           m_eCurState;    // 현재 상태
    float               m_fAccTime;     // 상태 유지 시간 (랜덤 배회 등을 위함)
    int                 m_iHP;          // 몬스터 체력
    float               m_fSpeed;       // 이동 속도
    int                 m_iDir;         // 방향 (1: 오른쪽, -1: 왼쪽)
    float               m_fGravity;     // 중력 가속도 (예: 800.f)
    float               m_fVelocityY;   // 현재 Y축 낙하 속도
    bool                m_bOnGround;    // 바닥 착지 여부

    float               m_fMaxGroundY;    // 이번 프레임에서 발견한 가장 높은 바닥의 Y값
    bool                m_bGroundContact; // 이번 프레임에 바닥에 닿았는지 여부

    Vec3                m_vSpawnPos;    // 스폰 시점의 중심 위치
    float               m_fMoveLimit;

    Ptr<GameObject>     m_pRangeObj;
    Ptr<GameObject>     m_pAttackObj;
    Ptr<GameObject>     m_pSensorObj;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    void OnBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
private:
    // 상태별 업데이트 로직
    void Update_Stand();
    void Update_Move();
    void Update_Hit();
    void Update_Attack();
    void Update_Die();

    // 상태 변경 함수 (애니메이션 자동 재생)
    void ChangeState(MON_STATE _eNextState);
    void ApplyGravity();

    
    void OnOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void OnEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}

    CLONE(CMonsterScript)
public:
    CMonsterScript();
    virtual ~CMonsterScript();
};

