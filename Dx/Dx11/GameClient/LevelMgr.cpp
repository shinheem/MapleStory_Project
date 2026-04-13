#include "pch.h"
#include "LevelMgr.h"
#include "AssetMgr.h"
#include "KeyMgr.h"
#include "RenderMgr.h"

#include "Source/Scripts/CCamMoveScript.h"
#include "CollisionMgr.h"

LevelMgr::LevelMgr()
	: m_LevelState(LEVEL_STATE::STOP)
{
}

LevelMgr::~LevelMgr()
{
}

void LevelMgr::Init()
{
	Ptr<ALevel> pStartLevel = AssetMgr::GetInst()->Load<ALevel>(
		L"Level\\Dungeon_Level.lv",
		L"Level\\Dungeon_Level.lv");

	m_CurLevel = m_SharedLevel = pStartLevel;

	// ⭐ UI 생성 (1회만)
	Ptr<APrefab> pUIPrefab = AssetMgr::GetInst()->Load<APrefab>(
		L"Prefab\\GlobalUI.pref",
		L"Prefab\\GlobalUI.pref");

	if (pUIPrefab != nullptr)
	{
		m_UI = pUIPrefab->Instantiate();
		m_UI->SetName(L"GlobalUI");
	}

	// ⭐ Player 생성 (1회만)
	Ptr<APrefab> pPlayerPrefab = AssetMgr::GetInst()->Load<APrefab>(
		L"Prefab\\Maple_Player.pref",
		L"Prefab\\Maple_Player.pref");

	if (pPlayerPrefab != nullptr)
	{
		m_Player = pPlayerPrefab->Instantiate();
		m_Player->SetName(L"Maple_Player");
	}

	// ⭐ Camera 생성 (1회만)
	Ptr<APrefab> pCameraPrefab = AssetMgr::GetInst()->Load<APrefab>(
		L"Prefab\\MainCamera.pref",
		L"Prefab\\MainCamera.pref");

	if (pCameraPrefab != nullptr)
	{
		m_Camera = pCameraPrefab->Instantiate();
		m_Camera->SetName(L"MainCamera");
	}

	// ⭐ 현재 레벨에 추가 (한 번만)
	if (m_Player) m_CurLevel->AddObject(3, m_Player);
	if (m_UI)     m_CurLevel->AddObject(31, m_UI);
	if (m_Camera) m_CurLevel->AddObject(0, m_Camera);

	// 시작 위치
	if (m_Player != nullptr)
	{
		Ptr<GameObject> pStartObj = m_CurLevel->FindObjectByName(L"PlayerStart");
		if (pStartObj != nullptr)
		{
			Vec3 vStartPos = pStartObj->Transform()->GetRelativePos();
			m_Player->Transform()->SetRelativePos(vStartPos);
		}
	}

	m_LevelState = LEVEL_STATE::STOP;
}

void LevelMgr::Progress()
{

	// 실행할 레벨이 지정된게 없으면 리턴
	if (nullptr == m_CurLevel)
		return;

	// 이전에 등록된 모든 오브젝트들 제거
	m_CurLevel->Deregister();

	// 레벨안에 있는 오브젝트들이 이번 DT 동안 할 일 수행
	// 레벨의 상태가 Play 일 때만 Level 의 Tick 을 수행
	if (m_LevelState == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}

	m_CurLevel->FinalTick();

	// 충돌 검사 진행
	if (m_LevelState == LEVEL_STATE::PLAY)	
		CollisionMgr::GetInst()->Progress(m_CurLevel);
}

Ptr<GameObject> LevelMgr::FindObjectByName(const wstring& _name)
{
	return m_CurLevel->FindObjectByName(_name);
}

void LevelMgr::ChangeLevel(Ptr<ALevel> _NextLevel)
{
	m_CurLevel = m_SharedLevel = _NextLevel;
	m_LevelState = LEVEL_STATE::STOP;
	_NextLevel->SetChanged();
}

