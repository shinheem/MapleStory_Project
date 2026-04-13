#include "pch.h"
#include "CInventoryScript.h"
#include "CCursorScript.h"

#include "../../LevelMgr.h"
#include "../../KeyMgr.h"
#include "../../AssetMgr.h"

#include "CSlotScript.h"

CInventoryScript::CInventoryScript()
	: CScript(SCRIPT_TYPE::INVENTORYSCRIPT)
    , m_pInventoryUI(nullptr)
    , m_vecInventory{}
    , m_vecTabContents{}
    , m_iCurrentTab(0)
    , m_iDragFromIdx()
    , m_bIsDragging()

{
}

void CInventoryScript::Begin()
{
    // 1. 중복 실행 방지 (자식 객체이거나 이미 데이터가 있다면 리턴)
    //if (GetOwner()->GetParent() != nullptr) return;
    //if (!m_vecTabContents.empty()) return;

    if (m_bInitialized)
        return;

    // 2. 인벤토리 데이터 배열 초기화 (24칸씩)
    for (int i = 0; i < (int)INVEN_TYPE::END; ++i)
    {
        m_vecInventory[i].clear();
        m_vecInventory[i].resize(INVEN_SLOT_COUNT, nullptr);
    }

    m_vecTabContents.clear();
    const vector<Ptr<GameObject>>& vecChildren = GetOwner()->GetChild();

    // 3. 각 탭의 부모 오브젝트(Content) 찾기 및 슬롯 인덱스 설정
    for (int i = 0; i < (int)INVEN_TYPE::END; ++i)
    {
        wstring strTargetName = m_tabNames[i] + L"_Content";
        bool bFound = false;

        for (size_t j = 0; j < vecChildren.size(); ++j)
        {
            if (vecChildren[j]->GetName() == strTargetName)
            {
                // 컨텐츠 패널 저장
                m_vecTabContents.push_back(vecChildren[j].Get());

                // 현재 선택된 탭만 활성화, 나머지는 비활성화
                if (i == m_iCurrentTab) vecChildren[j]->SetActive(true);
                else vecChildren[j]->SetActive(false);

                // ★ 핵심: 해당 패널 자식(슬롯)들에게 번호 부여 ★
                const vector<Ptr<GameObject>>& vecSlots = vecChildren[j]->GetChild();
                for (size_t k = 0; k < vecSlots.size(); ++k)
                {
                    CSlotScript* pSlot = vecSlots[k]->GetScript<CSlotScript>().Get();
                    if (pSlot)
                    {
                        // k가 0~23번까지의 고유 번호가 됩니다.
                        // i는 EQUIP, CONSUME 등의 탭 타입입니다.
                        pSlot->SetSlotInfo((INVEN_TYPE)i, (int)k);
                    }
                }

                bFound = true;
                break;
            }
        }
        assert(bFound && "탭 컨텐츠 오브젝트(L'Equip_Content' 등)를 찾을 수 없습니다!");
    }

    // 4. 탭 버튼 애니메이션/상태 초기화
    int iTabCount = 0;
    for (size_t i = 0; i < vecChildren.size(); ++i)
    {
        if (vecChildren[i]->GetName().find(L"_Button") != wstring::npos)
        {
            if (vecChildren[i]->FlipbookRender() != nullptr)
            {
                // 현재 선택된 탭 버튼은 '눌림' 상태(1번 프레임)
                if (iTabCount == m_iCurrentTab)
                    vecChildren[i]->FlipbookRender()->Play(1, 1.f, -1);
                else
                    vecChildren[i]->FlipbookRender()->Play(0, 1.f, -1);
            }
            iTabCount++;
        }
    }

    // 5. 초기 UI 데이터 반영
    UpdateTabUI(m_iCurrentTab);

    // 6. 시작 시 인벤토리는 닫혀 있도록 설정 (필요 시)
    GetOwner()->SetActive(false);
    m_bInitialized = true;
}

