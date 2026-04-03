#include "pch.h"
#include "CPlayerScript.h"
#include "CItemScript.h"
#include "CMissileScript.h"
#include "CInventoryScript.h"

#include "../../KeyMgr.h"
#include "../../TimeMgr.h"
#include "../../RenderMgr.h"
#include "../../ItemMgr.h"
#include "../../AssetMgr.h"
#include "../../LevelMgr.h"
#include "../../TaskMgr.h"

#include "CTransform.h"
#include "GameObject.h"
#include "CCollider2D.h"



CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
	, m_Target(nullptr)
	, m_Tex(nullptr)
	, m_Missile(nullptr)
	, m_Jump_Count(0)
	, m_Limit(0.f)
	, m_Speed(150)
	, m_bWalk(false)
	, m_fJumpVelocity(0.f)
	, m_DropTimer(0.f)
	, m_fGravity(-980.f)
	, m_prevPos()
	, m_bJump(false)
	, m_bOnRope(false)
	, m_bOnPlatform(false)
	, m_bGround(false)
	, m_bDropDown(false)
{		
}

CPlayerScript::~CPlayerScript()
{
}

// 초기화
// AddComponent
void CPlayerScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"PlayerSpeed", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Limit, L"Lmit", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Missile, L"Missile", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, &m_Tex, L"ChangeTex");	
}

// 레벨이 시작할때
void CPlayerScript::Begin()
{
	Ptr<CCollider2D> col = GetOwner()->Collider2D();

	col->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::OnBeginOverlap);
	col->AddDynamicOverlap(this, (COLLISION_EVENT)&CPlayerScript::OnOverlap);
	col->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CPlayerScript::OnEndOverlap);
}

void CPlayerScript::Tick()
{
	/* 테스트용 아이템 생성 (F1 ~ F5) */
	if (KEY_TAP(KEY::F1))
	{
		// Ptr이 스마트 포인터라면 아래와 같이 할당하거나 
		// 엔진의 별도 생성 함수가 있다면 그것을 사용하세요.
		Ptr<tItemInfo> pMyItem = new tItemInfo;
		pMyItem->strName = L"Test_Equip";
		pMyItem->eType = ITEM_TYPE::EQUIP;
		// 경로가 올바른지 확인 필요 (Material 폴더가 Asset 하위에 있는지)
		pMyItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Test.mtrl", L"Material\\Test.mtrl");
		pMyItem->iCount = 1;

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		ItemMgr::GetInst()->CreateItemDrop(pMyItem, vPos);
	}
	else if (KEY_TAP(KEY::F2))
	{
		Ptr<tItemInfo> pMyItem = new tItemInfo;
		pMyItem->strName = L"Test_Consum";
		pMyItem->eType = ITEM_TYPE::CONSUME;
		pMyItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Test.mtrl", L"Material\\Test.mtrl");
		pMyItem->iCount = 1;

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		ItemMgr::GetInst()->CreateItemDrop(pMyItem, vPos);
	}
	else if (KEY_TAP(KEY::F3))
	{
		Ptr<tItemInfo> pMyItem = new tItemInfo;
		pMyItem->strName = L"Test_Etc";
		pMyItem->eType = ITEM_TYPE::ETC;
		pMyItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Test.mtrl", L"Material\\Test.mtrl");
		pMyItem->iCount = 1;

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		ItemMgr::GetInst()->CreateItemDrop(pMyItem, vPos);
	}
	else if (KEY_TAP(KEY::F4))
	{
		Ptr<tItemInfo> pMyItem = new tItemInfo;
		pMyItem->strName = L"Intall_Etc";
		pMyItem->eType = ITEM_TYPE::INSTALL;
		pMyItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Test.mtrl", L"Material\\Test.mtrl");
		pMyItem->iCount = 1;

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		ItemMgr::GetInst()->CreateItemDrop(pMyItem, vPos);
	}
	else if (KEY_TAP(KEY::F5))
	{
		Ptr<tItemInfo> pMyItem = new tItemInfo;
		pMyItem->strName = L"Cash_Etc";
		pMyItem->eType = ITEM_TYPE::CASH;
		pMyItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Test.mtrl", L"Material\\Test.mtrl");
		pMyItem->iCount = 1;

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		ItemMgr::GetInst()->CreateItemDrop(pMyItem, vPos);
	}
	/* 테스트용 아이템 생성 (F1 ~ F5) */

	m_bGround = false;
	Change_State();
	Shoot();
	ItemCollect();
}

