#pragma once
#include "CScript.h"

struct tItemInfo;

class CSlotScript;

class CCursorScript :
    public CScript
{
private:
    // 1. 드래그 중인 아이템 데이터 (실제 알맹이)
    Ptr<tItemInfo>  m_pPickedItem;  // 현재 잡고 있는 아이템 데이터
    int             m_iFromIdx;     // 어디서 가져왔는지 저장
    int             m_iFromType;    // 어느 탭에서 가져왔는지 저장
    INVEN_TYPE      m_eFromType;
    MOUSE_STATE     m_ePrevState; // 이전 프레임의 마우스 상태 저장

    Vec2            m_vMousePos;

    // 3. 마우스 옆에 따라다닐 아이콘 전용 오브젝트 (자식 오브젝트)
    // 커서 화살표랑 별개로 아이템 이미지를 띄워야 하므로 필요합니다.
    GameObject*     m_pIconObj;

    // 4. 상태 제어
    bool            m_bIsDragging;    // 현재 아이템을 잡고 있는 중인가?

public:
    virtual void Tick() override;
    virtual void Begin() override;

    CSlotScript* GetHoveredSlot();
    Ptr<tItemInfo> GetPickedItem() { return m_pPickedItem; }

    bool GetDragging() { return m_bIsDragging;  }

    void SetPickedItem(Ptr<tItemInfo> _pItem) {
        m_pPickedItem = _pItem;
    }

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CCursorScript)
public:
    CCursorScript();
    virtual ~CCursorScript();
};

