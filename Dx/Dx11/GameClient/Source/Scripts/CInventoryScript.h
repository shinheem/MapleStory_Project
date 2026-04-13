#pragma once
#include "CScript.h"
#include "../../GameObject.h"
#include "../../AMaterial.h"

#define INVEN_SLOT_COUNT 24

struct tItemInfo 
    :Entity 
{
    wstring  strName;       // 이름
    Ptr<AMaterial> pIcon;   // UI에 보여줄 아이콘
    ITEM_TYPE eType;        // 소모품, 장비 등
    int      iMaxStack;     // 최대 겹치기 개수
    int      iCount;        // 현재 개수
};

class CSlotScript;

class CInventoryScript :
    public CScript
{
private:
    const wstring           m_tabNames[(UINT)INVEN_TYPE::END] = { L"Equip", L"Consume", L"Etc", L"Install", L"Cash" };

    GameObject*             m_pDragUIObj;    // 마우스를 따라다닐 임시 아이콘 오브젝트

    Ptr<tItemInfo>          m_pDragItem;     // 현재 드래그 중인 아이템 데이터
    Ptr<GameObject>         m_pInventoryUI;
    vector<Ptr<tItemInfo>>  m_vecInventory[(UINT)INVEN_TYPE::END];
    vector<GameObject*>     m_vecTabContents; // 각 탭의 슬롯들을 담은 부모 객체들
    vector<CSlotScript*>    m_vecSlots;

    INVEN_TYPE              m_eCurTab;

    int                     m_iDragFromIdx;
    bool                    m_bIsDragging;
    bool                    m_bInitialized;
    int                     m_iCurrentTab; // 현재 선택된 탭 인덱스 (기본값 0: Equip)

    void ChangeTab(int _iNextTab);
public:
    virtual void Tick() override;
    virtual void Begin() override;

    void SetCurTab(INVEN_TYPE _eType) { m_eCurTab = _eType; }
    void SetInventoryUI(Ptr<GameObject> _UI) { m_pInventoryUI = _UI; }
    bool AddItem(Ptr<tItemInfo> _pItem);
    void UpdateTabUI(int _iTabIdx);
    void SwapSlot(int _iType, int _iFromIdx, int _iToIdx);
    void HandleSlotClick(CSlotScript* _pSlot);

public:
    const vector<Ptr<GameObject>>& GetCurrentTabSlots() {
        // m_vecTabContents[m_iCurrentTab]는 현재 활성화된 Content 오브젝트입니다.
        // 그 자식들이 실제 슬롯들입니다.
        return m_vecTabContents[m_iCurrentTab]->GetChild();
    }


private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CInventoryScript)
public:
    CInventoryScript();
    virtual ~CInventoryScript();

};