void CPlayerScript::Change_State()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec3 vRotation = Transform()->GetRelativeRot();
	
	m_prevPos = vPos; // 이전 프레임 발판 저장

	/*if (KEY_PRESSED(KEY::UP))
		vPos.y += m_Speed * DT;
	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= m_Speed * DT;*/

	bool bMoveLeft = false;
	bool bMoveRight = false;

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += m_Speed * DT;
		bMoveRight = true;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= m_Speed * DT;
		bMoveLeft = true;
	}

	if (!m_bJump && KEY_PRESSED(KEY::DOWN) && KEY_TAP(KEY::ALT))
	{
		m_bDropDown = true;  
		m_DropTimer = 0.25f;   // 발판 무시 시간

		m_bJump = true;
		m_fJumpVelocity = 100.f; // 아래로 떨어지게
	}
	else if (KEY_TAP(KEY::ALT) && !m_bJump)
	{
		m_bJump = true;
		m_fJumpVelocity = 400.f;
	}

	if (m_bDropDown) // 아래점프
	{
		m_DropTimer -= DT;

		if (m_DropTimer <= 0.f)
		{
			m_bDropDown = false;
		}
	}

	// 로프에 매달려 있을 때 방향키를 누르고 점프를 하면 그쪽 방향으로 점프
	if (m_bOnRope)
	{
		if (KEY_TAP(KEY::ALT) && KEY_PRESSED(KEY::RIGHT))
		{
			m_bJump = true;
			m_bOnRope = false;
			m_fJumpVelocity = 400.f; // 점프 힘
		} 
		else if (KEY_TAP(KEY::ALT) && KEY_PRESSED(KEY::LEFT))
		{
			m_bJump = true;
			m_bOnRope = false;
			m_fJumpVelocity = 400.f; // 점프 힘
		}
	}

	//// ===== 중력 적용 =====
	if (!m_bGround && !m_bOnRope)
	{
		m_fJumpVelocity += m_fGravity * DT;
		vPos.y += m_fJumpVelocity * DT;

		if(KEY_PRESSED(KEY::UP) && KEY_TAP(KEY::ALT) && m_Jump_Count < 1)
		{
			++m_Jump_Count;
			m_bJump = true;
			m_fJumpVelocity = 400.f; // 점프 힘
		}

		if (KEY_PRESSED(KEY::RIGHT))
			GetOwner()->FlipbookRender()->Play(4, 0.f, 1);

		if (KEY_PRESSED(KEY::LEFT))
			GetOwner()->FlipbookRender()->Play(5, 0.f, 1);
		
		if (!bMoveLeft && !bMoveRight && !m_bWalk && KEY_RELEASED(KEY::RIGHT))
			GetOwner()->FlipbookRender()->Play(4, 0.f, 1);
		
		if (!bMoveLeft && !bMoveRight && !m_bWalk && KEY_RELEASED(KEY::LEFT))
			GetOwner()->FlipbookRender()->Play(5, 0.f, 1);

	}
	else if (!m_bJump)
	{
		if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::LEFT))
		{
			bMoveLeft = false;
			bMoveRight = false;
		}

		// 왼쪽으로 가다가 양쪽 방향키 눌렀을 시 왼쪽으로 서있는 모션
		if (!bMoveLeft && !bMoveRight && m_bWalk && KEY_PRESSED(KEY::LEFT))
		{
			GetOwner()->FlipbookRender()->Play(1, 2.f, -1);
			m_bWalk = false;
		} 
		// 오른쪽으로 가다가 양쪽 방향키 눌렀을 시 왼쪽으로 서있는 모션
		else if (!bMoveLeft && !bMoveRight && m_bWalk && KEY_PRESSED(KEY::RIGHT))
		{
			GetOwner()->FlipbookRender()->Play(0, 2.f, -1);
			m_bWalk = false;
		}

		if (!bMoveLeft && !bMoveRight && m_bWalk && KEY_RELEASED(KEY::LEFT)) // 왼쪽으로 서있는 모션
		{
			GetOwner()->FlipbookRender()->Play(1, 2.f, -1);
			m_bWalk = false;
		}
		else if (!bMoveLeft && !bMoveRight && m_bWalk && KEY_RELEASED(KEY::RIGHT)) // 오른쪽으로 서있는 모션
		{
			GetOwner()->FlipbookRender()->Play(0, 2.f, -1);
			m_bWalk = false;
		}

		if (bMoveRight && !m_bWalk) // 오른쪽으로 걷는 모션
		{
			GetOwner()->FlipbookRender()->Play(2, 8.f, -1);
			m_bWalk = true;
		}
		else if (bMoveLeft && !m_bWalk) // 왼쪽으로 걷는 모션
		{
			GetOwner()->FlipbookRender()->Play(3, 8.f, -1);
			m_bWalk = true;
		} 
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeScale(vScale);
	Transform()->SetRelativeRot(vRotation);
}
void CPlayerScript::Shoot()
{
	if (KEY_TAP(KEY::SPACE))
	{
		Vec3 vMyPos = Transform()->GetRelativePos();
		Vec3 vMyScale = Transform()->GetRelativeScale();
		Vec3 vRotation = Transform()->GetRelativeRot();
		Vec3 vUp = Transform()->GetDir(DIR::UP);

		Instantiate(m_Missile.Get(), 4, vMyPos + vMyScale * 0.5f * vUp);

		//// 미사일 역할의 오브젝트 생성
		//GameObject* pObject = new GameObject;
		//pObject->SetName(L"Missile");

		//pObject->AddComponent(new CTransform);
		//pObject->AddComponent(new CMeshRender);
		//pObject->AddComponent(new CCollider2D);

		//Ptr<CMissileScript> pMissileScript = new CMissileScript;
		//pMissileScript->SetTarget(m_Target);
		//pObject->AddComponent(pMissileScript.Get());	

		//pObject->Transform()->SetRelativePos(vMyPos + vMyScale * 0.5f * vUp);
		//pObject->Transform()->SetRelativeScale(Vec3(10.f, 30.f, 1.f));
		//pObject->Transform()->SetRelativeRot(vRotation);

		//pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"RectMesh"));
		//pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"Std2DMtrl"));

		//CreateObject(pObject, 4);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		/*DrawDebugRect(Transform()->GetRelativePos()
					, Transform()->GetRelativeScale() * Vec3(2.f, 2.f, 2.f)
					, Vec3(0.f, 0.f, 0.f), Vec4(0.f, 1.f, 0.f, 1.f), 1.f);*/

		//DrawDebugRect(Transform()->GetRelativePos(), 100.f, Vec4(1.f, 0.f, 0.f, 1.f), 2.f);
		//DrawDebugRect(Transform()->GetWorldMat(), Vec4(1.f, 0.f, 0.f, 1.f), 2.f);
	}	
}

