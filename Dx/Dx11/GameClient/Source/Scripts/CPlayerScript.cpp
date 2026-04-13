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
	, m_fAccTime(0.f)
	, m_fHitTimer(0.f)
	, m_vDeadPos()
	, m_fSoulAngle(0.f)
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
	// 1. 플레이어 본체 콜라이더 델리게이트 등록
	Ptr<CCollider2D> col = GetOwner()->Collider2D();
	if (col != nullptr && !m_bCollisionRegistered)
	{
		col->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::OnBeginOverlap);
		col->AddDynamicOverlap(this, (COLLISION_EVENT)&CPlayerScript::OnOverlap);
		col->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CPlayerScript::OnEndOverlap);
		m_bCollisionRegistered = true;
	}

	// 2. 초기 스탯 설정
	m_iMaxHP = 100; m_iCurHP = 100;
	m_iMaxMP = 50;  m_iCurMP = 50;
	m_iMaxEXP = 200; m_iCurEXP = 0;

	// 3. UI 바 스크립트 캐싱
	GameObject* pObj = nullptr;

	if (pObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"HpBar").Get())
		m_pHpBar = pObj->GetScript<CGaugeBarScript>();

	if (pObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MpBar").Get())
		m_pMpBar = pObj->GetScript<CGaugeBarScript>();

	if (pObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"EXP_Bar").Get())
		m_pExpBar = pObj->GetScript<CGaugeBarScript>();

	// 초기 UI 동기화
	if (m_pHpBar != nullptr) m_pHpBar->SetValue((float)m_iCurHP, (float)m_iMaxHP);
	if (m_pMpBar != nullptr) m_pMpBar->SetValue((float)m_iCurMP, (float)m_iMaxMP);
	if (m_pExpBar != nullptr) m_pExpBar->SetValue((float)m_iCurEXP, (float)m_iMaxEXP);

	// 4. [수정] 평소에는 공격 오브젝트가 아예 없도록 설정
	m_pAttackObj = nullptr;
}

void CPlayerScript::Tick()
{
	// ===== 1. 사망 상태 처리 (최우선 순위) =====
	if (m_bDead)
	{
		// 8번 유령 애니메이션 재생 및 유지
		PlayAnim(8, 1.f, false);

		// [공전 로직] 사망 시점의 위치(m_vDeadPos)를 기준으로 본체 위치를 회전시킴
		m_fSoulAngle += DT * 3.f; // 회전 속도 (숫자가 클수록 빠름)

		float fRadius = 40.f; // 공전 반지름 (캐릭터가 그리는 원의 크기)
		Vec3 vPos = m_vDeadPos; // 기준점으로부터 계산 시작

		// X, Y 좌표를 삼각함수로 계산하여 원운동 구현
		vPos.x += cosf(m_fSoulAngle) * fRadius;
		vPos.y += sinf(m_fSoulAngle) * fRadius;

		// 계산된 위치를 트랜스폼에 적용
		Transform()->SetRelativePos(vPos);


		// 사망 상태에서 엔터 키를 누르면 부활 로직 실행
		if (KEY_TAP(KEY::ENTER))
		{
			m_bDead = false;
			m_fSoulAngle = 0.f;

			// 리스폰 위치로 이동 (PlayerStart 오브젝트 찾기)
			Vec3 vRespawnPos = Vec3(0.f, 0.f, 0.f);
			GameObject* pStartObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"PlayerStart").Get();
			if (pStartObj != nullptr)
			{
				vRespawnPos = pStartObj->Transform()->GetRelativePos();
			}
			Transform()->SetRelativePos(vRespawnPos);

			// 상태 데이터 복구
			m_iCurHP = m_iMaxHP;
			m_iCurMP = m_iMaxMP;
			m_fJumpVelocity = 0.f;
			m_bJump = false;
			m_bGround = false;
			m_bOnRope = false;
			m_Jump_Count = 0;

			// UI 갱신
			if (m_pHpBar != nullptr) m_pHpBar->SetValue((float)m_iCurHP, (float)m_iMaxHP);
			if (m_pMpBar != nullptr) m_pMpBar->SetValue((float)m_iCurMP, (float)m_iMaxMP);

			// 부활 직후 짧은 무적 시간 부여
			m_bInvincible = true;
			m_fInvincibleTime = 1.5f;

			printf("플레이어가 부활했습니다.\n");
		}

		// 사망 상태일 때는 아래의 모든 이동/물리 로직을 무시하고 리턴
		return;
	}


	// ===== 2. 낙사 처리 (Y 좌표 체크) =====
	if (Transform()->GetRelativePos().y <= -1200.f)
	{
		TakeDamage(m_iMaxHP);
	}


	// ===== 3. 디버그 및 소모품 처리 (DEL 키) =====
	if (KEY_TAP(KEY::DEL))
	{
		m_iCurHP += 50;
		if (m_iCurHP > m_iMaxHP) m_iCurHP = m_iMaxHP;
		if (m_pHpBar != nullptr) m_pHpBar->SetValue((float)m_iCurHP, (float)m_iMaxHP);

		m_iCurMP += 50;
		if (m_iCurMP > m_iMaxMP) m_iCurMP = m_iMaxMP;
		if (m_pMpBar != nullptr) m_pMpBar->SetValue((float)m_iCurMP, (float)m_iMaxMP);
	}


	// ===== 4. 무적 시간 카운트다운 =====
	if (m_bInvincible)
	{
		m_fInvincibleTime -= DT;
		if (m_fInvincibleTime <= 0.f)
		{
			m_bInvincible = false;
			m_fInvincibleTime = 0.f;
		}
	}


	// ===== 5. 플레이어 행동 로직 호출 (살아있을 때만) =====
	Change_State();  // 이동, 점프, 애니메이션 결정
	Shoot();         // 미사일 발사
	ItemCollect();   // 아이템 습득

	m_bGround = false;
}

