#include "pch.h"
#include "ItemMgr.h"
#include "AssetMgr.h"
#include "TaskMgr.h"

#include "Source\Scripts\CItemScript.h"

ItemMgr::ItemMgr()
{

}

ItemMgr::~ItemMgr()
{

}


void ItemMgr::CreateItemDrop(Ptr<tItemInfo> _pInfo, Vec3 _vPos)
{
    // 1. 드랍될 아이템의 '몸체' 생성
    GameObject* pItemObj = new GameObject;
    pItemObj->SetName(_pInfo->strName + L"_Drop"); // 넣어주신 정보의 이름을 사용

    int iLayerIdx = (int)LAYER_TYPE::Layer_Item;
    pItemObj->SetLayerIdx(iLayerIdx);

    pItemObj->AddComponent(new CTransform);
    pItemObj->AddComponent(new CMeshRender);
    pItemObj->AddComponent(new CCollider2D);

    pItemObj->Transform()->SetRelativePos(_vPos);
    pItemObj->Transform()->SetRelativeScale(Vec3(42.f, 42.f, 1.f));

    if (nullptr != _pInfo->pIcon)
    {
        Ptr<AMaterial> pItemMtrl = _pInfo->pIcon->Clone();

        pItemObj->MeshRender()->SetMesh((AMesh*)AssetMgr::GetInst()->FindAsset(ASSET_TYPE::MESH, L"RectMesh").Get());
        pItemObj->MeshRender()->SetMaterial(pItemMtrl);
    }

    // 4. 스크립트에 '넘겨받은 정보'를 그대로 연결
    CItemScript* pScript = new CItemScript;

    // 사용자가 던져준 _pInfo를 스크립트가 들고 있게 합니다.
    pScript->SetItemInfo(_pInfo);

    pItemObj->AddComponent(pScript);

    // 5. 엔진에 생성 요청
    TaskInfo task = {};
    task.Type = TASK_TYPE::CREATE_OBJECT;
    task.Param_0 = (DWORD_PTR)pItemObj;
    task.Param_1 = (DWORD_PTR)iLayerIdx;

    TaskMgr::GetInst()->AddTask(task);
}