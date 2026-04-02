#include "pch.h"
#include "CCamMoveScript.h"

#include "KeyMgr.h"
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
	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
		MovePerspective();
	else
		MoveOrthographic();
}

void CCamMoveScript::MovePerspective()
{
	/*Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetDir(DIR::RIGHT);


	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);*/
}

void CCamMoveScript::MoveOrthographic()
{
	/*Vec3 vPos = Transform()->GetRelativePos();	

	if (KEY_PRESSED(KEY::W))
		vPos.y += DT * 500.f;
	if (KEY_PRESSED(KEY::S))
		vPos.y -= DT * 500.f;
	if (KEY_PRESSED(KEY::A))
		vPos.x -= DT * 500.f;
	if (KEY_PRESSED(KEY::D))
		vPos.x += DT * 500.f;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));*/
}
