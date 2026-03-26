#pragma once
#include "ComponentUI.h"

#include "CScript.h"

class ScriptUI :
    public ComponentUI
{
private:
    Ptr<CScript>    m_TargetScript;

    UINT            m_ItemHeight;


public:
    void SetScript(CScript* _Script);
    Ptr<CScript> GetTargetScript() { return m_TargetScript; }

public:
    virtual void Tick_UI() override;

private:
    void AddItemHeight();

public:
    ScriptUI();
    virtual ~ScriptUI();
};

