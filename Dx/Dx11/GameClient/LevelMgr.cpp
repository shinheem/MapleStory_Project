#include "pch.h"
#include "LevelMgr.h"
#include "AssetMgr.h"
#include "KeyMgr.h"

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
	// 1. 첫 레벨 로드
	Ptr<ALevel> pStartLevel = AssetMgr::GetInst()->Load<ALevel>(L"Level\\Hontail_Boss_Level.lv", L"Level\\Hontail_Boss_Level.lv");
	m_CurLevel = m_SharedLevel = pStartLevel;

	Ptr<APrefab> pUIPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\GlobalUI.pref", L"Prefab\\GlobalUI.pref");
	if (pUIPrefab != nullptr)
	{
		GameObject* pUI = pUIPrefab->Instantiate();
		pUI->SetName(L"GlobalUI");
		// UI 전용 레이어(예: 31번)에 추가
		m_CurLevel->AddObject(31, pUI);
	}

	// 2. 플레이어 프리팹 소환
	Ptr<APrefab> pPlayerPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\Maple_Player.pref", L"Prefab\\Maple_Player.pref");
	Ptr<GameObject> pPlayer = nullptr;
	if (pPlayerPrefab != nullptr)
	{
		pPlayer = pPlayerPrefab->Instantiate();
		pPlayer->SetName(L"Maple_Player");
		m_CurLevel->AddObject(3, pPlayer);
	}

	// 3. [핵심] PlayerStart 위치로 플레이어 이동
	if (pPlayer != nullptr)
	{
		Ptr<GameObject> pStartObj = m_CurLevel->FindObjectByName(L"PlayerStart");
		if (pStartObj != nullptr)
		{
			// PlayerStart의 월드/상대 좌표를 가져와서 플레이어에게 세팅
			Vec3 vStartPos = pStartObj->Transform()->GetRelativePos();
			pPlayer->Transform()->SetRelativePos(vStartPos);
		}
	}

	// 4. 카메라 소환 및 타겟팅
	Ptr<APrefab> pCameraPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\MainCamera.pref", L"Prefab\\MainCamera.pref");
	if (pCameraPrefab != nullptr)
	{
		GameObject* pCamera = pCameraPrefab->Instantiate();
		pCamera->SetName(L"MainCamera");
		m_CurLevel->AddObject(0, pCamera);

		// 카메라가 플레이어를 즉시 따라가도록 설정
		// pCamera->GetCamera()->SetTarget(pPlayer);
	}

	m_LevelState = LEVEL_STATE::PLAY;
	m_CurLevel->Begin();
}

