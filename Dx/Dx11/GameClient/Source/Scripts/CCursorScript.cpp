#include "pch.h"
#include "CCursorScript.h"
#include "CInventoryScript.h"
#include "CSlotScript.h"

#include "../../KeyMgr.h"
#include "../../LevelMgr.h"
#include "../../ItemMgr.h"

#include "../../GameObject.h"

CCursorScript::CCursorScript()
	: CScript(SCRIPT_TYPE::CURSORSCRIPT)
    , m_pPickedItem()
    , m_iFromIdx()
    , m_eFromType()
    , m_iFromType()
    , m_vMousePos()
    , m_pIconObj()
    , m_bIsDragging()
{

}

CCursorScript::~CCursorScript()
{
    
}

void CCursorScript::Begin()
{
    ShowCursor(FALSE);

    // 자식들 중 이름이 "CursorIcon"인 녀석을 찾아서 저장
    const vector<Ptr<GameObject>>& vecChild = GetOwner()->GetChild();

    for (size_t i = 0; i < vecChild.size(); ++i)
    {
        if (vecChild[i]->GetName() == L"Cursor_Icon")
        {
            m_pIconObj = vecChild[i].Get();
            break;
        }
    }

    // 만약 못 찾았다면 에러 알림 (디버깅용)
    assert(m_pIconObj && "CursorIcon 오브젝트를 찾을 수 없습니다! 자식으로 생성했는지 확인하세요.");
}


void CCursorScript::Tick()
{
    // --- 1. 마우스 좌표 계산 (화면 중앙 기준 정렬) ---
    Vec2 vMousePos = KeyMgr::GetInst()->GetMousePos();
    float fHWidth = 1600.f / 2.f;
    float fHHeight = 900.f / 2.f;

    Vec2 vCenteredPos = Vec2(vMousePos.x - fHWidth, fHHeight - vMousePos.y);
    m_vMousePos = vCenteredPos;

    // 커서 비주얼 위치 보정
    Vec3 vScale = Transform()->GetRelativeScale();
    Vec2 vFinalPos = vCenteredPos;
    vFinalPos.x += vScale.x * 0.4f;
    vFinalPos.y -= vScale.y * 0.5f;

    Transform()->SetRelativePos(Vec3(vFinalPos.x, vFinalPos.y, -10.f));


    // --- 2. 드래그 중인 아이콘(Cursor_Icon) 비주얼 업데이트 ---
    if (m_pPickedItem != nullptr)
    {
        if (m_pIconObj)
        {
            m_pIconObj->SetActive(true);
            if (m_pIconObj->MeshRender() && m_pPickedItem->pIcon != nullptr)
            {
                m_pIconObj->MeshRender()->SetMaterial(m_pPickedItem->pIcon);
            }
        }
    }
    else
    {
        if (m_pIconObj) m_pIconObj->SetActive(false);
    }


    // --- 3. 클릭 처리 (아이템 이동 / 인벤토리 내 보호 / 바닥 드롭) ---
    if (KEY_TAP(KEY::LBTN))
    {
        GameObject* pInvObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Charactor_Inventory").Get();
        CSlotScript* pHoveredSlot = GetHoveredSlot();

        // [상황 A] 아이템을 들고 있는 상태
        if (m_pPickedItem != nullptr)
        {
            // 1. 정확히 슬롯 위를 클릭했다면 -> 아이템 이동/교체
            if (pHoveredSlot != nullptr)
            {
                CInventoryScript* pInvScript = pInvObj->GetScript<CInventoryScript>().Get();
                pInvScript->HandleSlotClick(pHoveredSlot);
            }
            // 2. 슬롯 위가 아닐 때 (빈 공간 혹은 인벤토리 밖)
            else
            {
                bool bInInventoryArea = false;

                // 인벤토리 배경 영역 안에 있는지 체크
                if (nullptr != pInvObj && pInvObj->IsActive())
                {
                    Vec3 vInvPos = pInvObj->Transform()->GetWorldPos();
                    Vec3 vInvScale = pInvObj->Transform()->GetRelativeScale();

                    // AABB 충돌 판정 (인벤토리 전체 몸통 기준)
                    if (m_vMousePos.x >= vInvPos.x - vInvScale.x * 0.5f && m_vMousePos.x <= vInvPos.x + vInvScale.x * 0.5f &&
                        m_vMousePos.y >= vInvPos.y - vInvScale.y * 0.5f && m_vMousePos.y <= vInvPos.y + vInvScale.y * 0.5f)
                    {
                        bInInventoryArea = true;
                    }
                }

                // [결과] 인벤토리 영역 '밖'일 때만 캐릭터 위치에 버리기 실행
                if (!bInInventoryArea)
                {
                    GameObject* pPlayer = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Maple_Player").Get();

                    Vec3 vDropPos;
                    if (nullptr != pPlayer)
                    {
                        vDropPos = pPlayer->Transform()->GetRelativePos();
                        vDropPos.z = 50.f;
                    }
                    else
                    {
                        vDropPos = Vec3(m_vMousePos.x, m_vMousePos.y, 50.f);
                    }

                    // ItemMgr를 통해 아이템 생성
                    ItemMgr::GetInst()->CreateItemDrop(m_pPickedItem, vDropPos);

                    // 커서 데이터 초기화 (버리기 완료)
                    m_pPickedItem = nullptr;
                    m_bIsDragging = false;
                    if (m_pIconObj) m_pIconObj->SetActive(false);
                }
                else
                {
                    // 여기는 인벤토리 배경/슬롯 사이 빈 공간을 클릭한 경우입니다.
                    // 아무것도 하지 않음으로써 아이템을 계속 들고 있게 유지합니다.
                }
            }
        }
        // [상황 B] 아이템을 들고 있지 않은 상태
        else
        {
            if (nullptr != pInvObj && pInvObj->IsActive() && nullptr != pHoveredSlot)
            {
                CInventoryScript* pInvScript = pInvObj->GetScript<CInventoryScript>().Get();
                pInvScript->HandleSlotClick(pHoveredSlot);
            }
        }
    }
    // --- 4. 상태 결정 ---
    MOUSE_STATE eCurState = MOUSE_STATE::NONE;

    if (m_pPickedItem != nullptr)
    {
        eCurState = MOUSE_STATE::HOLD; // 3번: 아이템 들고 있을 때
    }
    else
    {
        CSlotScript* pHoveredSlot = GetHoveredSlot();
        bool bHoverTabButton = false;

        GameObject* pInvObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Charactor_Inventory").Get();
        if (pInvObj && pInvObj->IsActive())
        {
            const vector<Ptr<GameObject>>& vecChildren = pInvObj->GetChild();
            Vec3 vInvPos = pInvObj->Transform()->GetWorldPos();
            Vec2 vLocalMouse = Vec2(m_vMousePos.x - vInvPos.x, m_vMousePos.y - vInvPos.y);

            for (size_t i = 0; i < vecChildren.size(); ++i)
            {
                if (vecChildren[i]->GetName().find(L"_Button") != wstring::npos)
                {
                    Vec3 vPos = vecChildren[i]->Transform()->GetRelativePos();
                    Vec3 vScale = vecChildren[i]->Transform()->GetRelativeScale();

                    if (vLocalMouse.x >= vPos.x - vScale.x * 0.5f && vLocalMouse.x <= vPos.x + vScale.x * 0.5f &&
                        vLocalMouse.y >= vPos.y - vScale.y * 0.5f && vLocalMouse.y <= vPos.y + vScale.y * 0.5f)
                    {
                        bHoverTabButton = true;
                        break;
                    }
                }
            }
        }

        // --- 상태 세부 결정 ---
        if (bHoverTabButton)
        {
            eCurState = MOUSE_STATE::TAB_HOVER;
        }
        else if (pHoveredSlot != nullptr)
        {
            // 슬롯 위에 있을 때는 클릭 여부와 상관없이 무조건 SLOT_HOVER(2번) 유지
            // 만약 클릭 시 특별한 연출이 필요 없다면 아래처럼 한 줄로 처리하면 됩니다.
            eCurState = MOUSE_STATE::SLOT_HOVER;
        }
    }

    // --- 5. 애니메이션 재생 (번호 매핑은 동일) ---
    if (m_ePrevState != eCurState)
    {
        if (FlipbookRender())
        {
            switch (eCurState)
            {
            case MOUSE_STATE::NONE:
                FlipbookRender()->Play(0, 6.f, -1);
                break;
            case MOUSE_STATE::TAB_HOVER:
                FlipbookRender()->Play(1, 6.f, -1); // 탭 호버 (1번)
                break;
            case MOUSE_STATE::SLOT_HOVER:
                FlipbookRender()->Play(2, 6.f, -1); // 슬롯 호버/클릭 (2번) ★
                break;
            case MOUSE_STATE::HOLD:
                FlipbookRender()->Play(3, 6.f, -1); // 아이템 잡기 (3번)
                break;
            }
        }
        m_ePrevState = eCurState;
    }
}

