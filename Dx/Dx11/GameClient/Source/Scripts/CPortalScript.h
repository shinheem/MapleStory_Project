#pragma once
#include "CScript.h"
class CPortalScript :
    public CScript
{
private:
    wstring m_strTargetLevelPath; // 이동할 레벨 경로 (예: L"Level\\Dungeon_Level.lv")
    bool    m_bPlayerContact;    // 플레이어와 접촉 중인지 여부

public:
    virtual void Tick() override;
    virtual void Begin() override;

    void SetTargetLevel(const wstring& _path) { m_strTargetLevelPath = _path; }

    void OnBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void OnOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol);
    void OnEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CPortalScript)
public:
    CPortalScript();
    virtual ~CPortalScript();
};

