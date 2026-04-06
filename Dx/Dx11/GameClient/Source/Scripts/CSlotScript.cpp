#include "pch.h"
#include "CSlotScript.h"
#include "CInventoryScript.h"
#include "CCursorScript.h"

#include "../../LevelMgr.h"
#include "../../KeyMgr.h"

#include "../../GameObject.h"

CSlotScript::CSlotScript()
	: CScript(SCRIPT_TYPE::SLOTSCRIPT)
    , m_pTargetItem(nullptr)
    , m_iTabType(INVEN_TYPE::EQUIP)
    , m_iSlotIdx(0)
{

}

CSlotScript::~CSlotScript()
{
    m_pTargetItem = nullptr;
}

void CSlotScript::Tick()
{
}

void CSlotScript::SetItem(Ptr<tItemInfo> _pItem)
{
    // 1. 데이터 업데이트
    m_pTargetItem = _pItem;

    // 2. 슬롯 본체의 MeshRender를 가져옴
    Ptr<CMeshRender> pMeshRender = GetOwner()->MeshRender();

    if (nullptr != pMeshRender)
    {
        if (nullptr != m_pTargetItem)
        {
            // 아이템이 있으면: 아이템의 아이콘 재질(Material)을 슬롯에 직접 셋팅
            pMeshRender->SetMaterial(m_pTargetItem->pIcon);
        }
        else
        {
            // 아이템이 없으면: 슬롯을 비움 (기본 빈 칸 재질이 있다면 그걸로 교체 가능)
            // 일단 nullptr로 밀어버리면 아무것도 안 그려집니다.
            pMeshRender->SetMaterial(nullptr);
        }
    }
}

// 기존에 사용하던 SetSlotInfo도 내부적으로 SetItem을 호출하게 통일
void CSlotScript::SetSlotInfo(Ptr<tItemInfo> _pInfo)
{
    SetItem(_pInfo);
}