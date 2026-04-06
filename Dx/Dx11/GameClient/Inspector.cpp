#include "pch.h"
#include "Inspector.h"

#include "PathMgr.h"
#include "LevelMgr.h"
#include "TaskMgr.h"
#include "AssetMgr.h"
#include "Source\ScriptMgr.h"

#include "ALevel.h"
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

#include <filesystem>
namespace fs = std::filesystem;

Inspector::Inspector()
    : EditorUI("Inspector")
    , m_SelectedLevelIdx(-1)
{
    CreateChildUI();
    SetTargetObject(nullptr);

    // 처음에 목록 한 번 갱신
    RefreshLevelList();
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

            // Light
            if (nullptr == m_TargetObject->GetComponent(COMPONENT_TYPE::LIGHT2D))
            {
                if (ImGui::Button("Add Light2D"))
                {
                    TaskInfo info{};
                    info.Type = TASK_TYPE::ADD_COMPONENT;
                    info.Param_0 = (DWORD_PTR)m_TargetObject.Get();
                    info.Param_1 = (DWORD_PTR)new CLight2D;
                    TaskMgr::GetInst()->AddTask(info);

                    m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetActive(true);
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
            ITEMSCRIPT, CURSORSCRIPT, FADESCRIPT
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

void Inspector::RefreshLevelList()
{
    m_vecLevelNames.clear();
    wstring levelPath = CONTENT_PATH;
    levelPath += L"Level\\";

    if (!fs::exists(levelPath)) return;

    for (const auto& entry : fs::directory_iterator(levelPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".lv")
        {
            m_vecLevelNames.push_back(entry.path().stem().string());
        }
    }
}

void Inspector::Tick_UI()
{
    // --- 1. 레벨 매니저 섹션 (파일 리스트 및 로드/저장) ---
    if (ImGui::CollapsingHeader("Level Manager", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // 목록 갱신 버튼
        if (ImGui::Button("Refresh List")) { RefreshLevelList(); }

        ImGui::SameLine();
        string currentName = m_SelectedLevelIdx != -1 ? m_vecLevelNames[m_SelectedLevelIdx] : "None";
        ImGui::Text("Selected: %s", currentName.c_str());

        // 레벨 파일 리스트 박스
        if (ImGui::BeginListBox("##LevelList", ImVec2(-FLT_MIN, 100.f)))
        {
            for (int i = 0; i < (int)m_vecLevelNames.size(); ++i)
            {
                const bool is_selected = (m_SelectedLevelIdx == i);
                if (ImGui::Selectable(m_vecLevelNames[i].c_str(), is_selected))
                {
                    m_SelectedLevelIdx = i;
                }
            }
            ImGui::EndListBox();
        }

        // 선택된 레벨이 있을 때만 로드/저장 버튼 활성화
        if (m_SelectedLevelIdx != -1)
        {
            // [Load] 버튼 수정 부분
            if (ImGui::Button("Load Selected", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0)))
            {
                // 1. 불러올 파일명 계산
                wstring fileName = StrToWstr(m_vecLevelNames[m_SelectedLevelIdx]);
                wstring relativePath = L"Level\\" + fileName + L".lv";

                // 2. AssetMgr에 로드 (이때 relativePath가 Key값이 됨)
                Ptr<ALevel> pNextLevel = AssetMgr::GetInst()->Load<ALevel>(relativePath, relativePath);

                if (nullptr != pNextLevel)
                {
                    // [핵심] static 변수를 사용하여 메모리를 고정합니다.
                    // 이 변수는 함수가 끝나도 파괴되지 않고 다음 프레임까지 주소값을 유지합니다.
                    static wstring s_SafePath;
                    s_SafePath = relativePath;

                    TaskInfo info = {};
                    info.Type = TASK_TYPE::CHANGE_LEVEL;

                    // s_SafePath의 주소를 넘깁니다. 
                    // TaskMgr가 다음 프레임에 이 주소를 참조해도 s_SafePath는 살아있으므로 안전합니다.
                    info.Param_0 = (DWORD_PTR)s_SafePath.c_str();

                    TaskMgr::GetInst()->AddTask(info);

                    SetTargetObject(nullptr);
                    return;
                }
            }

            ImGui::SameLine();

            // [Save] 버튼
            if (ImGui::Button("Save to Selected", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();
                if (pCurLevel != nullptr)
                {
                    wstring fileName = StrToWstr(m_vecLevelNames[m_SelectedLevelIdx]);
                    pCurLevel->Save(CONTENT_PATH + L"Level\\" + fileName + L".lv");
                }
            }
        }
    }

    ImGui::Separator();
    // ---------------------------------------------------

    // --- 2. 기존 Inspector 로직 ---
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

    /*if (ImGui::Button("Save##LevelSaveBtn"))
    {
        Ptr<ALevel> level = LevelMgr::GetInst()->GetCurLevel();
        wstring saveName = L"";
        if (m_SelectedLevelIdx != -1)
            saveName = StrToWstr(m_vecLevelNames[m_SelectedLevelIdx]) + L".lv";

        level->Save(CONTENT_PATH + L"Level\\" + saveName);
    }*/

    ImGui::Separator();

    AddComponents();
    AddScriptUI();
    DrawLayerSelectorUI(m_TargetObject);

    UpdatePrefabButtons();
}

void Inspector::UpdatePrefabButtons()
{
    if (nullptr == m_TargetObject) return;

    ImGui::Separator();
    if (ImGui::Button("Save as Prefab", ImVec2(150, 30)))
    {
        CreatePrefabFromTarget();
    }
}
void Inspector::CreatePrefabFromTarget()
{
    if (nullptr == m_TargetObject) return;

    // 1. 현재 타겟 오브젝트를 복사 (Clone)
    // 원본 객체를 직접 넣으면 레벨에서 삭제될 때 프리팹도 깨질 수 있으므로 복사본을 만듭니다.
    GameObject* pClone = m_TargetObject->Clone();

    // 2. 프리팹 에셋 생성
    Ptr<APrefab> pNewPrefab = new APrefab;
    pNewPrefab->SetObject(pClone);

    // 3. 경로 결정 (예: Prefab 폴더 아래 오브젝트 이름으로)
    wstring PrefabName = L"Prefab\\" + m_TargetObject->GetName() + L".pref";

    // 4. 에셋 매니저에 등록 및 파일 저장
    AssetMgr::GetInst()->AddAsset(PrefabName, pNewPrefab.Get());

    wstring FullPath = CONTENT_PATH + PrefabName;
    pNewPrefab->Save(FullPath);

    // 알림 (로그 창이 있다면 출력)
    // LogMgr::AddLog(L"Prefab Saved: " + PrefabName);
}