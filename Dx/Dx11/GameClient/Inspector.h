#pragma once
#include "EditorUI.h"

#include "GameObject.h"
#include "ComponentUI.h"
#include "ScriptUI.h"
#include "AssetUI.h"

class Inspector :
    public EditorUI
{
private:
    Ptr<CScript>            m_pScript;
    Ptr<GameObject>         m_TargetObject;
    Ptr<ComponentUI>        m_arrComUI[(UINT)COMPONENT_TYPE::END];
    vector<Ptr<ScriptUI>>   m_vecScriptUI;

    Ptr<Asset>              m_TargetAsset;
    Ptr<AssetUI>            m_arrAssetUI[(UINT)ASSET_TYPE::END];

    vector<string> m_vecLevelNames; // 폴더 내 .lv 파일 이름들


    int            m_SelectedLevelIdx = -1; // 리스트에서 선택된 인덱스

public:
    virtual void Tick_UI() override;

public:
    Ptr<GameObject> GetTargetObject() { return m_TargetObject; }
    void SetTargetObject(Ptr<GameObject> _Object);
    void AddComponents();
    void AddScriptUI();

    void ChangeObjectLayer(Ptr<GameObject> obj, int newLayer);
    void DrawLayerSelectorUI(Ptr<GameObject> targetObject);
    Ptr<Asset> GetTargetAsset() { return m_TargetAsset; }
    void SetTargetAsset(Ptr<Asset> _Asset);
    void RefreshLevelList(); // 파일 목록 갱신 함수
    void UpdatePrefabButtons();
    void CreatePrefabFromTarget();

    wstring StrToWstr(const string& str)
    {
        if (str.empty()) return wstring();

        // 필요한 버퍼 크기 계산
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);

        wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

        return wstrTo;
    }

private:
    void CreateChildUI();

public:
    Inspector();
    virtual ~Inspector();
};


