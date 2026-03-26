#pragma once
#include "CScript.h"

#include "GameObject.h"

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
    Ptr<GameObject>      m_Target;
    Ptr<ATexture>        m_Tex;
    Ptr<APrefab>         m_Missile;

    UINT                 m_Jump_Count;

    float                m_Limit;
    float                m_Speed;
    float                m_fJumpVelocity;
    float                m_fGravity;
    float                m_DropTimer;    // 발판 무시 시간

    bool                 m_bDropDown;     // 아랫점프
    bool                 m_bJump;
    bool                 m_bWalk;
    bool                 m_bOnRope;
    bool                 m_bOnPlatform;
    bool                 m_bGround;

    Vec3                 m_prevPos;

public:
    void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

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

