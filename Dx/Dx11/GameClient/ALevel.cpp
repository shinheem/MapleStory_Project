#include "pch.h"
#include "ALevel.h"

#include "LevelMgr.h"
#include "SpawnMgr.h"


ALevel::ALevel()
	: Asset(ASSET_TYPE::LEVEL)
	, m_Matrix{}
	, m_Changed(false)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_LayerIdx = i;
	}
}

ALevel::~ALevel()
{
}

void ALevel::AddObject(int _LayerIdx, Ptr<GameObject> _Object)
{
	m_arrLayer[_LayerIdx].AddObject(_Object);
}

void ALevel::Deregister()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].DeregisterObject();
	}	
}


void ALevel::Begin()
{
	SpawnMgr::GetInst()->Clear();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Begin();
	}
}

void ALevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Tick();
	}
}

void ALevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].FinalTick();
	}
}


void ALevel::CheckCollisionLayer(UINT _LayerIdx1, UINT _LayerIdx2)
{
	UINT Row = _LayerIdx1;
	UINT Col = _LayerIdx2;

	// 더 작은 레이어 인덱스를 행 으로 사용한다.
	if (_LayerIdx2 < _LayerIdx1)
	{
		Row = _LayerIdx2;
		Col = _LayerIdx1;
	}

	m_Matrix[Row] ^= (1 << Col);
}

void ALevel::CheckCollisionLayer(const wstring& _LayerName1, const wstring& _LayerName2)
{

}

Ptr<GameObject> ALevel::FindObjectByName(const wstring& _Name)
{
	for (UINT layerIdx = 0; layerIdx < MAX_LAYER; ++layerIdx)
	{
		const vector<Ptr<GameObject>>& vecParents = m_arrLayer[layerIdx].GetParentObjects();

		for (size_t parentIdx = 0; parentIdx < vecParents.size(); ++parentIdx)
		{
			// ⭐ Dead 부모는 시작부터 제외
			if (vecParents[parentIdx] == nullptr || vecParents[parentIdx]->IsDead())
				continue;

			list<Ptr<GameObject>> queue;
			queue.push_back(vecParents[parentIdx]);

			while (!queue.empty())
			{
				Ptr<GameObject> pObject = queue.front();
				queue.pop_front();

				if (pObject == nullptr)
					continue;

				// ⭐ Dead 객체는 무시 (핵심)
				if (pObject->IsDead())
					continue;

				// ⭐ 이름 비교
				if (pObject->GetName() == _Name)
					return pObject;

				const vector<Ptr<GameObject>>& vecChild = pObject->GetChild();

				for (size_t j = 0; j < vecChild.size(); ++j)
				{
					// ⭐ 자식도 Dead이면 큐에 넣지 않음 (성능 + 안전)
					if (vecChild[j] == nullptr || vecChild[j]->IsDead())
						continue;

					queue.push_back(vecChild[j]);
				}
			}
		}
	}

	// 없다
	return nullptr;
}

vector<Ptr<GameObject>> ALevel::GetAllObjects()
{
	vector<Ptr<GameObject>> allObjects;

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		auto& layerObjects = m_arrLayer[i].GetAllObjects(); // Layer 클래스에 GetObjects() 필요
		allObjects.insert(allObjects.end(), layerObjects.begin(), layerObjects.end());
	}

	return allObjects;
}

int ALevel::Save(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	// 레벨 이름
	wstring LevelName = GetName();
	SaveWString(pFile, LevelName);

	// 충돌 체크 정보
	fwrite(m_Matrix, sizeof(UINT), MAX_LAYER, pFile);

	// 레이어 정보
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어의 이름 저장
		SaveWString(pFile, m_arrLayer[i].GetName());

		const vector<Ptr<GameObject>>& vecParents = m_arrLayer[i].GetParentObjects();

		// [추가] 실제로 저장할 오브젝트들만 따로 선별 (필터링)
		vector<Ptr<GameObject>> SaveTargetObjects;
		for (const auto& Object : vecParents)
		{
			// 파일에 저장하지 않을 제외 대상 이름들
			if (Object->GetName() == L"Maple_Player" ||
				Object->GetName() == L"MainCamera" ||
				Object->GetName() == L"GlobalUI"
				)
			{
				continue;
			}
			SaveTargetObjects.push_back(Object);
		}

		// [수정] 전체 개수가 아니라 필터링된 개수를 저장
		size_t parentCount = SaveTargetObjects.size();
		fwrite(&parentCount, sizeof(size_t), 1, pFile);

		// [수정] 선별된 오브젝트들만 저장 실행
		for (const auto& Object : SaveTargetObjects)
		{
			Object->SaveToLevelFile(pFile);
		}
	}

	fclose(pFile);

	return S_OK;
}

int ALevel::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 레벨 이름
	wstring LevelName = LoadWString(pFile);
	SetName(LevelName);
		
	// 충돌 체크 정보
	fread(m_Matrix, sizeof(UINT), MAX_LAYER, pFile);


	// 레이어 정보
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어의 이름
		wstring LayerName = LoadWString(pFile);
		m_arrLayer[i].SetName(LayerName);

		// 레이어 소속 최상위 부모 오브젝트를 계층구조로 불러온다		
		// 오브젝트 개수
		size_t parentCount = 0;
		fread(&parentCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < parentCount; ++j)
		{
			Ptr<GameObject> pObject = new GameObject;
			pObject->LoadFromLevelFile(pFile);
			AddObject(i, pObject);
		}
	}

	fclose(pFile);

	return S_OK;
}

// ALevel.cpp에 추가
Layer* ALevel::AddLayer()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		// 비어있는 레이어인지 확인 (예: 레이어가 비어있으면 Parent 오브젝트도 비어있음)
		if (m_arrLayer[i].GetAllObjects().empty())
		{
			m_arrLayer[i] = Layer();   // 새 Layer 초기화
			m_arrLayer[i].m_LayerIdx = i;
			SetChanged();              // 레벨 변경 플래그
			return &m_arrLayer[i];
		}
	}
	return nullptr; // 추가 가능한 슬롯 없음
}

void ALevel::RemoveLayer(int idx)
{
	if (idx < 0 || idx >= MAX_LAYER)
		return;

	// 레이어 안 모든 오브젝트 제거
	auto& objs = m_arrLayer[idx].GetAllObjects();
	for (auto& obj : objs)
	{
		RemoveObject(obj); // 기존 Level 함수 사용
	}

	// 레이어 초기화
	m_arrLayer[idx].DeregisterObject();
	m_arrLayer[idx].DeregisterAsParent(nullptr);

	SetChanged(); // 레벨 변경 플래그
}

int ALevel::GetLayerCount()
{
	return MAX_LAYER;
}

void ALevel::UncheckCollisionLayer(UINT _LayerIdx1, UINT _LayerIdx2)
{
    assert(_LayerIdx1 < MAX_LAYER && _LayerIdx2 < MAX_LAYER);

    // 비트 해제: XOR 대신 AND + NOT 사용
    m_Matrix[_LayerIdx1] &= ~(1 << _LayerIdx2);
    m_Matrix[_LayerIdx2] &= ~(1 << _LayerIdx1); // 양방향 해제
}

void ALevel::CopyLogicSettings(Ptr<ALevel> _pOther)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		// 1. 레이어 이름 복사
		this->GetLayer(i)->SetName(_pOther->GetLayer(i)->GetName());

		// 2. 충돌 매트릭스 복사
		// 내부적으로 m_LayerMasks[i] 등을 사용 중이라면 그대로 대입합니다.
		this->m_Matrix[i] = _pOther->m_Matrix[i];
	}
}