#include "pch.h"
#include "CGaugeBarScript.h"

#include "../../GameObject.h"
#include "../../AMaterial.h"
#include "../../CRenderComponent.h"

#include "../../TimeMgr.h"
#include "../../KeyMgr.h"


CGaugeBarScript::CGaugeBarScript()
	: CScript(SCRIPT_TYPE::GAUGEBARSCRIPT)
	, m_fCurrent(1.f)
	, m_fMax(1.f)
	, m_fTargetRatio(1.f)
	, m_fDisplayRatio(1.f)
	, m_fSpeed(10.f)
{
}

CGaugeBarScript::~CGaugeBarScript()
{
}

void CGaugeBarScript::Begin()
{
	Ptr<CRenderComponent> renderComp = GetOwner()->GetRenderComponent();
	if (renderComp)
	{
		auto mtrl = renderComp->GetMaterial()->Clone();
		renderComp->SetMaterial(mtrl);                 
	}

	if (GetOwner()->GetName() == L"EXP_Bar")
	{
		m_fCurrent = 0.3f;
		float newCurrent = m_fCurrent - (m_fMax * 0.1f);
		SetValue(newCurrent, m_fMax);
	}
	
}

void CGaugeBarScript::Tick()
{

	if (GetOwner()->GetName() == L"HpBar" && KEY_TAP(KEY::A))
	{
		float newCurrent = m_fCurrent - (m_fMax * 0.1f);
		SetValue(newCurrent, m_fMax);
	}
	else if (GetOwner()->GetName() == L"HpBar" && KEY_TAP(KEY::S))
	{
		float newCurrent = m_fCurrent + (m_fMax * 0.1f);
		SetValue(newCurrent, m_fMax);
	}

	m_fDisplayRatio = Lerp(m_fDisplayRatio, m_fTargetRatio, DT * m_fSpeed);
	UpdateMaterial(m_fDisplayRatio);
}

void CGaugeBarScript::UpdateMaterial(float ratio)
{
	AMaterial* mtrl = GetOwner()->GetRenderComponent()->GetMaterial().Get();
	AMesh* mesh = GetOwner()->GetRenderComponent()->GetMesh().Get();

	if (mtrl)
	{
		mtrl->SetScalar(FLOAT_0, ratio);
		mtrl->Binding();
	}
}