void CPlayerScript::OnBeginOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol) 
{
}

void CPlayerScript::OnOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol)
{
	GameObject* Other = _OtherCol->GetOwner();

	Vec3 vPos = Transform()->GetRelativePos();
		
	Vec3 otherPos = Other->Transform()->GetRelativePos();
	Vec3 otherScale = Other->Transform()->GetRelativeScale();

	Vec3 myScale = Transform()->GetRelativeScale();

	// ===== 발판 착지 =====
	if ((Other->GetName() == L"FootBoard" && !m_bDropDown && Other->GetLayerIdx() == 1) || (Other->GetName() == L"Ground" && Other->GetLayerIdx() == 2))
	{
		if (m_bOnRope)
			return;

		float prevBottom = m_prevPos.y - myScale.y * 0.5f;
		float boardTop = otherPos.y + otherScale.y * 0.5f;
		float playerBottom = vPos.y - myScale.y * 0.5f;
		float epsilon = 0.05f * myScale.y;

		if (prevBottom >= boardTop - epsilon && playerBottom <= boardTop && m_fJumpVelocity <= 0.f)
		{
			vPos.y = boardTop + myScale.y * 0.5f;
			m_bGround = true;
			m_fJumpVelocity = 0.f;
			m_Jump_Count = 0;
			m_bJump = false;
		}

	}
	// ===== 발판 착지 =====

	// ===== Rope 처리 =====
	if (Other->GetName() == L"Rope")
	{
		if (!m_bOnRope)
		{
			if (KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN))
			{
				m_bOnRope = true;
				m_bJump = false;
				m_bGround = false;
				m_fJumpVelocity = 0.f;
				m_Jump_Count = 0;
				vPos.x = otherPos.x;
			}
		}

		if (m_bOnRope)
		{
			vPos.x = otherPos.x;

			if (KEY_PRESSED(KEY::UP))
				vPos.y += m_Speed * DT;

			if (KEY_PRESSED(KEY::DOWN))
				vPos.y -= m_Speed * DT;
		}
	}
	Transform()->SetRelativePos(vPos);
	// ===== Rope 처리 =====
}

void CPlayerScript::ItemCollect()
{
	if (KEY_TAP(KEY::Z))
	{
		Layer* pItemLayer = LevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Layer_Item);
		const vector<Ptr<GameObject>>& vecItems = pItemLayer->GetParentObjects();

		Vec3 vPlayerPos = Transform()->GetRelativePos();
		CItemScript* pClosestScript = nullptr;
		float fMinDist = 60.f;

		for (size_t i = 0; i < vecItems.size(); ++i)
		{
			if (vecItems[i]->IsDead()) continue;

			CItemScript* pScript = vecItems[i]->GetScript<CItemScript>().Get();

			// [중요] 이미 습득 중(m_bGet == true)인 아이템은 아예 무시
			if (nullptr == pScript || pScript->IsGetting())
				continue;

			float fDist = Vec3::Distance(vPlayerPos, vecItems[i]->Transform()->GetRelativePos());

			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pClosestScript = pScript;
			}
		}

		if (nullptr != pClosestScript)
		{
			// 가장 가까운 '아직 안 주운' 아이템에게만 명령
			pClosestScript->GetItem(GetOwner());
		}
	}
}

void CPlayerScript::OnEndOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol)
{
	m_bOnRope = false;
}


void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Limit, sizeof(float), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Limit, sizeof(float), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
}