CInventoryScript::~CInventoryScript()
{
    m_vecTabContents.clear();
    for (int i = 0; i < (int)INVEN_TYPE::END; ++i)
    {
        m_vecInventory[i].clear();
    }
}

void CInventoryScript::Tick()
{
    //if (GetOwner()->GetParent() != nullptr) return;

    // --- 1. 키보드 입력 처리 ---
    if (KEY_TAP(KEY::I))
    {
        bool bActive = !GetOwner()->IsActive();
        GetOwner()->SetActive(bActive);
        if (bActive) UpdateTabUI(m_iCurrentTab);
    }
    else if (KEY_TAP(KEY::ESC))
    {
        GetOwner()->SetActive(false);
    }

    if (!GetOwner()->IsActive()) return;

    // 공통 좌표 계산 (클릭과 호버 모두 사용)
    Vec2 vMousePos = KeyMgr::GetInst()->GetMousePos();
    float fHWidth = 1600.f / 2.f;
    float fHHeight = 900.f / 2.f;
    Vec2 vCenteredMouse = Vec2(vMousePos.x - fHWidth, fHHeight - vMousePos.y);
    Vec3 vInvPos = Transform()->GetWorldPos();
    Vec2 vLocalMouse = Vec2(vCenteredMouse.x - vInvPos.x, vCenteredMouse.y - vInvPos.y);

    const vector<Ptr<GameObject>>& vecChildren = GetOwner()->GetChild();

    // --- [추가] Inventory_XBtn 호버 애니메이션 처리 ---
    for (size_t i = 0; i < vecChildren.size(); ++i)
    {
        if (vecChildren[i]->GetName() == L"Incentory_XBtn")
        {
            if (vecChildren[i]->FlipbookRender())
            {
                Vec3 vPos = vecChildren[i]->Transform()->GetRelativePos();
                Vec3 vScale = vecChildren[i]->Transform()->GetRelativeScale();

                // 마우스가 X버튼 영역 안에 있는지 체크
                if (vLocalMouse.x >= vPos.x - vScale.x * 0.5f && vLocalMouse.x <= vPos.x + vScale.x * 0.5f &&
                    vLocalMouse.y >= vPos.y - vScale.y * 0.5f && vLocalMouse.y <= vPos.y + vScale.y * 0.5f)
                {
                    // 마우스 올라감 -> 1번 애니메이션
                    if (vecChildren[i]->FlipbookRender()->GetCurFlipbookIdx() != 1)
                        vecChildren[i]->FlipbookRender()->Play(1, 1.f, -1);
                }
                else
                {
                    // 마우스 나감 -> 0번 애니메이션
                    if (vecChildren[i]->FlipbookRender()->GetCurFlipbookIdx() != 0)
                        vecChildren[i]->FlipbookRender()->Play(0, 1.f, -1);
                }
            }
            break; // X버튼을 찾았으므로 루프 종료
        }
    }


    // --- 2. 마우스 클릭 처리 ---
    if (KEY_TAP(KEY::LBTN))
    {
        int iTabIdx = 0;

        for (size_t i = 0; i < vecChildren.size(); ++i)
        {
            // [수정] Inventory_XBtn 클릭 판정
            if (vecChildren[i]->GetName() == L"Incentory_XBtn")
            {
                Vec3 vPos = vecChildren[i]->Transform()->GetRelativePos();
                Vec3 vScale = vecChildren[i]->Transform()->GetRelativeScale();

                if (vLocalMouse.x >= vPos.x - vScale.x * 0.5f && vLocalMouse.x <= vPos.x + vScale.x * 0.5f &&
                    vLocalMouse.y >= vPos.y - vScale.y * 0.5f && vLocalMouse.y <= vPos.y + vScale.y * 0.5f)
                {
                    GetOwner()->SetActive(false); // 인벤토리 닫기
                    return;
                }
                continue;
            }

            // [기존] 탭 버튼 클릭 판정
            if (vecChildren[i]->GetName().find(L"_Button") != wstring::npos)
            {
                Vec3 vPos = vecChildren[i]->Transform()->GetRelativePos();
                Vec3 vScale = vecChildren[i]->Transform()->GetRelativeScale();

                if (vLocalMouse.x >= vPos.x - vScale.x * 0.5f && vLocalMouse.x <= vPos.x + vScale.x * 0.5f &&
                    vLocalMouse.y >= vPos.y - vScale.y * 0.5f && vLocalMouse.y <= vPos.y + vScale.y * 0.5f)
                {
                    ChangeTab(iTabIdx);
                    break;
                }
                iTabIdx++;
            }
        }
    }
}

