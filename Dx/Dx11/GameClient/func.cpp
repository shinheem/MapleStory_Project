#include "pch.h"

#include "RenderMgr.h"
#include "TaskMgr.h"

void CreateObject(GameObject* _Object, int LayerIdx)
{
    TaskInfo info = {};

    info.Type = TASK_TYPE::CREATE_OBJECT;
    info.Param_0 = (DWORD_PTR)_Object;
    info.Param_1 = LayerIdx;

    TaskMgr::GetInst()->AddTask(info);
}


wchar_t Buff[255] = {};
void ChangeLevel(const wstring& _NextLevelName)
{
	TaskInfo info = {};

	wcscpy_s(Buff, 255, _NextLevelName.c_str());
	
	info.Type = TASK_TYPE::CHANGE_LEVEL;
	info.Param_0 = (DWORD_PTR)Buff;	

	TaskMgr::GetInst()->AddTask(info);
}

void ChangeLevelState(LEVEL_STATE _NextState)
{
	TaskInfo info = {};

	info.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	info.Param_0 = (DWORD_PTR)_NextState;

	TaskMgr::GetInst()->AddTask(info);
}

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Duration, bool _DepthTest)
{
    DbgInfo info = {};

    info.Shape = DBG_SHAPE::RECT;

    info.Pos = _Pos;
    info.Scale = _Scale;
    info.Rotation = _Rot;

    info.matWorld = XMMatrixIdentity();

    info.Color = _Color;
    info.Age = 0.f;
    info.Life = _Duration;

    info.DepthTest = _DepthTest;
    
    RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DepthTest)
{
    DbgInfo info = {};

    info.Shape = DBG_SHAPE::RECT;

    info.matWorld = _matWorld;

    info.Color = _Color;
    info.Age = 0.f;
    info.Life = _Duration;

    info.DepthTest = _DepthTest;

    RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Duration, bool _DepthTest)
{
    DbgInfo info = {};

    info.Shape = DBG_SHAPE::CIRCLE;
    info.Pos = _Pos;
    info.Scale = Vec3(_Radius * 2.f, _Radius * 2.f, 0.f);
    info.Rotation = Vec3(0.f, 0.f, 0.f);
    info.Color = _Color;
    info.Age = 0.f;
    info.Life = _Duration;
    info.DepthTest = _DepthTest;

    RenderMgr::GetInst()->AddDebugInfo(info);
}

void SaveWString(FILE* _File, const wstring& _String)
{   
    int Len = _String.length();
    fwrite(&Len, sizeof(int), 1, _File);        
    fwrite(_String.data(), sizeof(wchar_t), Len, _File);
}

wstring LoadWString(FILE* _File)
{
	int Len = 0;
	if (fread(&Len, sizeof(int), 1, _File) != 1) return L"";

	wstring str;
	str.resize(Len);

	// 2. 문자열 데이터를 str의 메모리 버퍼로 직접 읽기
	fread(&str[0], sizeof(wchar_t), Len, _File);

    return str;
}

void SaveAssetRef(FILE* _File, Asset* _Asset)
{
    // Asset 이 Null 인지 아닌지 저장
    bool IsNull = _Asset;
    fwrite(&IsNull, sizeof(bool), 1, _File);
     
    // Asset 의 Key, RelativePath 저장
    if (nullptr != _Asset)
    {
        SaveWString(_File, _Asset->GetKey());
        SaveWString(_File, _Asset->GetRelativePath());
    }
}





bool IsValid(Ptr<GameObject>& _Object)
{
    if (nullptr == _Object || _Object->IsDead())
    {
        _Object = nullptr;
        return false;
    }     
        
    return true;
}

float Saturate(float _Data)
{
    if (1.f < _Data)
        return 1.f;
    else if (_Data < 0.f)
        return 0.f;
    else
        return _Data;
}

#include "ALevel.h"
#include "GameObject.h"

#include "AssetMgr.h"
#include "Device.h"
#include "CollisionMgr.h"

