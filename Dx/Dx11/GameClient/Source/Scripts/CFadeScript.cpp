#include "pch.h"
#include "CFadeScript.h"

#include "TimeMgr.h"
#include "GameObject.h"

CFadeScript::CFadeScript()
	: CScript(SCRIPT_TYPE::FADESCRIPT)
{

}

CFadeScript::~CFadeScript()
{

}

void CFadeScript::Tick()
{
    if (m_eState == FADE_STATE::NONE) return;

    m_fTime += DT; // 델타 타임 누적
    float fAlpha = m_fTime / m_fDuration;

    if (m_eState == FADE_STATE::FADE_IN) fAlpha = 1.f - fAlpha; // 1 -> 0 (점점 밝아짐)

    // 재질의 투명도 조절 (보통 파라미터로 전달)
    GetOwner()->GetRenderComponent()->GetMaterial()->SetScalar(SCALAR_PARAM::FLOAT_0, fAlpha);

    if (m_fTime >= m_fDuration) {
        m_eState = FADE_STATE::NONE;
        // 여기서 레벨 이동을 실제로 실행하거나 끝났음을 알림
    }
}