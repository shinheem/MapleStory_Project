#include "pch.h"
#include "CSlotScript.h"
#include "CInventoryScript.h"

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
    // 1 & 2. 좌표 계산 로직 (기존과 동일)
    Vec2 vMousePos = KeyMgr::GetInst()->GetMousePos();
    float fHWidth = 1600.f / 2.f;
    float fHHeight = 900.f / 2.f;
    Vec2 vMousePoint = Vec2(vMousePos.x - fHWidth, fHHeight - vMousePos.y);

    Vec3 vWorldPos = Transform()->GetWorldPos();
    Vec3 vScale = Transform()->GetRelativeScale();

    // 3. 충돌 판정
    bool bHover = (vMousePoint.x >= vWorldPos.x - vScale.x * 0.5f && vMousePoint.x <= vWorldPos.x + vScale.x * 0.5f &&
        vMousePoint.y >= vWorldPos.y - vScale.y * 0.5f && vMousePoint.y <= vWorldPos.y + vScale.y * 0.5f);

    // 4. 상태 결정
    MOUSE_STATE eCurState = MOUSE_STATE::NONE;
    if (bHover)
    {
        if (KEY_PRESSED(KEY::LBTN)) eCurState = MOUSE_STATE::CLICK;
        else eCurState = MOUSE_STATE::HOVER;
    }

    // 5. 상태가 변경되었을 때만 애니메이션 재생
    if (m_ePrevState != eCurState)
    {
        GameObject* pMouse = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Mouse_Cursor").Get();
        if (pMouse && pMouse->FlipbookRender())
        {
            switch (eCurState)
            {
            case MOUSE_STATE::NONE:
                pMouse->FlipbookRender()->Play(0, 6.f, -1); // 기본 커서
                break;
            case MOUSE_STATE::HOVER:
                pMouse->FlipbookRender()->Play(2, 6.f, -1); // 호버 애니메이션
                break;
            case MOUSE_STATE::CLICK:
                pMouse->FlipbookRender()->Play(1, 6.f, -1); // 클릭 애니메이션
                break;
            }
        }
        m_ePrevState = eCurState; // 현재 상태 저장
    }
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