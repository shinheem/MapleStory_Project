#include "pch.h"
#include "FlipbookUI.h"

#include "AssetMgr.h"
#include "PathMgr.h"

#include "AFlipbook.h"
#include "ASprite.h"
#include "ATexture.h"

FlipbookUI::FlipbookUI()
	: AssetUI(ASSET_TYPE::FLIPBOOK)
	, m_SelectedIndex(0)
{
}

FlipbookUI::~FlipbookUI()
{
}

void FlipbookUI::Tick_UI()
{
	OutputTitle();

	Ptr<AFlipbook> pFlipbook = (AFlipbook*)GetTargetAsset().Get();
	if (!pFlipbook) return;

	auto& sprites = pFlipbook->GetSprites();
	int spriteCount = (int)sprites.size();

	ImGui::Text("Flipbook Name");
	ImGui::SameLine();
	string flipName(pFlipbook->GetKey().begin(), pFlipbook->GetKey().end());
	ImGui::InputText("##FlipbookName", flipName.data(), flipName.size() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::Separator();
	ImGui::Spacing();

	if (spriteCount > 0)
	{
		if (m_SelectedIndex >= spriteCount) m_SelectedIndex = spriteCount - 1;
		if (m_SelectedIndex < 0) m_SelectedIndex = 0;

		auto& selectedSprite = sprites[m_SelectedIndex];
		Ptr<ATexture> atlas = selectedSprite->GetAtlas();
		if (atlas)
		{
			Vec2 leftTop = selectedSprite->GetLeftTopUV();
			Vec2 slice = selectedSprite->GetSliceUV();
			float atlasHeight = (float)atlas->GetHeight();
			ImVec2 uv0(leftTop.x, (leftTop.y / atlasHeight) / 4.f);
			ImVec2 uv1(leftTop.x + slice.x, uv0.y + slice.y);

			ImGui::Text("Preview");
			ImGui::ImageWithBg(
				(ImTextureRef)atlas->GetSRV().Get(),
				ImVec2(200, 200),
				uv0, uv1,
				ImVec4(0.f, 0.f, 0.f, 1.f)
			);
		}
		ImGui::Text("Frame Index");
		ImGui::SameLine();
		if (ImGui::InputInt("##FrameIndex", &m_SelectedIndex))
		{
			if (m_SelectedIndex >= spriteCount) m_SelectedIndex = spriteCount - 1;
			if (m_SelectedIndex < 0) m_SelectedIndex = 0;
		}
	}
}