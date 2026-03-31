#pragma once
#include "CRenderComponent.h"

#include "AFlipbook.h"

struct FlipbookRenderData
{
    Vec2 Offset;
    Vec2 Scale;
};

class CFlipbookRender :
    public CRenderComponent
{
private:
    vector<Ptr<AFlipbook>>  m_vecFlipbook;
    vector<FlipbookRenderData>    m_vecRenderData;

    int                     m_CurFlipbook;
    int                     m_CurSprite;

    int                     m_RepeatCount;  // -1 : 반복재생, 1 이상이면 재생 횟수
    bool                    m_Finish;
    float                   m_FPS;
    float                   m_AccTime;

public:
    FlipbookRenderData* GetRenderData(int _Idx)
    {
        if (_Idx < 0 || _Idx >= (int)m_vecRenderData.size())
            return nullptr;

        return &m_vecRenderData[_Idx];
    }

    Ptr<AFlipbook> GetFlipbook(int _Idx)
    {
        if (_Idx < 0 || _Idx >= (int)m_vecFlipbook.size())
            return nullptr;
        return m_vecFlipbook[_Idx];
    }

    void SetFlipbook(int _Idx, Ptr<AFlipbook> _Flipbook)
    {
        if (m_vecFlipbook.size() <= _Idx)
            m_vecFlipbook.resize(_Idx + 1);

        if (m_vecRenderData.size() <= _Idx)
            m_vecRenderData.resize(_Idx + 1, FlipbookRenderData{ Vec2(0,0), Vec2(1,1) });

        m_vecFlipbook[_Idx] = _Flipbook;
    }

    void AddFlipbook(Ptr<AFlipbook> _Flipbook) 
    { 
         m_vecFlipbook.push_back(_Flipbook); 

         FlipbookRenderData data;

         data.Offset = Vec2(0.f, 0.f);
         data.Scale = Vec2(1.f, 1.f);

         m_vecRenderData.push_back(data);
    }

    void Play(int _FlipbookIdx, float _FPS, int _RepeatCount)
    {
        m_CurFlipbook = _FlipbookIdx;
        m_RepeatCount = _RepeatCount;
        m_FPS = _FPS;
        m_AccTime = 0.f;
    }

public:
    bool CheckFinish();

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CFlipbookRender);
public:
    CFlipbookRender();
    virtual ~CFlipbookRender();
};

