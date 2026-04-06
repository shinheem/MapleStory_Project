#include "pch.h"
#include "Menu.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "ContentUI.h"
#include "LevelMgr.h"
#include "TaskMgr.h"

#include "Inspector.h"
#include "GameObject.h"

#include "Source/ScriptMgr.h"

Menu::Menu()
	: EditorUI("Menu")
	, m_bAddTextureWindow(false)
	, m_bCreateSpriteWindow(false)
	, m_bCreateFlipbookWindow(false)
	, m_bCreateGameObjectWindow(false)
	, m_bCollisionWindow(false)
	, m_bLayerNameWindow(false)
	, m_bCreateMaterialWindow(false)
	, m_bCreateLevelWindow(false)
	, m_CollisionMatrix{}
{
}

Menu::~Menu()
{
}

void Menu::Tick_UI()
{
}


void Menu::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		File();

		Level();
		
		View();

		GameObjectMenu();

		Asset();		
		if(m_bLayerNameWindow)
			UpdateayerNameEditorWindow();

		if (m_bCollisionWindow)
			CollisionWindow();

		if (m_bCreateGameObjectWindow)
			CreateGameObjectWindow();

		if (m_bCreateSpriteWindow)
			CreateSpriteWindow();

		if (m_bCreateFlipbookWindow)
			CreateFlipbookWindow();

		if (m_bCreateMaterialWindow)
			CreateMaterialWindow();

		if (m_bCreateLevelWindow)
			CreateLevelWindow();

		ImGui::EndMainMenuBar();
	}
}




void Menu::File()
{
	if (ImGui::BeginMenu("File"))
	{
		// 1. 새 레벨 생성 (도화지 새로 만들기)
		if (ImGui::MenuItem("New Level"))
		{
			m_bCreateLevelWindow = true;
		}

		ImGui::Separator();

		// 2. 현재 레벨 저장 (이미 이름이 있는 경우 해당 이름으로, 없으면 기본 이름으로)
		if (ImGui::MenuItem("Level Save", "Ctrl+S"))
		{
			Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();
			if (pCurLevel != nullptr)
			{
				wstring saveName = pCurLevel->GetName();
				// 확장자가 이미 있다면 붙이지 않고, 없다면 붙여줌
				if (saveName.find(L".lv") == wstring::npos)
					saveName += L".lv";

				wstring savePath = CONTENT_PATH + L"Level\\" + saveName;
				pCurLevel->Save(savePath);
			}
		}

		// 3. 레벨 불러오기 (간단한 구현을 위해 일단 로직만 배치)
		// 실제로는 파일 선택창(ImGui FileBrowser 등)을 띄우는 것이 좋습니다.
		if (ImGui::MenuItem("Level Load", "Ctrl+L"))
		{
			// 예시로 작성된 로드 로직
			wstring loadPath = L"Level\\Hontail_Boss_Level.lv";
			Ptr<ALevel> pLevel = AssetMgr::GetInst()->Load<ALevel>(loadPath, loadPath);

			if (pLevel != nullptr)
			{
				// [핵심] static 변수로 경로 문자열을 고정
				static wstring s_LoadPath;
				s_LoadPath = loadPath;

				TaskInfo info = {};
				info.Type = TASK_TYPE::CHANGE_LEVEL;

				// TaskMgr가 다음 프레임에 읽을 때까지 살아있음
				info.Param_0 = (DWORD_PTR)s_LoadPath.c_str();

				TaskMgr::GetInst()->AddTask(info);
			}
		}

		ImGui::EndMenu();
	}
}

void Menu::Level()
{
	if (ImGui::BeginMenu("Level"))
	{
		bool HasLevel = LevelMgr::GetInst()->GetCurLevel().Get();
		bool IsPlay = false, IsPause = false, IsStop = false;
		if (HasLevel)
		{
			LEVEL_STATE CurState = LevelMgr::GetInst()->GetLevelState();
			if (LEVEL_STATE::PLAY == CurState)
				IsPlay = true;
			else if (LEVEL_STATE::PAUSE == CurState)
				IsPause = true;
			else if (LEVEL_STATE::STOP == CurState)
				IsStop = true;
		}

		if (ImGui::MenuItem("Play", nullptr, nullptr, HasLevel && !IsPlay))
		{
			ChangeLevelState(LEVEL_STATE::PLAY);
		}

		if (ImGui::MenuItem("Pause", nullptr, nullptr, HasLevel && IsPlay))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}

		if (ImGui::MenuItem("Stop", nullptr, nullptr, HasLevel && !IsStop))
		{
			ChangeLevelState(LEVEL_STATE::STOP);
		}

		ImGui::EndMenu();
	}
}

