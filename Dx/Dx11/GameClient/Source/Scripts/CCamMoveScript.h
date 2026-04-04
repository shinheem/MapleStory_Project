#pragma once
#include "CScript.h"

class CCamMoveScript :
    public CScript
{
private:
    GameObject*         m_pTarget;      // 추적 대상 (플레이어)
    float               m_fSpeed;       // 카메라 추적 속도 (Lerp용)
    Vec2                m_vLookLimit;   // 맵의 최대 가동 범위 (Left, Right, Top, Bottom 등)

public:
    virtual void Tick() override;

    void SetTarget(GameObject* _Target) { m_pTarget = _Target; }

    inline float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

private:
    void MovePerspective();
    void MoveOrthographic();

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CCamMoveScript)
public:
    CCamMoveScript();
    virtual ~CCamMoveScript();
};

