#include "pch.h"
#include "FlipbookRenderUI.h"

#include "GameObject.h"
#include "CFlipbookRender.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "ListUI.h"

FlipbookRenderUI::FlipbookRenderUI()
    : ComponentUI(COMPONENT_TYPE::FLIPBOOK_RENDER, "FlipbookRenderUI")
    , m_CurIdx()
{
}

FlipbookRenderUI::~FlipbookRenderUI()
{
}

// FlipbookRenderUI.cpp
void FlipbookRenderUI::Tick_UI()
{
    OutputTitle("FlipbookRender");

    // Index 입력란
    ImGui::InputInt("Index##FlipbookIdx", &m_CurIdx);
    if (m_CurIdx < 0) m_CurIdx = 0;

    Ptr<CFlipbookRender> pFlipbookRender = GetTarget()->FlipbookRender();
    if (nullptr == pFlipbookRender) return;

    Ptr<AFlipbook> pFlipbook = pFlipbookRender->GetFlipbook(m_CurIdx);

    string FlipbookKey = "";
    if (nullptr != pFlipbook) FlipbookKey = string(pFlipbook->GetKey().begin(), pFlipbook->GetKey().end());

    ImGui::InputText("##FlipbookName", FlipbookKey.data(), FlipbookKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

    // 드래그&드롭
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content");
        if (payload)
        {
            DWORD_PTR data = *((DWORD_PTR*)payload->Data);
            Ptr<Asset> pAsset = (Asset*)data;
            if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
            {
                pFlipbookRender->SetFlipbook(m_CurIdx, (AFlipbook*)pAsset.Get());
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::SameLine();

    // 리스트 버튼
    if (ImGui::Button("##FlipbookBtn", Vec2(20.f, 20.f)))
    {
        Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorMgr::GetInst()->FindUI("ListUI").Get());
        assert(pUI.Get());

        pUI->SetUIName("Flipbook List");

        vector<wstring> vecNames;
        AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecNames);

        pUI->AddString(vecNames);
        pUI->AddDelegate(this, (DELEGATE_1)&FlipbookRenderUI::SelectFlipbook); // 기존 DELEGATE_1 사용
        pUI->SetActive(true);
    }
}

void FlipbookRenderUI::SelectFlipbook(DWORD_PTR _ListUI)
{
    Ptr<ListUI> pListUI = (ListUI*)_ListUI;
    wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

    Ptr<AFlipbook> pFlipbook = FIND(AFlipbook, key);

    GetTarget()->FlipbookRender()->SetFlipbook(m_CurIdx, pFlipbook); // m_CurIdx 사용
}