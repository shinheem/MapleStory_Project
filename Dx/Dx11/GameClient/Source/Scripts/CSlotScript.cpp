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

void CSlotScript::SetSlotInfo(Ptr<tItemInfo> _pInfo)
{
    // 아이템이 없으면 메쉬렌더를 밀어버림
    if (nullptr == _pInfo)
    {
        if (GetOwner()->MeshRender())
            GetOwner()->MeshRender()->SetMaterial(nullptr);
        return;
    }

    // 아이템이 있으면 해당 아이콘 재질(Material)을 셋팅
    if (GetOwner()->MeshRender())
    {
        GetOwner()->MeshRender()->SetMaterial(_pInfo->pIcon);
    }
}

void CSlotScript::SetItem(Ptr<tItemInfo> _pItem)
{
    m_pTargetItem = _pItem;

    // 1. 슬롯 본체의 메쉬렌더러에 아이콘 재질 적용
    // (tItemInfo 구조체에서 pIcon이 Material이므로 직접 셋팅합니다)
    if (nullptr != m_pTargetItem)
    {
        if (GetOwner()->MeshRender())
        {
            GetOwner()->MeshRender()->SetMaterial(m_pTargetItem->pIcon);
        }
    }
    else
    {
        if (GetOwner()->MeshRender())
        {
            GetOwner()->MeshRender()->SetMaterial(nullptr);
        }
    }

    // 2. 만약 별도의 자식 오브젝트(ItemIcon)를 사용하여 Active/Inactive를 관리한다면 아래 로직 유지
    GameObject* pIconObj = nullptr;
    const vector<Ptr<GameObject>>& vecChild = GetOwner()->GetChild();

    for (size_t i = 0; i < vecChild.size(); ++i)
    {
        if (vecChild[i]->GetName() == L"ItemIcon")
        {
            pIconObj = vecChild[i].Get();
            break;
        }
    }

    if (nullptr != pIconObj)
    {
        if (nullptr != m_pTargetItem)
        {
            pIconObj->SetActive(true);
            // 자식 오브젝트가 실제 아이콘을 그리는 주체라면 여기에도 재질을 넣어줘야 합니다.
            if (pIconObj->MeshRender())
                pIconObj->MeshRender()->SetMaterial(m_pTargetItem->pIcon);
        }
        else
        {
            pIconObj->SetActive(false);
        }
    }
}