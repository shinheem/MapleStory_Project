#pragma once
#include "CScript.h"
class CFadeScript :
    public CScript
{
private:
    float               m_fTime;      // 진행 시간
    float               m_fDuration;  // 목표 시간 (예: 1초)
    FADE_STATE          m_eState;     // 현재 상태


public:
    void FadeIn(float _fTime) { m_fDuration = _fTime; m_fTime = 0.f; m_eState = FADE_STATE::FADE_IN; }
    void FadeOut(float _fTime) { m_fDuration = _fTime; m_fTime = 0.f; m_eState = FADE_STATE::FADE_OUT; }

public:
    virtual void Tick() override;

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CFadeScript)
public:
    CFadeScript();
    virtual ~CFadeScript();
};

