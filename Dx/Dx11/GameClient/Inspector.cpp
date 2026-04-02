#include "pch.h"
#include "Inspector.h"

#include "PathMgr.h"
#include "LevelMgr.h"
#include "TaskMgr.h"
#include "Source\ScriptMgr.h"

#include "GameObject.h"
#include "CFlipbookRender.h"

#include "Source\Scripts\CBossPartScript.h"
#include "Source\Scripts\CCamMoveScript.h"
#include "Source\Scripts\CHonTailBossScript.h"
#include "Source\Scripts\CMissileScript.h"
#include "Source\Scripts\CMonsterScript.h"
#include "Source\Scripts\CPlayerScript.h"
#include "Source\Scripts\CInventoryScript.h"
#include "Source\Scripts\CGaugeBarScript.h"

Inspector::Inspector()
    : EditorUI("Inspector")
{
    CreateChildUI();
    SetTargetObject(nullptr);
}

Inspector::~Inspector()
{
}

void Inspector::SetTargetObject(Ptr<GameObject> _Object)
{
    m_TargetObject = _Object;

    // ComponentUI 업데이트
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComUI[i])
            continue;

        m_arrComUI[i]->SetTarget(m_TargetObject);
    }

    // ScriptUI 업데이트
    if (nullptr != m_TargetObject)
    {
        const vector<Ptr<CScript>>& vecScripts = m_TargetObject->GetScripts();

        if (m_vecScriptUI.size() < vecScripts.size())
        {
            int AddCount = vecScripts.size() - m_vecScriptUI.size();

            for (int i = 0; i < AddCount; ++i)
            {
                ScriptUI* pScriptUI = new ScriptUI;
                pScriptUI->SetSizeAsChild(Vec2(0.f, 150.f));
                AddChildUI(pScriptUI);

                m_vecScriptUI.push_back(pScriptUI);
            }
        }

        for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
        {
            if (vecScripts.size() <= i)
                m_vecScriptUI[i]->SetScript(nullptr);
            else
                m_vecScriptUI[i]->SetScript(vecScripts[i].Get());
        }
    }
    else
    {
        for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
        {
            m_vecScriptUI[i]->SetScript(nullptr);
        }
    }

    // AssetUI 비활성화
    m_TargetAsset = nullptr;
    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
    {
        if (nullptr != m_arrAssetUI[i])
            m_arrAssetUI[i]->SetActive(false);
    }
}

// 기존 Component 추가 버튼
void Inspector::AddComponents()
{
    if (ImGui::CollapsingHeader("Add Component"))
    {
        if (nullptr != m_TargetObject)
        {
            // Transform
            if (nullptr == m_TargetObject->GetComponent(COMPONENT_TYPE::TRANSFORM))
            {
                if (ImGui::Button("Add Transform"))
                {
                    TaskInfo info{};
                    info.Type = TASK_TYPE::ADD_COMPONENT;
                    info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                    info.Param_1 = (DWORD_PTR)new CTransform;
                    TaskMgr::GetInst()->AddTask(info);

                    m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetActive(true);
                }
            }

            // Camera
            if (nullptr == m_TargetObject->GetComponent(COMPONENT_TYPE::CAMERA))
            {
                if (ImGui::Button("Add Camera"))
                {
                    TaskInfo info{};
                    info.Type = TASK_TYPE::ADD_COMPONENT;
                    info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                    info.Param_1 = (DWORD_PTR)new CCamera;
                    TaskMgr::GetInst()->AddTask(info);

                    m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetActive(true);
                }
            }

            // Collider2D
            if (nullptr == m_TargetObject->GetComponent(COMPONENT_TYPE::COLLIDER2D))
            {
                if (ImGui::Button("Add Collider2D"))
                {
                    TaskInfo info{};
                    info.Type = TASK_TYPE::ADD_COMPONENT;
                    info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                    info.Param_1 = (DWORD_PTR)new CCollider2D;
                    TaskMgr::GetInst()->AddTask(info);

                    m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetActive(true);
                }
            }

            // Render Components (배타성)
            auto render = m_TargetObject->GetRenderComponent();
            if (nullptr == render)
            {
                if (ImGui::Button("Add MeshRender"))
                {
                    TaskInfo info{};
                    info.Type = TASK_TYPE::ADD_COMPONENT;
                    info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                    info.Param_1 = (DWORD_PTR)new CMeshRender;
                    TaskMgr::GetInst()->AddTask(info);
                }

                if (ImGui::Button("Add FlipBookRender"))
                {
                    TaskInfo info{};
                    info.Type = TASK_TYPE::ADD_COMPONENT;
                    info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                    info.Param_1 = (DWORD_PTR)new CFlipbookRender;
                    TaskMgr::GetInst()->AddTask(info);
                }
            }
            else
            {
                ImGui::Text("RenderComponent already exists");
            }
        }
    }
}

