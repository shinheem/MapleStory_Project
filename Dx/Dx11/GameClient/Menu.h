#pragma once
#include "EditorUI.h"

class Menu :
    public EditorUI
{
private:
    bool m_bAddTextureWindow;
    bool m_bCreateSpriteWindow;
    bool m_bCreateFlipbookWindow;
    bool m_bCreateGameObjectWindow;
    bool m_bCollisionWindow;
    bool m_bLayerNameWindow;
    bool m_bCreateMaterialWindow;
    bool m_bCreateLevelWindow;

    vector<vector<bool>> m_CollisionMatrix;

private:
    void File();
    void Level();
    void View();
    void GameObjectMenu();
    void Asset();

public:
    void CreateSpriteWindow();
    void CreateSpriteFromTexture(const char* texKey, float sliceX, float sliceY, int count);
    void CreateFlipbookWindow();
    void CreateGameObjectWindow();
    void CreateMaterialWindow();
    void CreateMaterial(const char* matName, const char* texName);
    void CreateGameObject(const char* name, int layer);
    void DeleteGameObject(int layerIdx, int objIndex);
    void CreateFlipbook(const char* prefix, int count);
    void CollisionWindow();
    void UpdateayerNameEditorWindow();
    void AddLayerToCollisionMatrix();
    void RemoveLayerFromCollisionMatrix(int layerIdx);
    void CreateLevelWindow(); // UI 그리기 함수
    void CreateNewLevel(const char* _name); // 실제 로직 함수

    wstring StrToWstr(const string& str)
    {
        if (str.empty()) return wstring();

        // 필요한 버퍼 크기 계산
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);

        wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

        return wstrTo;
    }

    virtual void Tick() override;
    virtual void Tick_UI() override;

private:
    wstring GetAssetName(ASSET_TYPE _Type, const wstring& _Name);

public:
    Menu();
    virtual ~Menu();
};