void CPlayerScript::PlayAnim(int _iIdx, float _fSpeed, bool _bLoop)
{
	if (GetOwner()->FlipbookRender() == nullptr) return;

	// 현재 재생 중인 인덱스와 같으면 Play()를 다시 호출하지 않음 (이게 없으면 멈춰 보임)
	if (GetOwner()->FlipbookRender()->GetCurFlipbookIdx() == _iIdx)
		return;

	GetOwner()->FlipbookRender()->Play(_iIdx, _fSpeed, _bLoop ? -1 : 1);
}

void CPlayerScript::Change_State()
{
	// 1. 현재 데이터 백업
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vScale = Transform()->GetRelativeScale();
	m_prevPos = vPos;

	bool bMoveLeft = false;
	bool bMoveRight = false;

	// 목표 스케일 값 (현재 값으로 초기화)
	float fTargetScaleX = vScale.x;

	// 2. 공격 상태 처리
	if (m_bIsAttacking)
	{
		Update_Attack();
	}

	// 3. 피격 타이머 관리
	if (m_fHitTimer > 0.f)
	{
		m_fHitTimer -= DT;
	}

	// 4. 스킬 공격 입력 (LSHIFT)
	if (KEY_TAP(KEY::LSHIFT))
	{
		if (!m_bIsAttacking && m_iCurMP >= 10)
		{
			UseMana(10);
			m_bIsAttacking = true;
			m_fAccTime = 0.f;

			int iAttackMotion = (rand() % 2 == 0) ? 6 : 7;
			GetOwner()->FlipbookRender()->Play(iAttackMotion, 5.f, 1);

			Ptr<APrefab> pAttackPrefab = AssetMgr::GetInst()->Load<APrefab>(L"Prefab\\Player_Attack.pref", L"Prefab\\Player_Attack.pref");
			if (pAttackPrefab != nullptr)
			{
				m_pAttackObj = pAttackPrefab->Instantiate();
				GetOwner()->AddChild(m_pAttackObj);
				m_pAttackObj->SetLayerIdx((UINT)LAYER_TYPE::Layer_PlayerAttack);

				bool bLookRight = (vScale.x < 0.f);
				float fOffset = 160.f;
				Vec3 vEffectPos = Vec3(bLookRight ? fOffset : -fOffset, -40.f, -1.f);
				m_pAttackObj->Transform()->SetRelativePos(vEffectPos);

				Vec3 vFixedScale = Vec3(420.f, 300.f, 1.f);
				if (bLookRight) vFixedScale.x *= -1.f;
				m_pAttackObj->Transform()->SetRelativeScale(vFixedScale);

				if (m_pAttackObj->Collider2D() != nullptr)
					m_pAttackObj->Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::OnBeginOverlap);

				if (m_pAttackObj->FlipbookRender() != nullptr)
					m_pAttackObj->FlipbookRender()->Play(0, 20.f, 1);
			}
		}
	}

	// 5. 이동 및 물리 연산 (공격 중이 아닐 때만)
	if (!m_bIsAttacking)
	{
		if (KEY_PRESSED(KEY::RIGHT))
		{
			vPos.x += m_Speed * DT;
			bMoveRight = true;
			fTargetScaleX = -abs(vScale.x); // 오른쪽 보기
		}
		if (KEY_PRESSED(KEY::LEFT))
		{
			vPos.x -= m_Speed * DT;
			bMoveLeft = true;
			fTargetScaleX = abs(vScale.x);  // 왼쪽 보기
		}

		// 하단 점프
		if (!m_bJump && KEY_PRESSED(KEY::DOWN) && KEY_TAP(KEY::ALT))
		{
			m_bDropDown = true;
			m_DropTimer = 0.25f;
			m_bJump = true;
			m_fJumpVelocity = 100.f;
			m_bGround = false;
		}
		// 일반 점프
		else if (KEY_TAP(KEY::ALT) && !m_bJump)
		{
			m_bJump = true;
			m_fJumpVelocity = 400.f;
			m_bGround = false;
		}
	}

	// 로프/플랫폼 타이머 처리
	if (m_bDropDown) { m_DropTimer -= DT; if (m_DropTimer <= 0.f) m_bDropDown = false; }
	if (m_bOnRope)
	{
		if (KEY_TAP(KEY::ALT) && (KEY_PRESSED(KEY::RIGHT) || KEY_PRESSED(KEY::LEFT)))
		{
			m_bJump = true;
			m_bOnRope = false;
			m_fJumpVelocity = 400.f;
		}
	}

	// 6. 애니메이션 및 중력 최종 결정
	if (!m_bGround && !m_bOnRope)
	{
		m_fJumpVelocity += m_fGravity * DT;
		vPos.y += m_fJumpVelocity * DT;

		if (!m_bIsAttacking && KEY_PRESSED(KEY::UP) && KEY_TAP(KEY::ALT) && m_Jump_Count < 1)
		{
			++m_Jump_Count;
			m_bJump = true;
			m_fJumpVelocity = 400.f;
		}

		if (!m_bIsAttacking)
			PlayAnim(3, 1.f, true);
	}
	else if (!m_bJump)
	{
		if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::LEFT)) { bMoveLeft = false; bMoveRight = false; }

		if (!m_bIsAttacking)
		{
			if (m_bOnRope)
				PlayAnim(2, 5.f, true);
			else if (KEY_PRESSED(KEY::DOWN))
				PlayAnim(4, 1.f, true);
			else if (bMoveLeft || bMoveRight)
			{
				PlayAnim(1, 8.f, true);
				m_bWalk = true;
			}
			else
			{
				if (m_fHitTimer > 0.f) PlayAnim(5, 5.f, true);
				else PlayAnim(0, 2.f, true);
				m_bWalk = false;
			}
		}
	}

	// 스케일 방향이 바뀌었을 때만 업데이트
	if (vScale.x != fTargetScaleX)
	{
		vScale.x = fTargetScaleX;
		Transform()->SetRelativeScale(vScale);
	}

	// 최종 위치값 적용
	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::Update_Attack()
{
	m_fAccTime += DT;

	// 0.8초 후 공격 종료
	if (m_fAccTime > 0.8f)
	{
		m_bIsAttacking = false;

		if (m_pAttackObj != nullptr)
		{
			// [핵심] 오브젝트를 파괴하여 아예 없앰
			m_pAttackObj->Destroy();
			m_pAttackObj = nullptr;
		}
	}
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
	// 죽은 상태라면 몬스터에 의한 위치 밀림(넉백)이나 지형 보정을 모두 무시
	if (m_bDead) return;

	GameObject* pOtherObj = _OtherCol->GetOwner();
	UINT otherLayer = pOtherObj->GetLayerIdx();

	// 1. 충돌한 '나(_MyCol)'의 주인이 플레이어 본체(GetOwner())일 때만 피격 판정
	if (_MyCol->GetOwner() == GetOwner())
	{
		// 상대방 레이어가 몬스터의 공격 판정 레이어인 경우
		if (otherLayer == (UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack)
		{
			// [핵심 수정] 무적 상태가 아닐 때만 데미지와 넉백을 처리합니다.
			if (!m_bInvincible)
			{
				// TakeDamage 내부에서 m_bInvincible을 true로 바꾸고 시간을 설정합니다.
				TakeDamage(10);

				// 넉백(Knockback) 처리: 데미지를 입는 순간에만 실행됨
				Vec3 vPlayerPos = Transform()->GetRelativePos();
				Vec3 vMonsterPos = pOtherObj->Transform()->GetWorldPos();

				if (vMonsterPos.x < vPlayerPos.x)
					vPlayerPos.x += 30.f; // 몬스터가 왼쪽이면 오른쪽으로 밀려남
				else
					vPlayerPos.x -= 30.f; // 몬스터가 오른쪽이면 왼쪽으로 밀려남

				Transform()->SetRelativePos(vPlayerPos);
			}
		}
	}

	// 2. 만약 충돌한 '나(_MyCol)'가 플레이어의 공격 이펙트(m_pAttackObj)라면?
	else if (m_pAttackObj != nullptr && _MyCol->GetOwner() == m_pAttackObj)
	{
		// 공격 이펙트와 상대방(몬스터 등)이 충돌했을 때의 처리가 필요하다면 여기에 작성
		// 예: 몬스터에게 데미지를 입히는 함수 호출 등
	}
}

void CPlayerScript::OnOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol)
{
	// 죽은 상태라면 몬스터에 의한 위치 밀림(넉백)이나 지형 보정을 모두 무시
	if (m_bDead) return;

	// [안전 장치] 충돌 주체가 플레이어 본체가 아니라면(예: 공격 이펙트 등) 물리 로직 무시
	if (_MyCol->GetOwner() != GetOwner())
		return;

	GameObject* Other = _OtherCol->GetOwner();
	UINT otherLayer = Other->GetLayerIdx();

	Vec3 vPos = Transform()->GetRelativePos(); // 현재 프레임 위치
	Vec3 vMyScale = Transform()->GetRelativeScale();

	Vec3 otherPos = Other->Transform()->GetRelativePos();
	Vec3 otherScale = Other->Transform()->GetRelativeScale();


	// ===== 1. 지속 피격 판정 (몬스터와 닿아 있는 동안) =====
	// 무적 시간이 끝났는데 여전히 몬스터 공격 레이어 안에 있다면 다시 데미지를 입힘
	if (otherLayer == (UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack)
	{
		if (!m_bInvincible)
		{
			// 데미지 입힘 (무적 시간 0.7초 및 5번 자세 타이머 5초가 여기서 갱신됨)
			TakeDamage(10);

			// 넉백(Knockback) 처리
			Vec3 vPlayerPos = Transform()->GetRelativePos();
			Vec3 vMonsterPos = Other->Transform()->GetWorldPos();

			if (vMonsterPos.x < vPlayerPos.x)
				vPlayerPos.x += 30.f; // 몬스터가 왼쪽이면 오른쪽으로 밀려남
			else
				vPlayerPos.x -= 30.f; // 몬스터가 오른쪽이면 왼쪽으로 밀려남

			Transform()->SetRelativePos(vPlayerPos);

			// 피격 직후에는 지형 처리를 건너뛰고 싶다면 return;을 사용할 수 있지만, 
			// 넉백 도중 벽에 걸려야 하므로 보통은 계속 진행합니다.
		}
	}


	// ===== 2. Wall(벽) 충돌 처리 =====
	if (Other->GetName() == L"Wall" && Other->GetLayerIdx() == (UINT)LAYER_TYPE::Layer_Tile)
	{
		// 겹친 정도 계산 (AABB 충돌 깊이)
		float fOverlapX = (vMyScale.x * 0.5f + otherScale.x * 0.5f) - abs(vPos.x - otherPos.x);
		float fOverlapY = (vMyScale.y * 0.5f + otherScale.y * 0.5f) - abs(vPos.y - otherPos.y);

		if (fOverlapX < fOverlapY)
		{
			if (vPos.x < otherPos.x) vPos.x -= fOverlapX; // 왼쪽으로 밀기
			else vPos.x += fOverlapX;                     // 오른쪽으로 밀기
		}
		else
		{
			if (vPos.y < otherPos.y) // 천장 충돌
			{
				vPos.y -= fOverlapY;
				if (m_fJumpVelocity > 0.f) m_fJumpVelocity = 0.f;
			}
		}
		// 보정된 값을 즉시 반영하고 함수 종료 (벽인 경우 다른 지형 연산 방지)
		Transform()->SetRelativePos(vPos);
		return;
	}


	// ===== 3. 발판 착지 처리 (FootBoard, Ground) =====
	if ((Other->GetName() == L"FootBoard" && !m_bDropDown && Other->GetLayerIdx() == 1) ||
		(Other->GetName() == L"Ground" && Other->GetLayerIdx() == 2))
	{
		// 로프 타는 중에는 발판 착지 무시
		if (m_bOnRope) return;

		float prevBottom = m_prevPos.y - vMyScale.y * 0.5f;
		float boardTop = otherPos.y + otherScale.y * 0.5f;
		float playerBottom = vPos.y - vMyScale.y * 0.5f;
		float epsilon = 0.05f * vMyScale.y;

		// 위에서 아래로 떨어지는 중일 때만 착지 판정
		if (prevBottom >= boardTop - epsilon && playerBottom <= boardTop && m_fJumpVelocity <= 0.f)
		{
			vPos.y = boardTop + vMyScale.y * 0.5f;
			m_bGround = true;
			m_fJumpVelocity = 0.f;
			m_Jump_Count = 0;
			m_bJump = false;
			Transform()->SetRelativePos(vPos);
		}
	}


	// ===== 4. Rope(로프) 처리 =====
	if (Other->GetName() == L"Rope")
	{
		// 로프에 매달리기 시도 (위/아래 키 입력 시)
		if (!m_bOnRope && (KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN)))
		{
			m_bOnRope = true;
			m_bJump = false;
			m_bGround = false;
			m_fJumpVelocity = 0.f;
			m_Jump_Count = 0;
			vPos.x = otherPos.x; // 로프 중앙으로 위치 고정
		}

		// 로프 이동 로직
		if (m_bOnRope)
		{
			vPos.x = otherPos.x;
			if (KEY_PRESSED(KEY::UP)) vPos.y += m_Speed * DT;
			if (KEY_PRESSED(KEY::DOWN)) vPos.y -= m_Speed * DT;
			Transform()->SetRelativePos(vPos);
		}
	}
}

