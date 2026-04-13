#include "pch.h"
#include "CCamMoveScript.h"

#include "KeyMgr.h"
#include "LevelMgr.h"
#include "TimeMgr.h"
#include "CTransform.h"
#include "CCamera.h"

CCamMoveScript::CCamMoveScript()
	: CScript(SCRIPT_TYPE::CAMMOVESCRIPT)
{
}

CCamMoveScript::~CCamMoveScript()
{
}

void CCamMoveScript::Begin()
{
	GameObject* pPlayer = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Maple_Player").Get();
	if (pPlayer)
	{
		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
		Vec3 vCamPos = Transform()->GetRelativePos();

		// Begin 호출 시점에 즉시 플레이어 위치로 카메라 좌표를 동기화
		vCamPos.x = vPlayerPos.x;
		vCamPos.y = vPlayerPos.y;
		Transform()->SetRelativePos(vCamPos);

		// 행렬 갱신을 통해 Tick 이전에 이미 위치가 확정되게 함
		Transform()->FinalTick();
	}
}

void CCamMoveScript::Tick()
{
	GameObject* pPlayer = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Maple_Player").Get();
	if (nullptr == pPlayer) return;

	CCamera* pCam = GetOwner()->Camera().Get();
	if (nullptr == pCam) return;

	Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
	Vec3 vCamPos = Transform()->GetRelativePos();
	Vec3 vTargetPos = Vec3(vPlayerPos.x, vPlayerPos.y, vCamPos.z);

	// 현재 좌표와 목표 좌표 사이의 거리 체크 (2D 평면)
	float fDist = Vec2::Distance(Vec2(vCamPos.x, vCamPos.y), Vec2(vTargetPos.x, vTargetPos.y));

	// [중요] 거리가 너무 멀면(레벨 이동 등) Lerp를 무시하고 즉시 이동
	if (fDist > 100.f)
	{
		vCamPos.x = vTargetPos.x;
		vCamPos.y = vTargetPos.y;
	}
	else
	{
		// 평상시 부드러운 이동
		vCamPos.x = Lerp(vCamPos.x, vTargetPos.x, DT * 8.0f);
		vCamPos.y = Lerp(vCamPos.y, vTargetPos.y, DT * 8.0f);
	}

	Transform()->SetRelativePos(vCamPos);
}

void CCamMoveScript::MovePerspective()
{
	// 플레이어 위치와 카메라 위치 가져오기
	Vec3 vPlayerPos = m_pTarget->Transform()->GetRelativePos();
	Vec3 vCamPos = Transform()->GetRelativePos();

	// [추적] X, Y 좌표를 플레이어 위치로 부드럽게 이동 (Lerp)
	vCamPos.x = Lerp(vCamPos.x, vPlayerPos.x, DT * 10.f);
	vCamPos.y = Lerp(vCamPos.y, vPlayerPos.y, DT * 10.f);

	// [확대] Z값을 0에 가깝게 설정 (값이 커질수록(0에 가까울수록) 화면이 확대됨)
	// -1000.f가 기본이라면, -300.f ~ -400.f 정도로 설정하면 많이 확대됩니다.
	vCamPos.z = -400.f;

	// 최종 좌표 적용
	Transform()->SetRelativePos(vCamPos);
}

void CCamMoveScript::MoveOrthographic()
{
	//Vec3 vPlayerPos = m_pTarget->Transform()->GetRelativePos();
	//Vec3 vCamPos = Transform()->GetRelativePos();

	//// [추적] X, Y 좌표 이동
	//vCamPos.x = Lerp(vCamPos.x, vPlayerPos.x, DT * 10.f);
	//vCamPos.y = Lerp(vCamPos.y, vPlayerPos.y, DT * 10.f);

	//// [확대] 직교 투영은 Z값이 아닌 Camera의 Scale을 조절해야 합니다.
	//// 1.0f가 기본(100%), 0.5f가 200% 확대입니다.
	//Camera()->SetScale(0.6f);

	//// 최종 좌표 적용 (Z값은 기존 카메라 설정 유지)
	//Transform()->SetRelativePos(Vec3(vCamPos.x, vCamPos.y, vCamPos.z));
}
