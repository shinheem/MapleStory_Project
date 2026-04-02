#include "pch.h"
#include "ItemMgr.h"
#include "AssetMgr.h"
#include "TaskMgr.h"


#include "Source\Scripts\CItemScript.h"
#include "Source\Scripts\CInventoryScript.h"

void CreateItemDrop(Ptr<tItemInfo> _pInfo, Vec3 _vPos)
{
    // 1. 오브젝트 생성 및 기본 컴포넌트 세팅
    GameObject* pItemObj = new GameObject;
    pItemObj->SetName(_pInfo->strName + L"_Drop");
    pItemObj->SetLayerIdx((int)LAYER_TYPE::Layer_Item);

    pItemObj->AddComponent(new CTransform);
    pItemObj->AddComponent(new CMeshRender);
    pItemObj->AddComponent(new CCollider2D);

    pItemObj->Transform()->SetRelativePos(_vPos);
    pItemObj->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 1.f));

    // 2. 재질 복제 및 적용 (중요!)
    // 원본 재질을 가져와서 Clone()으로 나만의 복사본을 만듭니다.
    Ptr<AMaterial> pOriginMtrl = (AMaterial*)AssetMgr::GetInst()->FindAsset(ASSET_TYPE::MATERIAL, (L"Std2DMtrl")).Get();
    if (nullptr == pOriginMtrl) return;
    AMaterial* pCloneMtrl = pOriginMtrl->Clone(); // 복사본 생성

    // 3. 텍스처(아이콘) 세팅
    // Material.h에 정의된 SetTexture(TEX_PARAM, Ptr<ATexture>)를 사용합니다.
    pCloneMtrl->SetTexture(TEX_PARAM::TEX_0, _pInfo->pIcon);

    // 4. 메쉬 및 재질 세팅
    pItemObj->MeshRender()->SetMesh((AMesh*)AssetMgr::GetInst()->FindAsset(ASSET_TYPE::MESH, L"RectMesh").Get());
    pItemObj->MeshRender()->SetMaterial(pCloneMtrl); // 복제된 재질을 오브젝트에 전달

    // 5. 스크립트 추가 및 데이터 연결
    CItemScript* pScript = new CItemScript;
    pScript->SetItemInfo(_pInfo);
    pItemObj->AddComponent(pScript);

    // 4. [DirectX 핵심] TaskMgr를 이용한 레이어 등록
    TaskInfo task = {};
    task.Type = TASK_TYPE::CREATE_OBJECT; // 엔진에 정의된 생성 태스크
    task.Param_0 = (DWORD_PTR)pItemObj;              
    task.Param_1 = (DWORD_PTR)LAYER_TYPE::Layer_Item;

    TaskMgr::GetInst()->AddTask(task);
}