void LevelMgr::ChangeLevelState(LEVEL_STATE _NextState)
{
	if (m_LevelState == _NextState)
		return;

	// Stop -> Play
	if (m_LevelState == LEVEL_STATE::STOP
		&& _NextState == LEVEL_STATE::PLAY)
	{
		// 원본 에셋 레벨의 복제본 레벨을 만들어서 현재 레벨로 가리킨다.
		m_CurLevel = m_SharedLevel->Clone();
		m_CurLevel->SetChanged();
		m_CurLevel->Begin();

		if (m_pCurBGM == nullptr) {
			m_pCurBGM = LOAD(ASound, L"Sound\\DunGeon.mp3");
		}

		if (m_pCurBGM != nullptr) {
			m_pCurBGM->Play(0, 0.5f, false);
		}
	}
	
	else if (  (m_LevelState == LEVEL_STATE::PLAY || m_LevelState == LEVEL_STATE::PAUSE)
			  && _NextState == LEVEL_STATE::STOP)
	{
		m_CurLevel = m_SharedLevel;
		m_CurLevel->SetChanged();
	}

	m_LevelState = _NextState;
}

void LevelMgr::EnterLevel(Ptr<ALevel> _NextLevel)
{
	if (_NextLevel == nullptr || m_CurLevel == _NextLevel)
		return;

	// 1️⃣ 기존 레벨에서 UI와 Player를 안전하게 제거 (메모리 해제가 아님!)
	// 이 과정이 없으면 이전 레벨이 Delete될 때 UI도 같이 사라질 수 있습니다.
	if (m_CurLevel.Get())
	{
		// 엔진에 해당 객체를 레벨의 관리 대상에서 제외하는 기능이 있다면 호출
		// 예: m_CurLevel->RemoveObject(m_UI); 
	}

	// 2️⃣ 레벨 교체
	m_CurLevel = m_SharedLevel = _NextLevel;
	m_LevelState = LEVEL_STATE::PLAY;

	// 3️⃣ UI 데이터 보존을 위해 '이미 존재하는지' 확인 후 등록
	// 새로 생성(Instantiate)하는 것이 아니라, Init에서 만든 m_UI를 그대로 재사용합니다.
	if (m_UI)
	{
		// 이미 새 레벨에 같은 이름의 UI가 있다면 중복 생성 방지를 위해 체크
		if (m_CurLevel->FindObjectByName(L"GlobalUI") == nullptr)
		{
			m_CurLevel->AddObject(31, m_UI);
		}
	}

	if (m_Player)
	{
		if (m_CurLevel->FindObjectByName(L"Maple_Player") == nullptr)
		{
			m_CurLevel->AddObject(3, m_Player);
		}
	}

	// 4️⃣ PlayerStart 위치 적용
	Ptr<GameObject> pStartObj = m_CurLevel->FindObjectByName(L"PlayerStart");
	if (m_Player && pStartObj)
	{
		Vec3 vStartPos = pStartObj->Transform()->GetRelativePos();
		m_Player->Transform()->SetRelativePos(vStartPos);
	}

	// 5️⃣ 레벨 시작 및 카메라 등록
	m_CurLevel->Begin();
	if (m_Camera)
	{
		m_CurLevel->AddObject(0, m_Camera);
		if (m_Camera->Camera())
			RenderMgr::GetInst()->RegisterMainCamera(m_Camera->Camera());
	}

	wstring strLevelPath = _NextLevel->GetKey();
	wstring strTargetBGM = L"";

	if (strLevelPath.find(L"Dungeon_Level") != wstring::npos)
	{
		strTargetBGM = L"Sound\\DunGeon.mp3";
	}
	else if (strLevelPath.find(L"Hontail_Entrance_Level") != wstring::npos ||
		strLevelPath.find(L"Hontail_Left_Level") != wstring::npos ||
		strLevelPath.find(L"Hontail_Right_Level") != wstring::npos)
	{
		strTargetBGM = L"Sound\\Hontail_Entrance.mp3";
	}
	else if (strLevelPath.find(L"Hontail_Boss_Level") != wstring::npos)
	{
		strTargetBGM = L"Sound\\HonTale.mp3";
	}

	if (!strTargetBGM.empty())
	{
		if (m_pCurBGM == nullptr || m_pCurBGM->GetKey() != strTargetBGM)
		{
			if (m_pCurBGM != nullptr)
			{
				m_pCurBGM->Stop();
			}

			m_pCurBGM = LOAD(ASound, strTargetBGM);

			if (m_pCurBGM != nullptr)
			{
				m_pCurBGM->Play(0, 0.5f, false);
			}
		}
	}

	m_CurLevel->SetChanged();
}