CSlotScript* CCursorScript::GetHoveredSlot()
{
    // 1. 인벤토리 오브젝트 찾기
    GameObject* pInvObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Charactor_Inventory").Get();
    if (nullptr == pInvObj || !pInvObj->IsActive()) return nullptr;

    CInventoryScript* pInvScript = pInvObj->GetScript<CInventoryScript>().Get();

    // 2. 현재 탭의 슬롯 리스트 가져오기
    const vector<Ptr<GameObject>>& vecSlots = pInvScript->GetCurrentTabSlots();

    // 3. 모든 슬롯을 돌며 마우스(m_vMousePos)와 충돌하는지 체크
    for (size_t i = 0; i < vecSlots.size(); ++i)
    {
        // 슬롯의 월드 좌표와 크기 (인벤토리 UI의 스케일에 영향을 받으므로 WorldPos 사용)
        Vec3 vWorldPos = vecSlots[i]->Transform()->GetWorldPos();
        Vec3 vScale = vecSlots[i]->Transform()->GetRelativeScale();

        // AABB 충돌 판정
        if (m_vMousePos.x >= vWorldPos.x - vScale.x * 0.5f && m_vMousePos.x <= vWorldPos.x + vScale.x * 0.5f &&
            m_vMousePos.y >= vWorldPos.y - vScale.y * 0.5f && m_vMousePos.y <= vWorldPos.y + vScale.y * 0.5f)
        {
            return vecSlots[i]->GetScript<CSlotScript>().Get();
        }
    }

    return nullptr;
}