#pragma once
#include "CRenderComponent.h"


class CBillboardRender :
    public CRenderComponent
{
private:
    Vec2            m_BillboardScale;
    Ptr<ATexture>   m_Tex;

public:
    GET_SET(Vec2, BillboardScale);


public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBillboardRender);
public:
    CBillboardRender();
    virtual ~CBillboardRender();
};