bool CInventoryScript::AddItem(Ptr<tItemInfo> _pInfo)
{
    if (nullptr == _pInfo) return false;

    UINT iType = (UINT)_pInfo->eType;
    auto& vecTargetTab = m_vecInventory[iType];

    for (size_t i = 0; i < vecTargetTab.size(); ++i)
    {
        if (vecTargetTab[i] == nullptr)
        {
            vecTargetTab[i] = _pInfo;

            // 현재 보고 있는 탭과 추가된 아이템 탭이 같다면 화면 즉시 갱신
            if (m_iCurrentTab == (int)iType)
            {
                UpdateTabUI(m_iCurrentTab);
            }

            return true;
        }
    }

    return false;
}

void CInventoryScript::UpdateTabUI(int _iTabIdx)
{
    if (_iTabIdx >= (int)m_vecTabContents.size()) return;

    // 1. 해당 탭의 슬롯 오브젝트들 가져오기
    const vector<Ptr<GameObject>>& vecSlots = m_vecTabContents[_iTabIdx]->GetChild();

    // 2. 해당 탭의 데이터 가져오기
    const vector<Ptr<tItemInfo>>& vecItems = m_vecInventory[_iTabIdx];

    for (size_t i = 0; i < vecSlots.size(); ++i)
    {
        CSlotScript* pSlotScript = vecSlots[i]->GetScript<CSlotScript>().Get();
        if (nullptr == pSlotScript) continue;

        if (i < vecItems.size())
        {
            // 데이터 전달 (SetItem 내부에서 이제 재질을 바꿉니다)
            pSlotScript->SetItem(vecItems[i]);
        }
        else
        {
            pSlotScript->SetItem(nullptr);
        }
    }
}

void CInventoryScript::ChangeTab(int _iNextTab)
{
    if (_iNextTab < 0 || _iNextTab >= (int)INVEN_TYPE::END) return;
    if (m_iCurrentTab == _iNextTab) return;

    if (m_vecTabContents[m_iCurrentTab])
        m_vecTabContents[m_iCurrentTab]->SetActive(false);

    const vector<Ptr<GameObject>>& vecChildren = GetOwner()->GetChild();
    int iTabCount = 0;
    for (auto& child : vecChildren)
    {
        if (child->GetName().find(L"_Button") != wstring::npos)
        {
            if (iTabCount == m_iCurrentTab)
            {
                if (child->FlipbookRender()) child->FlipbookRender()->Play(0, 1.f, -1);
            }
            if (iTabCount == _iNextTab)
            {
                if (child->FlipbookRender()) child->FlipbookRender()->Play(1, 1.f, -1);
            }
            iTabCount++;
        }
    }

    m_iCurrentTab = _iNextTab;

    if (m_vecTabContents[m_iCurrentTab])
    {
        m_vecTabContents[m_iCurrentTab]->SetActive(true);
        UpdateTabUI(m_iCurrentTab);
    }
}

// CInventoryScript.cpp
void CInventoryScript::SwapSlot(int _iType, int _iFromIdx, int _iToIdx)
{
    // 1. 같은 위치면 무시
    if (_iFromIdx == _iToIdx) return;

    // 2. 실제 데이터 교체
    Ptr<tItemInfo> pTemp = m_vecInventory[_iType][_iFromIdx];
    m_vecInventory[_iType][_iFromIdx] = m_vecInventory[_iType][_iToIdx];
    m_vecInventory[_iType][_iToIdx] = pTemp;

    // 3. UI 갱신 (이미 만들어두신 함수 호출)
    UpdateTabUI(_iType);
}

