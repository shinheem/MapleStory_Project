#pragma once
#include "CScript.h"
#include "../../GameObject.h"
#include "../../ATexture.h"

struct tItemInfo 
    :Entity 
{
    wstring  strName;      // 이름
    Ptr<ATexture> pIcon;   // UI에 보여줄 아이콘
    ITEM_TYPE eType;       // 소모품, 장비 등
    int      iMaxStack;    // 최대 겹치기 개수
    int      iCount;       // 현재 개수
};

class CInventoryScript :
    public CScript
{
private:
    void ChangeTab(int _iNextTab);

    const wstring m_tabNames[(UINT)INVEN_TYPE::END] = { L"Equip", L"Consume", L"Etc", L"Install", L"Cash" };

    Ptr<GameObject>         m_pInventoryUI;
    vector<Ptr<tItemInfo>>  m_vecInventory[(UINT)ITEM_TYPE::END];
    vector<Ptr<GameObject>> m_vecTabContents; // 각 탭의 슬롯들을 담은 부모 객체들

    int                 m_iCurrentTab; // 현재 선택된 탭 인덱스 (기본값 0: Equip)

public:
    virtual void Tick() override;
    virtual void Begin() override;

    void SetInventoryUI(Ptr<GameObject> _UI) { m_pInventoryUI = _UI; }
    void AddItem(Ptr<tItemInfo> _pItem);
    void UpdateTabUI(int _iTabIdx);
   /* void CreateSlotsForTab(GameObject* pParent, int iTabType);

    void CreateInventorySlots();*/

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CInventoryScript)
public:
    CInventoryScript();
    virtual ~CInventoryScript();

};

