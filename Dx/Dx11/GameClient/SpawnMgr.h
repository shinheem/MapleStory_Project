#pragma once

class CMonsterSpawner;

class SpawnMgr
	: public singleton<SpawnMgr>
{
	SINGLE(SpawnMgr)

private:
	// 현재 레벨에 존재하는 모든 스포너들을 관리
	vector<CMonsterSpawner*> m_vecSpawners;

public:
    // 미리 배치된 스포너들이 Begin()에서 스스로를 등록함
    void RegisterSpawner(CMonsterSpawner* _pSpawner) {
        m_vecSpawners.push_back(_pSpawner);
    }

    void Clear() { m_vecSpawners.clear(); }

    //void CreateSpawner(const wstring& _strName, const wstring& _strPrefabPath, Vec3 _vPos);
};

