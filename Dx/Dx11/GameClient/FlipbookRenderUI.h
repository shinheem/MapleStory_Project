#pragma once
#include "ComponentUI.h"
class FlipbookRenderUI :
    public ComponentUI
{
private:
    int         m_CurIdx;
public:
    virtual void Tick_UI() override;
    void SelectFlipbook(DWORD_PTR _ListUI);

public:
    FlipbookRenderUI();
    virtual ~FlipbookRenderUI();
};

