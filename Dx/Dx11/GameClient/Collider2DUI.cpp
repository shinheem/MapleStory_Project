#include "pch.h"
#include "Collider2DUI.h"
#include "GameObject.h"
#include "CCollider2D.h"

Collider2DUI::Collider2DUI()
    : ComponentUI(COMPONENT_TYPE::COLLIDER2D, "Collider2D")
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Tick_UI()
{
    Ptr<GameObject> obj = GetTarget();
    if (nullptr == obj.Get())
        return;

    Ptr<CCollider2D> col = obj->Collider2D();
    if (nullptr == col)
        return; // Collider 없음 → UI 없음

    OutputTitle("Collider2D");

    Vec2 offset = col->GetOffset();
    ImGui::Text("Offset");
    ImGui::SameLine(100);
    if (ImGui::DragFloat2("##Offset", &offset.x, 0.01f))
        col->SetOffset(offset);

    Vec2 scale = col->GetScale();
    ImGui::Text("Scale");
    ImGui::SameLine(100);
    if (ImGui::DragFloat2("##Scale", &scale.x, 0.01f))
        col->SetScale(scale);

}