void CInventoryScript::HandleSlotClick(CSlotScript* _pSlot)
{
    if (nullptr == _pSlot) return;

    // 1. 커서 객체와 스크립트 가져오기
    GameObject* pMouse = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Mouse_Cursor").Get();
    if (nullptr == pMouse) return;

    CCursorScript* pCursor = pMouse->GetScript<CCursorScript>().Get();
    if (nullptr == pCursor) return;

    // 2. 커서의 비주얼 아이콘 오브젝트 찾기 (자식 객체)
    GameObject* pCursorIconObj = nullptr;
    const vector<Ptr<GameObject>>& vecCursorChildren = pMouse->GetChild();
    for (size_t i = 0; i < vecCursorChildren.size(); ++i)
    {
        if (vecCursorChildren[i]->GetName() == L"Cursor_Icon")
        {
            pCursorIconObj = vecCursorChildren[i].Get();
            break;
        }
    }

    // 3. 클릭된 슬롯의 정보 추출
    int iTargetIdx = _pSlot->GetSlotIdx();   // 슬롯 번호 (0~23)
    int iTabType = (int)_pSlot->GetTabType(); // 탭 종류 (EQUIP, CONSUME 등)

    // 안전 장치: 인덱스 범위 확인
    if (iTargetIdx < 0 || iTargetIdx >= INVEN_SLOT_COUNT) return;

    // [상황 A] 커서가 비어 있음 -> 아이템 집기 (Pick Up)
    if (pCursor->GetPickedItem() == nullptr)
    {
        Ptr<tItemInfo> pItem = m_vecInventory[iTabType][iTargetIdx];

        // 슬롯에 아이템이 있을 때만 집음
        if (pItem != nullptr)
        {
            // 커서 데이터 설정
            pCursor->SetPickedItem(pItem);

            // 인벤토리 데이터 비우기
            m_vecInventory[iTabType][iTargetIdx] = nullptr;

            // 드래그 상태값 설정
            m_iDragFromIdx = iTargetIdx;
            m_bIsDragging = true;

            // 커서 아이콘 활성화 및 재질 적용
            if (pCursorIconObj)
            {
                pCursorIconObj->SetActive(true);
                if (pCursorIconObj->MeshRender())
                    pCursorIconObj->MeshRender()->SetMaterial(pItem->pIcon);

                // 커서보다 살짝 앞에 위치 (Z값 조정)
                pCursorIconObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
            }

            // 슬롯 UI 갱신 (아이템이 사라진 모습으로)
            UpdateTabUI(iTabType);
        }
    }
    // [상황 B] 커서가 아이템을 들고 있음 -> 내려놓기 또는 교체 (Drop / Swap)
    else
    {
        Ptr<tItemInfo> pPickedItem = pCursor->GetPickedItem(); // 손에 든 아이템
        Ptr<tItemInfo> pExistItem = m_vecInventory[iTabType][iTargetIdx]; // 슬롯에 원래 있던 아이템(없으면 nullptr)

        // 중요: 아이템 타입과 현재 탭이 일치하는지 확인 (예: 장비는 장비탭에만)
        if (pPickedItem->eType != (ITEM_TYPE)iTabType)
        {
            // 타입이 안 맞으면 그냥 무시하거나 안내 메시지
            return;
        }

        // 데이터 교체 로직 (핵심)
        m_vecInventory[iTabType][iTargetIdx] = pPickedItem; // 슬롯에 손에 든 아이템 배치
        pCursor->SetPickedItem(pExistItem);              // 슬롯에 있던 아이템을 손으로 (없었다면 nullptr이 됨)

        // 커서 비주얼 업데이트
        if (pCursorIconObj)
        {
            if (pExistItem != nullptr)
            {
                // 교체(Swap)된 경우: 새로운 아이템 아이콘 표시
                pCursorIconObj->SetActive(true);
                if (pCursorIconObj->MeshRender())
                    pCursorIconObj->MeshRender()->SetMaterial(pExistItem->pIcon);
            }
            else
            {
                // 빈 슬롯에 놓은 경우: 커서 아이콘 비활성화 및 드래그 종료
                pCursorIconObj->SetActive(false);
                m_bIsDragging = false;
            }
        }

        // 전체 UI 갱신
        UpdateTabUI(iTabType);
    }
}

