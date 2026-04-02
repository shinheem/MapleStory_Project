#pragma once
#include "CScript.h"

class CGaugeBarScript
	: public CScript
{
private:
    float m_fCurrent;
    float m_fMax;

    float m_fTargetRatio;
    float m_fDisplayRatio;

    float m_fSpeed;


public:
    void SetValue(float current, float max)
    {
        m_fCurrent = clamp(current, 0.f, max);
        m_fMax = max;

        m_fTargetRatio = (max == 0.f) ? 0.f : m_fCurrent / m_fMax;
    }

    inline float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    virtual void Tick() override;
    virtual void Begin() override;

private:
    void UpdateMaterial(float ratio);

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CGaugeBarScript)

public:
    CGaugeBarScript();
    virtual ~CGaugeBarScript();
};