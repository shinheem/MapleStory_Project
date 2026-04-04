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

void CCamMoveScript::Tick()
{
    // 1. 타겟 및 카메라 컴포넌트 가져오기
    GameObject* pPlayer = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Maple_Player").Get();
    if (nullptr == pPlayer) return;

    CCamera* pCam = GetOwner()->Camera().Get();
    if (nullptr == pCam) return;

    // 2. 위치 정보 가져오기
    Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
    Vec3 vCamPos = Transform()->GetRelativePos();

    // ---------------------------------------------------------
    // ★ 수정 1: Offset을 대폭 낮추거나 0으로 설정
    // 캐릭터 상단이 찍힌다는 건 카메라 목표지점이 너무 높다는 뜻입니다.
    // 0.f로 먼저 테스트해보고, 캐릭터 발이 잘린다면 조금씩 올리세요.
    float fOffsetY = 0.f; 
    Vec3 vTargetPos = Vec3(vPlayerPos.x, vPlayerPos.y + fOffsetY, vPlayerPos.z);
    // ---------------------------------------------------------

    // 3. 부드러운 추적
    vCamPos.x = Lerp(vCamPos.x, vTargetPos.x, DT * 3.5f);
    vCamPos.y = Lerp(vCamPos.y, vTargetPos.y, DT * 3.5f);

    // 4. 확대 설정
    float fScale = 0.65f;
    if (PROJ_TYPE::PERSPECTIVE == pCam->GetProjType()) {
        vCamPos.z = -400.f;
    } else {
        pCam->SetOrthoScale(fScale);
    }

    // ---------------------------------------------------------
    // ★ 수정 2: 맵 경계 제한 확인 (가장 의심되는 부분)
    // 캐릭터가 바닥 근처에 있을 때 fMapMinY가 너무 높으면 카메라가 위로 튕깁니다.
    // 일단 제한을 확 풀어서(-2000.f) 캐릭터가 중앙에 오는지 확인하세요.
    // ---------------------------------------------------------
    float fMapMinX = -2000.f;  float fMapMaxX = 5000.f;
    float fMapMinY = -2000.f;  float fMapMaxY = 5000.f; // 우선 범위를 크게 잡으세요.

    float fHalfWidth = (1600.f * fScale) * 0.5f;
    float fHalfHeight = (900.f * fScale) * 0.5f;

    // Clamp 로직
    if (vCamPos.x < fMapMinX + fHalfWidth) vCamPos.x = fMapMinX + fHalfWidth;
    if (vCamPos.x > fMapMaxX - fHalfWidth) vCamPos.x = fMapMaxX - fHalfWidth;
    if (vCamPos.y < fMapMinY + fHalfHeight) vCamPos.y = fMapMinY + fHalfHeight;
    if (vCamPos.y > fMapMaxY - fHalfHeight) vCamPos.y = fMapMaxY - fHalfHeight;

    // 5. 결과 적용
    Transform()->SetRelativePos(vCamPos);
}

void CCamMoveScript::MovePerspective()
{
	//// 플레이어 위치와 카메라 위치 가져오기
	//Vec3 vPlayerPos = m_pTarget->Transform()->GetRelativePos();
	//Vec3 vCamPos = Transform()->GetRelativePos();

	//// [추적] X, Y 좌표를 플레이어 위치로 부드럽게 이동 (Lerp)
	//vCamPos.x = Lerp(vCamPos.x, vPlayerPos.x, DT * 10.f);
	//vCamPos.y = Lerp(vCamPos.y, vPlayerPos.y, DT * 10.f);

	//// [확대] Z값을 0에 가깝게 설정 (값이 커질수록(0에 가까울수록) 화면이 확대됨)
	//// -1000.f가 기본이라면, -300.f ~ -400.f 정도로 설정하면 많이 확대됩니다.
	//vCamPos.z = -400.f;

	//// 최종 좌표 적용
	//Transform()->SetRelativePos(vCamPos);
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
