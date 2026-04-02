#include "pch.h"
#include "CInventoryScript.h"

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

{
}

void CInventoryScript::Begin()
{
    // 인벤토리 탭, 슬롯 생성
    /*if (GetOwner()->GetChilds().size() <= 1)
    {
        CreateInventorySlots();
    }*/
    if (GetOwner()->GetParent() != nullptr)
        return;

    // 1. 기존에 담긴 내용이 있다면 비워줍니다.
    m_vecTabContents.clear();

    // 2. 현재 내 자식 오브젝트들을 순회하며 컨텐츠 패널을 찾습니다.
    const vector<Ptr<GameObject>>& vecChildren = GetOwner()->GetChild();

    // 탭 이름 정의 (헤더에 선언된 m_tabNames 활용)
    // L"Equip", L"Consume", L"Etc", L"Install", L"Cash" 순서대로 찾아서 넣어야 합니다.
    for (int i = 0; i < (int)INVEN_TYPE::END; ++i)
    {
        wstring strTargetName = m_tabNames[i] + L"_Content";
        bool bFound = false;

        for (size_t j = 0; j < vecChildren.size(); ++j)
        {
            if (vecChildren[j]->GetName() == strTargetName)
            {
                m_vecTabContents.push_back(vecChildren[j]);

                // 처음 시작할 때 0번(Equip)만 켜고 나머지는 끕니다.
                if (i == m_iCurrentTab) vecChildren[j]->SetActive(true);
                else vecChildren[j]->SetActive(false);

                bFound = true;
                break;
            }
        }

        // 만약 레벨에서 이름을 잘못 지어서 못 찾았다면 경고를 띄웁니다.
        assert(bFound && "레벨에서 인벤토리 컨텐츠 오브젝트를 찾을 수 없습니다!");
    }

    GetOwner()->SetActive(false);
    m_iCurrentTab = 0;

    // 3. 이제 벡터가 채워졌으므로 안전하게 호출 가능합니다.
    UpdateTabUI(m_iCurrentTab);
}

CInventoryScript::~CInventoryScript()
{
}

void CInventoryScript::Tick()
{
    if (GetOwner()->GetParent() != nullptr) return;

    // 1. 인벤토리 열기/닫기
    if (KEY_TAP(KEY::I))
    {
        bool bActive = !GetOwner()->IsActive();
        GetOwner()->SetActive(bActive);

        // 열 때 현재 탭의 슬롯 상태를 최신화
        if (bActive) UpdateTabUI(m_iCurrentTab);
    }
    else if (KEY_TAP(KEY::ESC))
    {
        GetOwner()->SetActive(false);
    }

    if (!GetOwner()->IsActive()) return;

    // 2. 탭 클릭 체크
    if (KEY_TAP(KEY::LBTN))
    {
        Vec2 vMousePos = KeyMgr::GetInst()->GetMousePos();

        // 해상도 값은 엔진 환경에 맞게 넣어주세요 (예: 1600, 900)
        float fHWidth = 1600.f / 2.f;
        float fHHeight = 900.f / 2.f;

        // 좌표계 일치 작업 (중앙 0,0 기준)
        Vec2 vCenteredMouse = Vec2(vMousePos.x - fHWidth, fHHeight - vMousePos.y);

        Vec3 vInvPos = Transform()->GetWorldPos();
        Vec2 vLocalMouse = Vec2(vCenteredMouse.x - vInvPos.x, vCenteredMouse.y - vInvPos.y);

        // 이제 이 vLocalMouse를 가지고 버튼의 Rect 판정을 수행합니다.
        const vector<Ptr<GameObject>>& vecChildren = GetOwner()->GetChild();
        int iTabIdx = 0;

        for (size_t i = 1; i < vecChildren.size(); ++i) // 0번 X버튼 스킵
        {
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

void CInventoryScript::AddItem(Ptr<tItemInfo> _pItem)
{
    if (nullptr == _pItem) return;

    // 전달받은 아이템 정보를 그대로 탭에 맞게 분류
    UINT iTargetTab = (UINT)_pItem->eType;

    if (m_vecInventory[iTargetTab].size() < 24)
    {
        // 핵심: 새로 New 하지 않고 받은 포인터를 그대로 보관
        m_vecInventory[iTargetTab].push_back(_pItem);
    }

    // UI 갱신 (슬롯에 아이콘 그려주기)
    UpdateTabUI(m_iCurrentTab);
}

void CInventoryScript::UpdateTabUI(int _iTabIdx)
{
    // 1. 현재 선택된 탭 컨테이너(부모)가 가진 슬롯 자식들을 가져옴
    const vector<Ptr<GameObject>>& vecSlots = m_vecTabContents[_iTabIdx]->GetChild();

    // 2. 현재 데이터 상의 아이템 목록 가져옴
    const vector<Ptr<tItemInfo>>& vecItems = m_vecInventory[_iTabIdx];

    for (size_t i = 0; i < vecSlots.size(); ++i)
    {
        // 각 슬롯 오브젝트에 붙어있는 CSlotScript를 찾음
        Ptr<CSlotScript> pSlotScript = vecSlots[i]->GetScript<CSlotScript>();
        if (nullptr == pSlotScript) continue;

        // 슬롯 인덱스에 맞는 데이터가 있으면 전달, 없으면 nullptr 전달
        if (i < vecItems.size())
        {
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

    // 1. 기존 탭 처리
    // 컨텐츠 숨기기
    if (m_vecTabContents[m_iCurrentTab])
        m_vecTabContents[m_iCurrentTab]->SetActive(false);

    // 버튼 애니메이션 0번(비활성)으로 변경
    const vector<Ptr<GameObject>>& vecChildren = GetOwner()->GetChild();
    int iTabCount = 0;
    for (auto& child : vecChildren)
    {
        if (child->GetName().find(L"_Button") != wstring::npos)
        {
            if (iTabCount == m_iCurrentTab)
            {
                child->FlipbookRender()->Play(0, 1.f, -1);
                break;
            }
            iTabCount++;
        }
    }

    // 2. 현재 탭 인덱스 갱신
    m_iCurrentTab = _iNextTab;

    // 3. 새로운 탭 처리
    // 컨텐츠 보이기
    if (m_vecTabContents[m_iCurrentTab])
    {
        m_vecTabContents[m_iCurrentTab]->SetActive(true);
        UpdateTabUI(m_iCurrentTab);
    }

    // 버튼 애니메이션 1번(활성)으로 변경
    iTabCount = 0;
    for (auto& child : vecChildren)
    {
        if (child->GetName().find(L"_Button") != wstring::npos)
        {
            if (iTabCount == m_iCurrentTab)
            {
                child->FlipbookRender()->Play(1, 1.f, -1);
                break;
            }
            iTabCount++;
        }
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
