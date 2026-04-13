#pragma once
#include "CScript.h"
#include "CGaugeBarScript.h"
#include "ATexture.h"


enum class PLAYER_STATE
{
    STAND,
    WALK,
    ATTACK,
    JUMP
};

class CPlayerScript :
    public CScript
{
private:
    Ptr<GameObject>      m_pAttackObj;     // 공격 판정 자식 오브젝트
    Ptr<GameObject>      m_Target;
    Ptr<ATexture>        m_Tex;
    Ptr<APrefab>         m_Missile;
    // UI 스크립트 캐싱 (GaugeBarScript 포인터들)
    Ptr<CGaugeBarScript> m_pHpBar;
    Ptr<CGaugeBarScript> m_pMpBar;
    Ptr<CGaugeBarScript> m_pExpBar;
    

    // 스탯 데이터
    int                  m_iMaxHP;
    int                  m_iCurHP;
    int                  m_iMaxMP;
    int                  m_iCurMP;
    int                  m_iMaxEXP;
    int                  m_iCurEXP;
    UINT                 m_Jump_Count;

    float                m_fGetSpeed;    // 빨려 들어가는 속도
    float                m_Limit;
    float                m_Speed;
    float                m_fJumpVelocity;
    float                m_fGravity;
    float                m_DropTimer;    // 발판 무시 시간
    float                m_fInvincibleTime;
    float                m_fAccTime;
    float                m_fHitTimer;
    float                m_fSoulAngle;   // 공전 각도
    
    bool                 m_bInvincible; // 피격 후 무적 시간 체크
    bool                 m_bGet;         // 습득 중인지 여부
    bool                 m_bDropDown;     // 아랫점프
    bool                 m_bJump;
    bool                 m_bWalk;
    bool                 m_bOnRope;
    bool                 m_bOnPlatform;
    bool                 m_bGround;
    bool                 m_bIsAttacking;   // 현재 공격 중인지 여부
    bool                 m_bCollisionRegistered;
    bool                 m_bDead; // 플레이어 사망 여부

    Vec3                 m_prevPos;
    Vec3                 m_vDeadPos;     // 사망한 지점의 좌표 (중심축)

public:
    int GetMaxExp() { return m_iMaxEXP; }
    int GetCurExp() { return m_iCurEXP; }
    int GetCurHP() { return m_iCurHP; }
    int GetMaxHP() { return m_iMaxHP; }
    void PlayAnim(int _iIdx, float _fSpeed, bool _bLoop);

    void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }
    void ItemCollect();

    void TakeDamage(int _iDamage);
    void UseMana(int _iMana);
    void GainExp(int _iExp);

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    void Update_Attack();

private:
    void OnBeginOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol);
    void OnOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol);
    void OnEndOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol);
    void Change_State();
    void Shoot();

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CPlayerScript)
public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