// 인벤토리 탭, 슬롯 생성 함수
//void CInventoryScript::CreateInventorySlots()
//{
//    m_vecTabContents.clear();
//
//    // --- [1. 탭 버튼 배치 (상단 주황색 줄 영역)] ---
//    float fTabWidth = 36.58f;
//    float fTabHeight = 17.f;
//    float fTabPadding = 0.f;
//    Vec3 vTabStartPos = Vec3(-73.f,154.f, -1.f); // 첫 번째 탭 위치
//
//    for (int t = 0; t < (int)INVEN_TYPE::END; ++t)
//    {
//        // 탭 버튼 생성 (이 버튼들이 '조그마한' 탭입니다)
//        GameObject* pTabButton = new GameObject;
//        pTabButton->SetName(m_tabNames[t] + L"_Button");
//        pTabButton->AddComponent(new CTransform);
//
//        float posX = vTabStartPos.x + (t * (fTabWidth + fTabPadding));
//        pTabButton->Transform()->SetRelativePos(Vec3(posX, vTabStartPos.y, -1.f));
//        pTabButton->Transform()->SetRelativeScale(Vec3(fTabWidth, fTabHeight, 1.f));
//        pTabButton->Transform()->SetIndependentScale(true);
//
//        GetOwner()->AddChild(pTabButton);
//
//        // --- [2. 탭 컨테이너 생성 (하단 슬롯 영역 부모)] ---
//        GameObject* pTabContent = new GameObject;
//        pTabContent->SetName(m_tabNames[t] + L"_Content");
//        pTabContent->AddComponent(new CTransform);
//        GetOwner()->AddChild(pTabContent);
//
//        m_vecTabContents.push_back(pTabContent);
//        if (t != (int)INVEN_TYPE::EQUIP) pTabContent->SetActive(false);
//
//        // 슬롯 생성 호출
//        CreateSlotsForTab(pTabContent, t);
//    }
//}
//
//void CInventoryScript::CreateSlotsForTab(GameObject* pParent, int iTabType)
//{
//    int iCol = 4;
//    float fSlotSize = 34.f;      // 격자 크기에 맞춤
//    float fPaddingX = 8.f;       // 가로 간격
//    float fPaddingY = 8.f;       // 세로 간격
//    Vec3 vSlotStartPos = Vec3(-66.f, 123.f, -1.f); // 첫 번째 슬롯 중심
//
//    for (int i = 0; i < 24; ++i)
//    {
//        GameObject* pSlotObj = new GameObject;
//        pSlotObj->SetName(m_tabNames[iTabType] + L"_Slot_" + std::to_wstring(i));
//        pSlotObj->AddComponent(new CTransform);
//
//        int curRow = i / iCol;
//        int curCol = i % iCol;
//
//        float posX = vSlotStartPos.x + (curCol * (fSlotSize + fPaddingX));
//        float posY = vSlotStartPos.y - (curRow * (fSlotSize + fPaddingY));
//
//        pSlotObj->Transform()->SetRelativePos(Vec3(posX, posY, -1.f));
//        pSlotObj->Transform()->SetRelativeScale(Vec3(fSlotSize, fSlotSize, 1.f));
//        pSlotObj->Transform()->SetIndependentScale(true);
//
//        // 스크립트 추가 및 부모 설정
//        CSlotScript* pSlotScript = new CSlotScript;
//        pSlotScript->SetSlotInfo((INVEN_TYPE)iTabType, i);
//        pSlotObj->AddComponent(pSlotScript);
//
//        pParent->AddChild(pSlotObj);
//    }
//}
