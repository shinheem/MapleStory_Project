#include "pch.h"
#include "SpriteUI.h"

#include "AssetMgr.h"
#include "PathMgr.h"

#include "ASprite.h"

SpriteUI::SpriteUI()
	: AssetUI(ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Tick_UI()
{
	float buttonWidth = 100.f;

	OutputTitle();

	//ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - buttonWidth);

	// Save 버튼
	if (ImGui::Button("Save Sprite"))
	{
		Ptr<ASprite> pSprite = (ASprite*)GetTargetAsset().Get();
		if (pSprite)
		{
			// 상대 경로 + CONTENT_PATH
			wstring fullPath = CONTENT_PATH + pSprite->GetRelativePath();

			if (pSprite->Save(fullPath) == 0)
				wprintf(L"[Info] Sprite '%s' saved successfully.\n", pSprite->GetKey().c_str());
			else
				wprintf(L"[Error] Failed to save sprite '%s'.\n", pSprite->GetKey().c_str());
		}
	}

	Ptr<ASprite> pSprite = (ASprite*)GetTargetAsset().Get();
	if (!pSprite) return;

	string Key = string(pSprite->GetKey().begin(), pSprite->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(120);
	ImGui::InputText("##SpriteName", Key.data(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	// Sprite 에 Atlas 텍스쳐가 세팅되어있는지 확인
	Ptr<ATexture> pAtlas = pSprite->GetAtlas();
	bool IsAtlas = pAtlas.Get();

	// Atlas 이름
	string AtlasName = "None";
	if(IsAtlas)
		AtlasName = string(pAtlas->GetKey().begin(), pAtlas->GetKey().end());

	ImGui::Text("Atlas Name");
	ImGui::SameLine(120);
	ImGui::InputText("##AtlasName", AtlasName.data(), AtlasName.length() + 1, ImGuiInputTextFlags_ReadOnly);

	if (pAtlas)
	{
		
		float atlasHeight = (float)pAtlas->GetHeight();
		Vec2 leftTop = pSprite->GetLeftTopUV();
		Vec2 slice = pSprite->GetSliceUV(); // UV 단위

		ImVec2 uv0(leftTop.x, (leftTop.y / atlasHeight) / 4.f);

		ImVec2 uv1(leftTop.x + slice.x, uv0.y + slice.y);

		ImGui::Text("Preview");
		ImGui::ImageWithBg(
			(ImTextureRef)pAtlas->GetSRV().Get(),
			ImVec2(200, 200),
			uv0,
			uv1,
			ImVec4(0.f, 0.f, 0.f, 1.f)
		);
	}

	// Sprite UV 정보
	Vec2 LeftTopUV = pSprite->GetLeftTopUV();
	Vec2 SliceUV = pSprite->GetSliceUV();
	Vec2 BackgroundUV = pSprite->GetBackgroundUV();
	Vec2 OffsetUV = pSprite->GetOffsetUV();
	
	ImGui::Text("LeftTop");
	ImGui::BeginDisabled(!IsAtlas);
	{		
		if (IsAtlas)
		{
			float pixel[2] = { (float)LeftTopUV.x * (float)pAtlas->GetWidth()
						  ,  (float)LeftTopUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputFloat2("##LeftTop", pixel))
			{
				pSprite->SetLeftTopUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth() 
										 , (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##LeftTop", pixel);
		}		
	}	
	ImGui::EndDisabled();

	ImGui::Text("Slice");	
	ImGui::BeginDisabled(!IsAtlas);
	{		
		if (IsAtlas)
		{
			float pixel[2] = { (float)SliceUV.x * (float)pAtlas->GetWidth()
						  ,  (float)SliceUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputFloat2("##Slice", pixel))
			{
				pSprite->SetSliceUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth() 
									   , (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Slice", pixel);
		}		
	}	
	ImGui::EndDisabled();


	ImGui::Text("Background");	
	ImGui::BeginDisabled(!IsAtlas);
	{		
		if (IsAtlas)
		{
			float pixel[2] = { (float)BackgroundUV.x * (float)pAtlas->GetWidth()
						  ,  (float)BackgroundUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputFloat2("##Background", pixel))
			{
				pSprite->SetBackgroundUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth() 
										    , (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Background", pixel);
		}		
	}	
	ImGui::EndDisabled();

	ImGui::Text("Offset");	
	ImGui::BeginDisabled(!IsAtlas);
	{		
		if (IsAtlas)
		{
			float pixel[2] = { (float)OffsetUV.x * (float)pAtlas->GetWidth()
						  ,  (float)OffsetUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputFloat2("##Offset", pixel))
			{
				pSprite->SetOffsetUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth() 
										, (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Offset", pixel);
		}		
	}	
	ImGui::EndDisabled();

	// ---------------------------
	// Delete Sprite 버튼
	// ---------------------------
	static wstring keyToDeselect;

	if (ImGui::Button("Delete Sprite"))
	{
		Ptr<ASprite> pSprite = (ASprite*)GetTargetAsset().Get();
		if (!pSprite) return;

		wstring key = pSprite->GetKey();

		bool bReferenced = false;
		auto flipMap = AssetMgr::GetInst()->GetmapAsset(ASSET_TYPE::FLIPBOOK);
		for (const auto& pair : flipMap)
		{
			AFlipbook* flip = (AFlipbook*)pair.second.Get();
			for (auto& sprite : flip->GetSprites())
			{
				if (sprite->GetKey() == key)
				{
					bReferenced = true;
					break;
				}
			}
			if (bReferenced) break;
		}

		if (!bReferenced)
		{
			AssetMgr::GetInst()->RemoveAsset(ASSET_TYPE::SPRITE, key);
			DeleteFile((CONTENT_PATH + key).c_str());

			// 즉시 nullptr 대신 다음 프레임 처리
			keyToDeselect = key;

			wprintf(L"[Info] Sprite '%s' deleted successfully.\n", key.c_str());
		}
		else
		{
			wprintf(L"[Warning] Cannot delete sprite '%s', still referenced!\n", key.c_str());
		}
	}

	// 프레임 끝에서 안전하게 선택 해제
	if (!keyToDeselect.empty())
	{
		Ptr<Asset> target = GetTargetAsset();
		if (target && target->GetKey() == keyToDeselect)
		{
			SetTargetAsset(nullptr);
		}
		keyToDeselect.clear();
	}
}