#include "Source/Scripts/CPlayerScript.h"
#include "Source/Scripts/CCamMoveScript.h"
#include "Source/Scripts/CMonsterScript.h"

void CreateTestLevel()
{
	{
		Ptr<ALevel> pLevel = LOAD(ALevel, L"Level\\TestLevel.lv");		
		ChangeLevel(L"Level\\TestLevel.lv");
	}	

	return;

	//// Level 생성
	//Ptr<ALevel> pLevel = new ALevel;
	//pLevel->SetName(L"Current Level");

	//pLevel->GetLayer(0)->SetName(L"Default");
	//pLevel->GetLayer(1)->SetName(L"Background");
	//pLevel->GetLayer(2)->SetName(L"Tile");
	//pLevel->GetLayer(3)->SetName(L"Player");
	//pLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	//pLevel->GetLayer(5)->SetName(L"Enermy");
	//pLevel->GetLayer(6)->SetName(L"EnermyProjectile");

	//Ptr<GameObject> pObject = nullptr;

	//// 카메라 역할 오브젝트 
	//pObject = new GameObject;
	//pObject->SetName(L"MainCamera");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CCamera);
	//pObject->AddComponent(new CCamMoveScript);

	//pObject->Camera()->LayerCheckAll();
	////pObject->Camera()->LayerCheck(0); 
	////pObject->Camera()->LayerCheck(1); 
	////pObject->Camera()->LayerCheck(2);

	//pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pObject->Camera()->SetFar(10000.f);
	//pObject->Camera()->SetFOV(90.f);
	//pObject->Camera()->SetOrthoScale(1.f);
	//Vec2 vResolution = Device::GetInst()->GetRenderResolution();
	//pObject->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	//pObject->Camera()->SetWidth(vResolution.x);

	//pLevel->AddObject(0, pObject);

	//// 광원 추가
	//pObject = new GameObject;
	//pObject->SetName(L"Light 1");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CLight2D);

	//pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	//pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pObject->Light2D()->SetRadius(400.f);
	//pObject->Light2D()->SetAngle(XM_PI / 2.f);

	//pObject->Transform()->SetRelativePos(Vec3(-250.f, 0.f, 0.f));
	//pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 4.f));

	//pLevel->AddObject(0, pObject);

	////// 몬스터 생성
	////for (int i = 0; i < 5; ++i)
	////{
	////	Ptr<GameObject> pMonster = new GameObject;
	////	pMonster->SetName(L"Monster");

	////	pMonster->AddComponent(new CTransform);
	////	pMonster->AddComponent(new CMeshRender);
	////	pMonster->AddComponent(new CCollider2D);
	////	pMonster->AddComponent(new CMonsterScript);

	////	pMonster->Transform()->SetRelativePos(Vec3(300.f * (float)i, 0.f, 100.f));
	////	pMonster->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));

	////	pMonster->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	////	pMonster->MeshRender()->SetMaterial(FIND(AMaterial, L"MonsterMtrl"));

	////	pLevel->AddObject(5, pMonster);
	////}

	//Ptr<GameObject> pTop = new GameObject;
	//pTop->SetName(L"Ground");

	//pTop->AddComponent(new CTransform);
	//pTop->AddComponent(new CCollider2D);

	//pTop->Transform()->SetRelativePos(Vec3(0.f, -395.f, 0.f));
	//pTop->Transform()->SetRelativeScale(Vec3(1595.f, 128.f, 1.f));

	//pLevel->AddObject(2, pTop);

	//for (int i = 0; i < 4; ++i)
	//{
	//	Ptr<GameObject> pRope = new GameObject;
	//	pRope->SetName(L"Rope");

	//	pRope->AddComponent(new CTransform);
	//	//pRope->AddComponent(new CMeshRender);
	//	pRope->AddComponent(new CCollider2D);
	//	//pRope->AddComponent(new CMonsterScript);

	//	pRope->Transform()->SetRelativePos(Vec3(510.f, -189.f, 90.f));
	//	pRope->Transform()->SetRelativeScale(Vec3(6.f, 198.f, 0.f));

	//	//pRope->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	//	//pRope->MeshRender()->SetMaterial(FIND(AMaterial, L"MonsterMtrl"));

	//	pLevel->AddObject(1, pRope);
	//}

	//for (int i = 0; i < 11; ++i)
	//{
	//	// =====================
	//	// Top 발판
	//	// =====================
	//	Ptr<GameObject> pTop = new GameObject;
	//	pTop->SetName(L"FootBoard");

	//	pTop->AddComponent(new CTransform);
	//	pTop->AddComponent(new CCollider2D);

	//	pTop->Transform()->SetRelativePos(Vec3(510.f, -140.f, 0.f));
	//	pTop->Transform()->SetRelativeScale(Vec3(200.f, 0.f, 1.f));

	//	pLevel->AddObject(1, pTop);


	//	// =====================
	//	// Left 벽
	//	// =====================
	//	Ptr<GameObject> pLeft = new GameObject;
	//	pLeft->SetName(L"FootBoardLeft");

	//	pLeft->AddComponent(new CTransform);
	//	pLeft->AddComponent(new CCollider2D);

	//	pLeft->Transform()->SetRelativePos(Vec3(410.f, -190.f, 0.f));
	//	pLeft->Transform()->SetRelativeScale(Vec3(0.f, 100.f, 1.f));

	//	pLevel->AddObject(1, pLeft);


	//	// =====================
	//	// Right 벽
	//	// =====================
	//	Ptr<GameObject> pRight = new GameObject;
	//	pRight->SetName(L"FootBoardRight");

	//	pRight->AddComponent(new CTransform);
	//	pRight->AddComponent(new CCollider2D);

	//	pRight->Transform()->SetRelativePos(Vec3(610.f, -190.f, 0.f));
	//	pRight->Transform()->SetRelativeScale(Vec3(0.f, 100.f, 1.f));

	//	pLevel->AddObject(1, pRight);
	//}

	//// Boss_Left_Head_Stand 추가
	//Ptr<GameObject> pLeft_Head_Stand = new GameObject;
	//pLeft_Head_Stand->SetName(L"HornTail_Left_Head_Stand");
	//pLeft_Head_Stand->AddComponent(new CTransform);
	//pLeft_Head_Stand->AddComponent(new CFlipbookRender);
	//pLeft_Head_Stand->AddComponent(new CCollider2D);

	//pLeft_Head_Stand->Transform()->SetRelativePos(Vec3(-154.f, 94.f, 102.f));
	//pLeft_Head_Stand->Transform()->SetRelativeScale(Vec3(267.f, 236.f, 1.f));

	//pLeft_Head_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pLeft_Head_Stand->Collider2D()->SetScale(Vec2(1.f, 1.f));

	//pLeft_Head_Stand->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\HornTail_Left_Head_Stand.flip"));
	//pLeft_Head_Stand->FlipbookRender()->Play(0, 7.f, -1);

	//pLevel->AddObject(5, pLeft_Head_Stand);

	//// Boss_Middle_Head_Stand 추가
	//Ptr<GameObject> pMiddle_Head_Stand = new GameObject;
	//pMiddle_Head_Stand->SetName(L"HornTail_Middle_Head_Stand");
	//pMiddle_Head_Stand->AddComponent(new CTransform);
	//pMiddle_Head_Stand->AddComponent(new CFlipbookRender);
	//pMiddle_Head_Stand->AddComponent(new CCollider2D);

	//pMiddle_Head_Stand->Transform()->SetRelativePos(Vec3(-16.f, 188.f, 100.f));
	//pMiddle_Head_Stand->Transform()->SetRelativeScale(Vec3(197.f, 420.f, 1.f));

	//pMiddle_Head_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pMiddle_Head_Stand->Collider2D()->SetScale(Vec2(1.f, 1.f));

	//pMiddle_Head_Stand->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\HornTail_Middle_Head_Stand.flip"));
	//pMiddle_Head_Stand->FlipbookRender()->Play(0, 7.f, -1);

	//pLevel->AddObject(5, pMiddle_Head_Stand);

	//// Boss_Right_Head_Stand 추가
	//Ptr<GameObject> pRight_Head_Stand = new GameObject;
	//pRight_Head_Stand->SetName(L"HornTail_Right_Head_Stand");
	//pRight_Head_Stand->AddComponent(new CTransform);
	//pRight_Head_Stand->AddComponent(new CFlipbookRender);
	//pRight_Head_Stand->AddComponent(new CCollider2D);

	//pRight_Head_Stand->Transform()->SetRelativePos(Vec3(119.f, 94.f, 102.f));
	//pRight_Head_Stand->Transform()->SetRelativeScale(Vec3(267.f, 236.f, 1.f));

	//pRight_Head_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pRight_Head_Stand->Collider2D()->SetScale(Vec2(1.f, 1.f));

	//pRight_Head_Stand->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\HornTail_Right_Head_Stand.flip"));
	//pRight_Head_Stand->FlipbookRender()->Play(0, 7.f, -1);

	//pLevel->AddObject(5, pRight_Head_Stand);

	//// Boss_Left_Arm_Stand 추가
	//Ptr<GameObject> pLeft_Arm_Stand = new GameObject;
	//pLeft_Arm_Stand->SetName(L"HornTail_Left_Arm_Stand");
	//pLeft_Arm_Stand->AddComponent(new CTransform);
	//pLeft_Arm_Stand->AddComponent(new CFlipbookRender);
	//pLeft_Arm_Stand->AddComponent(new CCollider2D);

	//pLeft_Arm_Stand->Transform()->SetRelativePos(Vec3(-147.f, -65.f, 101.f));
	//pLeft_Arm_Stand->Transform()->SetRelativeScale(Vec3(275.f, 164.f, 1.f));

	//pLeft_Arm_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pLeft_Arm_Stand->Collider2D()->SetScale(Vec2(1.f, 1.f));

	//pLeft_Arm_Stand->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Left_Arm_Stand.flip"));
	//pLeft_Arm_Stand->FlipbookRender()->Play(0, 7.f, -1);

	//pLevel->AddObject(5, pLeft_Arm_Stand);

	//// Boss_Right_Arm_Stand 추가
	//Ptr<GameObject> pRight_Arm_Stand = new GameObject;
	//pRight_Arm_Stand->SetName(L"HornTail_Right_Arm_Stand");
	//pRight_Arm_Stand->AddComponent(new CTransform);
	//pRight_Arm_Stand->AddComponent(new CFlipbookRender);
	//pRight_Arm_Stand->AddComponent(new CCollider2D);

	//pRight_Arm_Stand->Transform()->SetRelativePos(Vec3(106.f, -65.f, 101.f));
	//pRight_Arm_Stand->Transform()->SetRelativeScale(Vec3(236.f, 160.f, 1.f));

	//pRight_Arm_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pRight_Arm_Stand->Collider2D()->SetScale(Vec2(1.f, 1.f));

	//pRight_Arm_Stand->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Right_Arm_Stand.flip"));
	//pRight_Arm_Stand->FlipbookRender()->Play(0, 7.f, -1);

	//pLevel->AddObject(5, pRight_Arm_Stand);

	//// Boss_Wing_Stand 추가
	//Ptr<GameObject> pWing_Stand = new GameObject;
	//pWing_Stand->SetName(L"HornTail_Wing_Stand");
	//pWing_Stand->AddComponent(new CTransform);
	//pWing_Stand->AddComponent(new CFlipbookRender);
	//pWing_Stand->AddComponent(new CCollider2D);

	//pWing_Stand->Transform()->SetRelativePos(Vec3(-25.f, -112.f, 104.f));
	//pWing_Stand->Transform()->SetRelativeScale(Vec3(694.f, 453.f, 1.f));

	//pWing_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pWing_Stand->Collider2D()->SetScale(Vec2(1.f, 1.f));

	//pWing_Stand->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Wing_Stand.flip"));
	//pWing_Stand->FlipbookRender()->Play(0, 7.f, -1);

	//pLevel->AddObject(5, pWing_Stand);

	//// Boss_Leg_Stand 추가
	//Ptr<GameObject> pLeg_Stand = new GameObject;
	//pLeg_Stand->SetName(L"HornTail_Leg_Stand");
	//pLeg_Stand->AddComponent(new CTransform);
	//pLeg_Stand->AddComponent(new CMeshRender);
	//pLeg_Stand->AddComponent(new CCollider2D);

	//pLeg_Stand->Transform()->SetRelativePos(Vec3(-22.f, -238.f, 100.f));
	//pLeg_Stand->Transform()->SetRelativeScale(Vec3(604.f, 197.f, 1.f));

	//pLeg_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pLeg_Stand->Collider2D()->SetScale(Vec2(0.25f, 0.5f));

	//pLeg_Stand->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	//pLeg_Stand->MeshRender()->SetMaterial(FIND(AMaterial, L"Leg_StandMtrl"));

	//pLevel->AddObject(5, pLeg_Stand);

	//// Boss_Tail_Stand 추가
	//Ptr<GameObject> pTail_Stand = new GameObject;
	//pTail_Stand->SetName(L"HornTail_Tail_Stand");
	//pTail_Stand->AddComponent(new CTransform);
	//pTail_Stand->AddComponent(new CMeshRender);
	//pTail_Stand->AddComponent(new CCollider2D);
	////pObject->AddComponent(new CPlayerScript);

	//pTail_Stand->Transform()->SetRelativePos(Vec3(243.f, -248.f, 100.f));
	//pTail_Stand->Transform()->SetRelativeScale(Vec3(419.f, 159.f, 1.f));

	//pTail_Stand->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pTail_Stand->Collider2D()->SetScale(Vec2(0.25f, 0.5f));

	//pTail_Stand->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	//pTail_Stand->MeshRender()->SetMaterial(FIND(AMaterial, L"Tail_StandMtrl"));

	//pLevel->AddObject(5, pTail_Stand);

	//// BackGround1 추가
	//Ptr<GameObject> Horn_Tail_BackGround1 = new GameObject;
	//Horn_Tail_BackGround1->SetName(L"HornTail_BackGround1");
	//Horn_Tail_BackGround1->AddComponent(new CTransform);
	//Horn_Tail_BackGround1->AddComponent(new CMeshRender);

	//Horn_Tail_BackGround1->Transform()->SetRelativePos(Vec3(0.f, 0.f, 109.f));
	//Horn_Tail_BackGround1->Transform()->SetRelativeScale(Vec3(1600.f, 900.f, 1.f));

	//Horn_Tail_BackGround1->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	//Horn_Tail_BackGround1->MeshRender()->SetMaterial(FIND(AMaterial, L"BackGround1Mtrl"));

	//pLevel->AddObject(5, Horn_Tail_BackGround1);

	//// BackGround2 추가
	//Ptr<GameObject> Horn_Tail_BackGround2 = new GameObject;
	//Horn_Tail_BackGround2->SetName(L"HornTail_BackGround2");
	//Horn_Tail_BackGround2->AddComponent(new CTransform);
	//Horn_Tail_BackGround2->AddComponent(new CMeshRender);

	//Horn_Tail_BackGround2->Transform()->SetRelativePos(Vec3(0.f, 0.f, 99.f));
	//Horn_Tail_BackGround2->Transform()->SetRelativeScale(Vec3(1600.f, 900.f, 1.f));

	//Horn_Tail_BackGround2->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	//Horn_Tail_BackGround2->MeshRender()->SetMaterial(FIND(AMaterial, L"BackGround2Mtrl"));

	//pLevel->AddObject(5, Horn_Tail_BackGround2);

	//// MaplePlayer Object 추가
	//Ptr<GameObject> MapleObject = new GameObject;
	//MapleObject->SetName(L"Maple_Player");
	//MapleObject->AddComponent(new CTransform);
	//MapleObject->AddComponent(new CFlipbookRender);
	//MapleObject->AddComponent(new CCollider2D);
	//MapleObject->AddComponent(new CPlayerScript);

	//MapleObject->Transform()->SetRelativePos(Vec3(580.f, -287.656f, 100.f));
	//MapleObject->Transform()->SetRelativeScale(Vec3(75.f, 85.f, 1.f));

	//MapleObject->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//MapleObject->Collider2D()->SetScale(Vec2(1.f, 1.f));

	//MapleObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Maple_Stand_Right1.flip"));	// 0
	//MapleObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Maple_Stand_Left1.flip"));	// 1
	//MapleObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Maple_Walk_Right1.flip"));	// 2
	//MapleObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Maple_Walk_Left1.flip"));	// 3
	//MapleObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Maple_Jump_Right.flip"));	// 4
	//MapleObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Maple_Jump_Left.flip"));		// 5
	//MapleObject->FlipbookRender()->Play(0, 4.f, -1);

	//// Player(부모 오브젝트) 를 레벨에 추가
	//pLevel->AddObject(3, MapleObject);

	////// Player Object 추가
	////pObject = new GameObject;
	////pObject->SetName(L"Player");
	////pObject->AddComponent(new CTransform);
	////pObject->AddComponent(new CFlipbookRender);
	////pObject->AddComponent(new CCollider2D);
	////pObject->AddComponent(new CPlayerScript);
	////pObject->AddComponent(new CMonsterScript);

	////pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	////pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	////pObject->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	////pObject->Collider2D()->SetScale(Vec2(0.25f, 0.5f));

	////pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Link_MoveDown.flip"));
	////pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Link_MoveLeft.flip"));
	////pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Link_MoveUp.flip"));
	////pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Link_MoveRight.flip"));
	////pObject->FlipbookRender()->Play(1, 15.f, 1);

	////Ptr<GameObject> pChild = new GameObject;
	////pChild->SetName(L"Child");
	////pChild->AddComponent(new CTransform);
	////pChild->AddComponent(new CMeshRender);
	////pChild->AddComponent(new CCollider2D);

	////pChild->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 0.f));
	////pChild->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	////pChild->Transform()->SetIndependentScale(true);

	////pChild->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"RectMesh"));
	////pChild->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"Std2DMtrl"));

	////// Player 와 Child 부모자식 연결
	////pObject->AddChild(pChild);

	////// Player(부모 오브젝트) 를 레벨에 추가
	////pLevel->AddObject(3, pObject);


	////// Tile Object
	////Ptr<GameObject> pTileObj = new GameObject;

	////pTileObj->AddComponent(new CTransform);
	////pTileObj->AddComponent(new CCollider2D);
	////pTileObj->AddComponent(new CTileRender);

	////pTileObj->Transform()->SetRelativePos(Vec3(-640.f, 640.f, 500.f));
	////pTileObj->TileRender()->SetTileMap(LOAD(ATileMap, L"TileMap\\TestTileMap.tile"));

	////pLevel->AddObject(2, pTileObj);


	//// 레벨 충돌 설정
	//pLevel->CheckCollisionLayer(1, 3);
	//pLevel->CheckCollisionLayer(2, 3);
	//pLevel->CheckCollisionLayer(3, 5);
	//pLevel->CheckCollisionLayer(4, 5);
	//pLevel->CheckCollisionLayer(3, 6);

	//// 레벨 변경점 체크
	//pLevel->SetChanged();

	//// 레벨을 AssetMgr 에 등록
	//AssetMgr::GetInst()->AddAsset(L"TestLevel", pLevel.Get());	

	//// 레벨을 파일로 저장
	//wstring ContentPath = CONTENT_PATH;
	//pLevel->Save(ContentPath + L"Level\\TestLevel.lv");
	//	
	//// 레벨을 변경
	//ChangeLevel(L"TestLevel");
}