void Inspector::AddScriptUI()
{
    if (!m_TargetObject) return;

    if (ImGui::CollapsingHeader("Add Script"))
    {
        static const vector<SCRIPT_TYPE> AllScripts = {
            BOSSPARTSCRIPT, CAMMOVESCRIPT, HONTAILBOSSSCRIPT,
            MISSILESCRIPT, MONSTERSCRIPT, PLAYERSCRIPT, 
            INVENTORYSCRIPT, GAUGEBARSCRIPT, SLOTSCRIPT,
            ITEMSCRIPT
        };

        for (SCRIPT_TYPE type : AllScripts)
        {
            const wchar_t* pWName = ScriptMgr::GetScriptNameByType(type);
            wstring WName = pWName ? pWName : L"Unknown";
            string strName(WName.begin(), WName.end());

            // 중복 체크 로직
            bool bExists = false;
            for (const auto& s : m_TargetObject->GetScripts())
            {
                if (s->GetScriptType() == type)
                {
                    bExists = true;
                    break;
                }
            }

            if (bExists)
            {
                ImGui::Text("%s (Added)", strName.c_str());
            }
            else
            {
                if (ImGui::Button(strName.c_str()))
                {
                
                    CScript* pNewScript = ScriptMgr::GetScript((UINT)type);
               

                    TaskInfo info{};
                    info.Type = TASK_TYPE::ADD_COMPONENT;
                    info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                    info.Param_1 = (DWORD_PTR)pNewScript;
                    TaskMgr::GetInst()->AddTask(info);

                    SetTargetObject(m_TargetObject);
                    return; 
                }
            }
        }
    }

    // Remove Script UI
    if (ImGui::CollapsingHeader("Remove Script"))
    {
        for (const auto& s : m_TargetObject->GetScripts())
        {
            wstring WName = ScriptMgr::GetScriptName(s.Get());
            string strName(WName.begin(), WName.end());

            ImGui::Text("%s", strName.c_str());
            ImGui::SameLine();

            if (ImGui::SmallButton(("Remove##" + strName).c_str()))
            {
                TaskInfo info{};
                info.Type = TASK_TYPE::REMOVE_COMPONENT;
                info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                info.Param_1 = (DWORD_PTR)s.Get();
                TaskMgr::GetInst()->AddTask(info);

                SetTargetObject(m_TargetObject);
                return;
            }
        }
    }
    SetTargetObject(m_TargetObject);
}

// AssetUI
void Inspector::SetTargetAsset(Ptr<Asset> _Asset)
{
    SetTargetObject(nullptr);

    m_TargetAsset = _Asset;
    if (nullptr == m_TargetAsset)
    {
        for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
        {
            m_arrAssetUI[i]->SetActive(false);
        }
    }
    else
    {
        ASSET_TYPE Type = m_TargetAsset->GetType();
        m_arrAssetUI[(UINT)Type]->SetActive(true);
        m_arrAssetUI[(UINT)Type]->SetTargetAsset(m_TargetAsset);
    }
}

void Inspector::ChangeObjectLayer(Ptr<GameObject> obj, int newLayer)
{
    if (!obj)
        return;

    Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
    if (!pLevel)
        return;

    int oldLayer = obj->GetLayerIdx();

    // 1️⃣ 기존 레이어에서 제거
    if (oldLayer >= 0 && oldLayer < pLevel->GetLayerCount())
    {
        Layer* oldLayerPtr = pLevel->GetLayer(oldLayer);
        if (oldLayerPtr)
        {
            oldLayerPtr->RemoveObject(obj);
        }
    }

    // 2️⃣ 새 레이어에 추가
    if (newLayer >= 0 && newLayer < pLevel->GetLayerCount())
    {
        pLevel->AddObject(newLayer, obj);
        obj->SetLayerIdx(newLayer);
    }
}

void Inspector::DrawLayerSelectorUI(Ptr<GameObject> targetObject)
{
    if (!targetObject)
        return;

    Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
    if (!pLevel)
        return;

    // 접기/펼치기
    if (ImGui::CollapsingHeader("Layer Selector"))
    {
        int currentLayer = targetObject->GetLayerIdx();
        int layerCount = pLevel->GetLayerCount();

        for (int i = 0; i < layerCount; ++i)
        {
            Layer* layer = pLevel->GetLayer(i);
            if (!layer) continue;

            // 레이어 이름 가져오기
            std::wstring wName = layer->GetName();
            std::string nameStr(wName.begin(), wName.end());

            // 표시 텍스트: "0 - DefaultLayer" 형식
            std::string displayStr = std::to_string(i) + " - " + nameStr;

            bool isSelected = (currentLayer == i);

            if (ImGui::Selectable(displayStr.c_str(), isSelected))
            {
                // 클릭 시 레이어 이동
                ChangeObjectLayer(targetObject, i);
            }
        }
    }
}

// UI Tick
void Inspector::Tick_UI()
{
    float buttonWidth = 40.f;

    if (nullptr == m_TargetObject)
        return;

    wstring Name = m_TargetObject->GetName();
    string strName(Name.begin(), Name.end());
    if (strName.empty())
        strName = "No Name";

    ImGui::Button(strName.c_str());
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - buttonWidth);

    if (ImGui::Button("Save##LevelSaveBtn"))
    {
        Ptr<ALevel> level = LevelMgr::GetInst()->GetCurLevel();
        level->Save(CONTENT_PATH + L"Level\\TestLevel.lv");
    }

    ImGui::Separator();

    // 기존 Component 추가 UI
    AddComponents();

    // Script Add/Remove UI
    AddScriptUI();

    DrawLayerSelectorUI(m_TargetObject);
}