void CPlayerScript::ItemCollect()
{
	if (KEY_TAP(KEY::Z))
	{
		Layer* pItemLayer = LevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Layer_Item);
		const vector<Ptr<GameObject>>& vecItems = pItemLayer->GetParentObjects();

		Vec3 vPlayerPos = Transform()->GetRelativePos();
		CItemScript* pClosestScript = nullptr;
		float fMinDist = 160.f;

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

void CPlayerScript::TakeDamage(int _iDamage)
{
	if (m_bInvincible || m_bDead) return;

	m_iCurHP -= _iDamage;

	if (m_iCurHP <= 0)
	{
		m_iCurHP = 0;
		m_bDead = true;

		// [중요] 죽는 순간의 현재 위치를 공전 중심축으로 저장
		m_vDeadPos = Transform()->GetRelativePos();

		PlayAnim(8, 1.f, false);
	}

	if (m_pHpBar != nullptr)
		m_pHpBar->SetValue((float)m_iCurHP, (float)m_iMaxHP);

	if (!m_bDead)
	{
		m_bInvincible = true;
		m_fInvincibleTime = 0.8f;
	}
}

void CPlayerScript::UseMana(int _iMana)
{
	if (m_iCurMP >= _iMana)
	{
		m_iCurMP -= _iMana;
		if (m_pMpBar != nullptr)
			m_pMpBar->SetValue((float)m_iCurMP, (float)m_iMaxMP);
	}
}

void CPlayerScript::GainExp(int _iExp)
{
	m_iCurEXP += _iExp;

	// 레벨업 체크
	if (m_iCurEXP >= m_iMaxEXP)
	{
		// 레벨업 로직 (HP/MP 풀회복 등)
		m_iCurEXP -= m_iMaxEXP;
		m_iMaxEXP = (int)((float)m_iMaxEXP * 1.5f); // 다음 요구치 증가
	}

	if (m_pExpBar != nullptr)
		m_pExpBar->SetValue((float)m_iCurEXP, (float)m_iMaxEXP);
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
