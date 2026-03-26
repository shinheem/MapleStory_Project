#pragma once
#include "AssetUI.h"
class FlipbookUI :
    public AssetUI
{
private:
    int            m_SelectedIndex;

public:
    virtual void Tick_UI() override;

public:
    FlipbookUI();
    virtual ~FlipbookUI();
};

