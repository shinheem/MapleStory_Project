#include "pch.h"
#include "ComponentUI.h"

#include "TaskMgr.h"
#include "EditorMgr.h" 

#include "ScriptUI.h"


ComponentUI::ComponentUI(COMPONENT_TYPE _Type, const string& _Name)
	: EditorUI(_Name)
	, m_ComType(_Type)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::SetTarget(Ptr<GameObject> _Object)
{
	m_Target = _Object;

		if (nullptr == m_Target || nullptr == m_Target->GetComponent(m_ComType))
			SetActive(false);
		else
			SetActive(true);
}

void ComponentUI::OutputTitle(const string& _Title)
{

    // 1. ID 충돌 방지를 위해 객체의 주소(this)를 ID로 사용
    ImGui::PushID(this);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.8f));

    // 2. 타이틀 버튼 (스크립트 이름)
    ImGui::Button(_Title.c_str());

    // 3. 버튼 바로 옆에 붙이기
    ImGui::SameLine();

    if (COMPONENT_TYPE::TRANSFORM != m_ComType && nullptr != m_Target)
    {
        Ptr<Component> pTargetComp = nullptr;

       
        if (m_ComType == COMPONENT_TYPE::SCRIPT)
        {
            pTargetComp = ((ScriptUI*)this)->GetTargetScript().Get();
        }
        else
        {
            pTargetComp = m_Target->GetComponent(m_ComType);
        }

        // --- [삭제 버튼 출력] ---
        if (nullptr != pTargetComp)
        {
            if (ImGui::SmallButton("X##Remove"))
            {
                TaskInfo info{};
                info.Type = TASK_TYPE::REMOVE_COMPONENT;
                info.Param_0 = (DWORD_PTR)m_Target.Get();
                info.Param_1 = (DWORD_PTR)pTargetComp.Get();
                TaskMgr::GetInst()->AddTask(info);

               
                SetActive(false);
            }
        }
    }

    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Spacing();
    ImGui::Spacing();
}