void Menu::View()
{
	if (ImGui::BeginMenu("View"))
	{
		bool ShowDemo = EditorMgr::GetInst()->IsShowDemo();
		if (ImGui::MenuItem("Demo", nullptr, &ShowDemo, true))
		{
			EditorMgr::GetInst()->ShowDemo(ShowDemo);
		}

		Ptr<EditorUI> pInspector = EditorMgr::GetInst()->FindUI("Inspector");
		bool InspectorActive = pInspector->IsActive();
		if (ImGui::MenuItem("Inspector", nullptr, &InspectorActive))
		{
			pInspector->SetActive(InspectorActive);
		}

		Ptr<EditorUI> pOutliner = EditorMgr::GetInst()->FindUI("Outliner");
		bool OutlinerActive = pOutliner->IsActive();
		if (ImGui::MenuItem("Outliner", nullptr, &OutlinerActive))
		{
			pOutliner->SetActive(OutlinerActive);
		}

		ImGui::EndMenu();
	}
}

void Menu::GameObjectMenu()
{
	if (ImGui::BeginMenu("GameObject"))
	{

		if (ImGui::MenuItem("Update LayerName"))
		{
			m_bLayerNameWindow = true;
		}

		if (ImGui::MenuItem("Update Layer Collider2D"))
		{
			m_bCollisionWindow = true;
		}

		if (ImGui::MenuItem("Create GameObject"))
		{
			m_bCreateGameObjectWindow = true;
		}

		if (ImGui::BeginMenu("Add Script"))
		{
			vector<wstring> vecScriptName;
			ScriptMgr::GetScriptInfo(vecScriptName);

			for (const auto& ScriptName : vecScriptName)
			{				
				if (ImGui::MenuItem(string(ScriptName.begin(), ScriptName.end()).c_str()))
				{					
					Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
					Ptr<GameObject> pObject = pInspector->GetTargetObject();

					if (nullptr != pObject)
					{
						CScript* pNewScript = ScriptMgr::GetScript(ScriptName);
						pObject->AddComponent(pNewScript);
					}					
				}				
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void Menu::Asset()
{
	if (ImGui::BeginMenu("Asset"))
	{
		if (ImGui::BeginMenu("Create Asset"))
		{

			if (ImGui::MenuItem("Create Material"))
			{
				m_bCreateMaterialWindow = true;
			}

			if (ImGui::MenuItem("Create Sprite"))
			{
				m_bCreateSpriteWindow = true;
			}

			if (ImGui::MenuItem("Create Flipbook"))
			{
				m_bCreateFlipbookWindow = true;
			}

			if (ImGui::MenuItem("Create TileMap"))
			{

			}
			ImGui::EndMenu();
		}	

		ImGui::EndMenu();
	}
}


wstring Menu::GetAssetName(ASSET_TYPE _Type, const wstring& _Name)
{
	wstring Ext;

	switch (_Type)
	{
	case ASSET_TYPE::MESH:
		Ext = L".mesh";
		break;
	case ASSET_TYPE::MATERIAL:
		Ext = L".mtrl";
		break;
	case ASSET_TYPE::TEXTURE:
	case ASSET_TYPE::SOUND:
	case ASSET_TYPE::GRAPHICSHADER:
	case ASSET_TYPE::COMPUTESHADER:
		assert(nullptr);
		break;
	case ASSET_TYPE::SPRITE:
		Ext = L".sprite";
		break;
	case ASSET_TYPE::FLIPBOOK:
		Ext = L".flip";
		break;
	case ASSET_TYPE::PREFAB:
		Ext = L".pref";
		break;
	case ASSET_TYPE::LEVEL:
		Ext = L".lv";
		break;
	}

	int i = 0;
	while (true)
	{
		wchar_t Num[50] = {};
		swprintf_s(Num, 50, L"_%d", i);

		wstring AssetName = wstring(_Name + Num + Ext);
		if (nullptr == AssetMgr::GetInst()->FindAsset(_Type, AssetName))
		{
			return AssetName;
		}

		i++;
	}
}

void Menu::CreateSpriteWindow()
{
	ImGui::Begin("Create Sprite", &m_bCreateSpriteWindow);

	static char texKey[64] = "";
	static float sliceX = 64;
	static float sliceY = 64;
	static int count = 1;

	ImGui::InputText("Texture Key", texKey, 64);
	ImGui::InputFloat("Slice X", &sliceX);
	ImGui::InputFloat("Slice Y", &sliceY);
	ImGui::InputInt("Count", &count);

	if (ImGui::Button("Create"))
	{
		CreateSpriteFromTexture(texKey, sliceX, sliceY, count);
	}

	ImGui::End();
}

	
void Menu::CreateSpriteFromTexture(const char* texKey, float sliceX, float sliceY, int count)
{
	wstring wTexKey(texKey, texKey + strlen(texKey));

	Ptr<ATexture> pTex = AssetMgr::GetInst()->Find<ATexture>(wTexKey);
	if (!pTex)
		return;

	float width = pTex->GetWidth();
	float height = pTex->GetHeight();

	Vec2 slicePixel = Vec2(sliceX, sliceY);

	for (int i = 0; i < count; ++i)
	{
		wchar_t name[100];
		swprintf_s(name, L"Sprite\\%s_%d.sprite", wTexKey.c_str(), i);

		Ptr<ASprite> pSprite = new ASprite;
		pSprite->SetName(name);
		pSprite->SetAtlas(pTex);

		pSprite->SetLeftTopUV(Vec2(
			(slicePixel.x / width) * i,
			0.f
		));

		pSprite->SetSliceUV(slicePixel / Vec2(width, height));
		pSprite->SetBackgroundUV(slicePixel / Vec2(width, height));

		AssetMgr::GetInst()->AddAsset(name, pSprite.Get());

		wstring path = CONTENT_PATH + name;
		pSprite->Save(path);
	}
}

void Menu::CreateFlipbookWindow()
{
	ImGui::Begin("Create Flipbook", &m_bCreateFlipbookWindow);

	static char prefix[64] = "";
	static int count = 1;

	ImGui::InputText("Sprite Prefix", prefix, 64);
	ImGui::InputInt("Count", &count);

	if (ImGui::Button("Create Flipbook"))
	{
		CreateFlipbook(prefix, count);
	}

	ImGui::End();
}

void Menu::CreateFlipbook(const char* prefix, int count)
{
	wstring wPrefix(prefix, prefix + strlen(prefix));

	wchar_t flipName[100];
	swprintf_s(flipName, L"Flipbook\\%s.flip", wPrefix.c_str());

	Ptr<AFlipbook> pFlip = new AFlipbook;
	pFlip->SetName(flipName);

	for (int i = 0; i < count; ++i)
	{
		wchar_t spriteName[100];
		swprintf_s(spriteName, L"Sprite\\%s_%d.sprite", wPrefix.c_str(), i);

		Ptr<ASprite> pSprite = AssetMgr::GetInst()->Load<ASprite>(spriteName, spriteName);
		pFlip->AddSprite(pSprite);
	}

	AssetMgr::GetInst()->AddAsset(flipName, pFlip.Get());

	wstring path = CONTENT_PATH + flipName;
	pFlip->Save(path);
}

void Menu::CreateGameObjectWindow()
{
	ImGui::Begin("Create GameObject", &m_bCreateGameObjectWindow);

	static char name[64] = "NewGameObject";
	static int layer = 0;

	ImGui::InputText("Name", name, 64);

	// --- 생성용 레이어를 현재 레벨 기준으로 동적 리스트 생성 ---
	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
	std::vector<std::string> layerNameStrs;
	if (pLevel)
	{
		int layerCount = pLevel->GetLayerCount();
		for (int i = 0; i < layerCount; ++i)
		{
			wstring wName = pLevel->GetLayer(i)->GetName();
			std::string strName = wName.empty() ? "None" : std::string(wName.begin(), wName.end());
			layerNameStrs.push_back(strName);
		}
	}

	std::vector<const char*> layerNamesCStr;
	for (auto& s : layerNameStrs)
		layerNamesCStr.push_back(s.c_str());

	if (!layerNamesCStr.empty())
		ImGui::Combo("Layer", &layer, layerNamesCStr.data(), layerNamesCStr.size());

	if (ImGui::Button("Create"))
	{
		CreateGameObject(name, layer);
	}

	ImGui::Separator();

	// --- 오브젝트 삭제 UI ---
	static int deleteLayer = 0;
	static int selectedObjectIndex = 0;

	if (!layerNamesCStr.empty())
		ImGui::Combo("Delete Layer", &deleteLayer, layerNamesCStr.data(), layerNamesCStr.size());

	if (pLevel)
	{
		auto& objects = pLevel->GetLayer(deleteLayer)->GetAllObjects();
		int objCount = (int)objects.size();

		static std::vector<std::string> objNameStrs;
		objNameStrs.clear();
		for (int i = 0; i < objCount; ++i)
			objNameStrs.push_back(std::string(objects[i]->GetName().begin(), objects[i]->GetName().end()));

		std::vector<const char*> objNamesCStr(objCount);
		for (int i = 0; i < objCount; ++i)
			objNamesCStr[i] = objNameStrs[i].c_str();

		if (objCount > 0)
		{
			ImGui::Combo("Object", &selectedObjectIndex, objNamesCStr.data(), objCount);

			if (ImGui::Button("Delete Object"))
			{
				DeleteGameObject(deleteLayer, selectedObjectIndex);
				selectedObjectIndex = 0;
			}
		}
		else
		{
			ImGui::Text("No objects in this layer.");
		}
	}

	ImGui::End();
}

void Menu::CreateGameObject(const char* name, int layer)
{
	Ptr<GameObject> obj = new GameObject;

	std::wstring wName(name, name + strlen(name));
	obj->SetName(wName);

	obj->AddComponent(new CTransform);

	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
	if (pLevel)
	{
		pLevel->AddObject(layer, obj);
	}
}

void Menu::DeleteGameObject(int layerIdx, int objIndex)
{
	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
	if (!pLevel) return;

	auto& objects = pLevel->GetObjects(layerIdx);
	if (objIndex < 0 || objIndex >= (int)objects.size()) return;

	Ptr<GameObject> objToDelete = objects[objIndex];

	pLevel->RemoveObject(objToDelete);
}

void Menu::CollisionWindow()
{
    ImGui::Begin("Layer Collision", &m_bCollisionWindow);

    Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
    if (!pLevel) { ImGui::End(); return; }

    int layerCount = (int)pLevel->GetLayerCount();
    
    // CollisionMatrix 크기 안전하게 맞추기
    while ((int)m_CollisionMatrix.size() < layerCount)
        AddLayerToCollisionMatrix();

    for (int i = 0; i < layerCount; ++i)
    {
        for (int j = i + 1; j < layerCount; ++j)
        {
            bool collide = m_CollisionMatrix[i][j];
            std::wstring label = pLevel->GetLayer(i)->GetName() + L" <-> " + pLevel->GetLayer(j)->GetName();
            if (ImGui::Checkbox(std::string(label.begin(), label.end()).c_str(), &collide))
            {
                m_CollisionMatrix[i][j] = collide;
                if (collide)
                    pLevel->CheckCollisionLayer(i, j);
                else
                    pLevel->UncheckCollisionLayer(i, j);
            }
        }
    }

    ImGui::End();
}

void Menu::AddLayerToCollisionMatrix()
{
	int newSize = (int)m_CollisionMatrix.size() + 1;
	m_CollisionMatrix.resize(newSize);
	for (auto& row : m_CollisionMatrix)
		row.resize(newSize, false); // 새 레이어 기본값 false
}

void Menu::RemoveLayerFromCollisionMatrix(int layerIdx)
{
	m_CollisionMatrix.erase(m_CollisionMatrix.begin() + layerIdx);
	for (auto& row : m_CollisionMatrix)
		row.erase(row.begin() + layerIdx);
}

void Menu::UpdateayerNameEditorWindow()
{
	if (!m_bLayerNameWindow)
		return;

	ImGui::Begin("Layer Name Editor", &m_bLayerNameWindow);

	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
	if (pLevel)
	{
		int layerCount = (int)pLevel->GetLayerCount();

		ImGui::Text("=== Layer List ===");
		ImGui::Separator();

		for (int i = 0; i < layerCount; ++i)
		{
			Layer* l = pLevel->GetLayer(i);
			if (!l) continue;

			std::wstring wLayerName = l->GetName();
			std::string layerNameStr(wLayerName.begin(), wLayerName.end());

			char buf[128];
			strcpy_s(buf, layerNameStr.c_str());

			if (ImGui::InputText(("Layer " + std::to_string(i)).c_str(), buf, IM_ARRAYSIZE(buf)))
			{
				std::wstring newName(buf, buf + strlen(buf));
				l->SetName(newName);
			}
		}
	}

	ImGui::End();
}

void Menu::CreateMaterialWindow()
{

	static char matName[64] = "NewMaterial";
	static char texName[64] = "TextureKey";

	ImGui::Begin("Create Material", &m_bCreateMaterialWindow);

	// 이름과 텍스처 입력
	ImGui::InputText("Material Name", matName, IM_ARRAYSIZE(matName));
	ImGui::InputText("Texture Key", texName, IM_ARRAYSIZE(texName));

	if (ImGui::Button("Create"))
	{
		CreateMaterial(matName, texName);
	}

	ImGui::End();
}

void Menu::CreateMaterial(const char* matName, const char* texName)
{
	if (!matName || strlen(matName) == 0)
		return;

	Ptr<AMaterial> pMtrl = new AMaterial;

	// 이름 설정
	std::wstring wMatName(matName, matName + strlen(matName));
	pMtrl->SetName(wMatName);

	// 기본 셰이더 설정 (예제에서는 Std2DShader)
	Ptr<AGraphicShader> pShader = AssetMgr::GetInst()->Find<AGraphicShader>(L"Std2DShader");
	pMtrl->SetShader(pShader);

	// 텍스처 설정
	std::wstring wTexName(texName, texName + strlen(texName));
	Ptr<ATexture> pTex = AssetMgr::GetInst()->Find<ATexture>(wTexName);
	if (pTex)
		pMtrl->SetTexture(TEX_0, pTex);

	// 기본 파라미터
	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));

	// 렌더링 도메인 설정
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);

	// 에셋 등록
	AssetMgr::GetInst()->AddAsset(wMatName, pMtrl.Get());

	// 저장
	std::wstring path = CONTENT_PATH + L"Material\\" + wMatName + L".mtrl";
	pMtrl->Save(path);
}

void Menu::CreateLevelWindow()
{
	// 팝업을 열어야 하는 상태인지 체크
	if (m_bCreateLevelWindow)
	{
		ImGui::OpenPopup("Create New Level");
		// m_bCreateLevelWindow 변수와 연동하여 창이 닫히면 false가 되도록 설정
		if (ImGui::BeginPopupModal("Create New Level", &m_bCreateLevelWindow, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static char levelName[64] = "NewLevel";
			ImGui::InputText("Name", levelName, IM_ARRAYSIZE(levelName));

			if (ImGui::Button("Create", ImVec2(120, 0)))
			{
				// 실제 로직 함수 호출
				CreateNewLevel(levelName);

				m_bCreateLevelWindow = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				m_bCreateLevelWindow = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

void Menu::CreateNewLevel(const char* _name)
{
	if (strlen(_name) == 0) return;

	Ptr<ALevel> pNewLevel = new ALevel;
	wstring wName = StrToWstr(_name);

	// 확장자가 없다면 붙여주는 것이 좋습니다.
	if (wName.find(L".lv") == wstring::npos)
		wName += L".lv";

	pNewLevel->SetName(wName);
	AssetMgr::GetInst()->AddAsset(wName, pNewLevel.Get());

	static wstring s_NewLevelName;
	s_NewLevelName = wName;

	TaskInfo info = {};
	info.Type = TASK_TYPE::CHANGE_LEVEL;
	info.Param_0 = (DWORD_PTR)s_NewLevelName.c_str();

	TaskMgr::GetInst()->AddTask(info);

	// 인스펙터 타겟 초기화
	Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
	if (pInspector) pInspector->SetTargetObject(nullptr);
}