#include "pch.h"
#include "SpawnMgr.h"
#include "LevelMgr.h"
#include "AssetMgr.h"

#include "ALevel.h"
#include "APrefab.h"

#include "Source\Scripts\CMonsterSpawner.h"


SpawnMgr::SpawnMgr()
{

}

SpawnMgr::~SpawnMgr()
{

}

// 스포너를 생성하여 특정 위치에 배치하는 핵심 함수
//void SpawnMgr::CreateSpawner(const wstring& _strName, const wstring& _strPrefabPath, Vec3 _vPos)
//{
//    // 1. 스포너 오브젝트 생성
//    GameObject* pSpawnerObj = new GameObject;
//    pSpawnerObj->SetName(_strName);
//    pSpawnerObj->AddComponent(new CTransform);
//
//    // 2. 스포너 스크립트 추가 및 프리팹 설정
//    CMonsterSpawner* pSpawnerScript = new CMonsterSpawner;
//
//    Ptr<APrefab> pPrefab = AssetMgr::GetInst()->Load<APrefab>(_strPrefabPath, _strPrefabPath);
//    pSpawnerScript->SetMonsterPrefab(pPrefab);
//
//    pSpawnerObj->AddComponent(pSpawnerScript);
//
//    // 3. 위치 설정
//    pSpawnerObj->Transform()->SetRelativePos(_vPos);
//
//    // 4. 현재 레벨에 추가 (스포너 자체는 로직만 돌리므로 DEFAULT 레이어)
//    ALevel* pCurLevel = LevelMgr::GetInst()->GetCurLevel().Get();
//    pCurLevel->AddObject((UINT)LAYER_TYPE::Layer_Enemy, pSpawnerObj);
//}