void LevelMgr::Progress()
{

	// 테스트용: 1번 키 입력 시 다른 레벨로 이동
	// (KeyMgr 클래스가 있다고 가정할 때의 예시입니다)
	//if (KEY_TAP(KEY::X))
	//{
	//	Ptr<ALevel> pLevelAsset = AssetMgr::GetInst()->Load<ALevel>(L"Level\\Hontail_Entrance_Level.lv", L"Level\\Hontail_Entrance_Level.lv");
	//	if (pLevelAsset != nullptr)
	//	{
	//		// 원본 에셋을 그대로 쓰지 않고 Clone()하여 새 인스턴스를 만듭니다.
	//		Ptr<ALevel> pNextLevel = pLevelAsset->Clone();
	//		EnterLevel(pNextLevel);
	//	}
	//}

	//if (KEY_TAP(KEY::C))
	//{
	//	Ptr<ALevel> pLevelAsset = AssetMgr::GetInst()->Load<ALevel>(L"Level\\Dungeon_Level.lv", L"Level\\Dungeon_Level.lv");
	//	if (pLevelAsset != nullptr)
	//	{
	//		Ptr<ALevel> pNextLevel = pLevelAsset->Clone();
	//		EnterLevel(pNextLevel);
	//	}
	//}

	//if (KEY_TAP(KEY::W))
	//{
	//	Ptr<ALevel> pLevelAsset = AssetMgr::GetInst()->Load<ALevel>(L"Level\\Hontail_Left_Level.lv", L"Level\\Hontail_Left_Level.lv");
	//	if (pLevelAsset != nullptr)
	//	{
	//		// 원본 에셋을 그대로 쓰지 않고 Clone()하여 새 인스턴스를 만듭니다.
	//		Ptr<ALevel> pNextLevel = pLevelAsset->Clone();
	//		EnterLevel(pNextLevel);
	//	}
	//}

	//if (KEY_TAP(KEY::Q))
	//{
	//	Ptr<ALevel> pLevelAsset = AssetMgr::GetInst()->Load<ALevel>(L"Level\\Hontail_Right_Level.lv", L"Level\\Hontail_Right_Level.lv");
	//	if (pLevelAsset != nullptr)
	//	{
	//		Ptr<ALevel> pNextLevel = pLevelAsset->Clone();
	//		EnterLevel(pNextLevel);
	//	}
	//}

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
	if (nullptr == _NextLevel || m_CurLevel == _NextLevel) return;

	// 1. 현재 레벨에서 이사 보낼 중요 객체들을 미리 확보
	Ptr<ALevel> pPrevLevel = m_CurLevel;
	Ptr<GameObject> pPlayer = pPrevLevel->FindObjectByName(L"Maple_Player");
	Ptr<GameObject> pUI = pPrevLevel->FindObjectByName(L"GlobalUI");
	Ptr<GameObject> pCamera = pPrevLevel->FindObjectByName(L"MainCamera");

	// 2. [핵심] 이동할 레벨(_NextLevel)에 이미 동일한 이름의 객체가 있는지 체크 후 제거
	// AssetMgr에 로드된 원본 레벨을 재사용할 때, 이전에 방문하며 추가됐던 객체들을 청소합니다.
	Ptr<GameObject> pOldPlayer = _NextLevel->FindObjectByName(L"Maple_Player");
	if (pOldPlayer != nullptr) pOldPlayer->Destroy();

	Ptr<GameObject> pOldUI = _NextLevel->FindObjectByName(L"GlobalUI");
	if (pOldUI != nullptr) pOldUI->Destroy();

	Ptr<GameObject> pOldCamera = _NextLevel->FindObjectByName(L"MainCamera");
	if (pOldCamera != nullptr) pOldCamera->Destroy();

	// 3. 기존 레벨의 레이어 이름 및 충돌 매트릭스 설정을 새 레벨로 복사
	_NextLevel->CopyLogicSettings(pPrevLevel);

	// 4. 레벨 교체 (이제 m_CurLevel은 새 레벨을 가리킴)
	m_CurLevel = m_SharedLevel = _NextLevel;

	// 5. 새 레벨에 객체들을 등록 (기존에 쓰던 객체를 그대로 유지)
	if (pPlayer != nullptr) m_CurLevel->AddObject(3, pPlayer);
	if (pUI != nullptr)     m_CurLevel->AddObject(31, pUI);
	if (pCamera != nullptr) m_CurLevel->AddObject(0, pCamera);

	// 6. 새 레벨의 시작 위치(PlayerStart)로 플레이어 및 카메라 좌표 이동
	Ptr<GameObject> pStartObj = m_CurLevel->FindObjectByName(L"PlayerStart");
	if (pPlayer != nullptr && pStartObj != nullptr)
	{
		Vec3 vStartPos = pStartObj->Transform()->GetRelativePos();
		pPlayer->Transform()->SetRelativePos(vStartPos);

		// 카메라도 플레이어 위치로 즉시 이동
		if (pCamera != nullptr)
			pCamera->Transform()->SetRelativePos(vStartPos);
	}

	// 7. 상태 설정 및 시작
	m_LevelState = LEVEL_STATE::PLAY;

	// 레벨의 첫 시작(Begin) 호출
	m_CurLevel->Begin();

	// 레벨이 변경되었음을 엔진에 알림
	m_CurLevel->SetChanged();
}