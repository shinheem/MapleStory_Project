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

    vector<vector<bool>> m_CollisionMatrix;

private:
    void File();
    void Level();
    void View();
    void GameObjectMenu();
    void Asset();

public:
    void CreateSpriteWindow();
    void CreateSpriteFromTexture(const char* texKey, int sliceX, int sliceY, int count);
    void CreateFlipbookWindow();
    void CreateGameObjectWindow();
    void CreateGameObject(const char* name, int layer);
    void DeleteGameObject(int layerIdx, int objIndex);
    void CreateFlipbook(const char* prefix, int count);
    void CollisionWindow();
    void UpdateayerNameEditorWindow();
    void AddLayerToCollisionMatrix();
    void RemoveLayerFromCollisionMatrix(int layerIdx);


    virtual void Tick() override;
    virtual void Tick_UI() override;

private:
    wstring GetAssetName(ASSET_TYPE _Type, const wstring& _Name);

public:
    Menu();
    virtual ~Menu();
};

