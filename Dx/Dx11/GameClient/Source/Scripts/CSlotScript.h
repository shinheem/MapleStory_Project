#pragma once
#include "CScript.h"

struct tItemInfo;

class CSlotScript :
    public CScript
{
private:
    Ptr<tItemInfo>      m_pTargetItem;

    MOUSE_STATE         m_ePrevState; // 이전 프레임의 마우스 상태 저장
    INVEN_TYPE          m_iTabType;   // 어떤 탭에 속해 있는가 (0:장비, 1:소비...)
    int                 m_iSlotIdx;   // 해당 탭에서 몇 번째 칸인가 (0~23)

public:
    void SetSlotInfo(Ptr<tItemInfo> _pInfo);
    void SetSlotInfo(INVEN_TYPE _type, int _idx) { m_iTabType = _type; m_iSlotIdx = _idx; }


    void SetItem(Ptr<tItemInfo> _pItem);

    int GetSlotIdx() { return m_iSlotIdx; }
    INVEN_TYPE GetTabType() { return m_iTabType; }

    virtual void Tick() override;

private:
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CSlotScript)
public:
    CSlotScript();
    virtual ~CSlotScript();
};

