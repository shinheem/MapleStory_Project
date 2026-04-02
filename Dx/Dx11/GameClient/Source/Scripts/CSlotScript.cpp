#include "pch.h"
#include "CSlotScript.h"

#include "CInventoryScript.h"


CSlotScript::CSlotScript()
	: CScript(SCRIPT_TYPE::SLOTSCRIPT)
    , m_pTargetItem(nullptr)
    , m_iTabType(INVEN_TYPE::EQUIP)
    , m_iSlotIdx(0)
{

}

CSlotScript::~CSlotScript()
{

}

void CSlotScript::Tick()
{

}

void CSlotScript::SetItem(Ptr<tItemInfo> _pItem)
{
    m_pTargetItem = _pItem;

    if (m_pTargetItem)
    {
        // 아이템이 있으면 해당 아이콘으로 텍스처 변경
        // GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam...
        // 또는 아이콘 오브젝트의 이미지를 변경하는 로직
    }
    else
    {
        // 아이템이 없으면 빈칸 처리 (투명화 등)
    }
}
