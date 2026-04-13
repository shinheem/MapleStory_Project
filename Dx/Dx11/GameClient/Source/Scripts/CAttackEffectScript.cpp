#include "pch.h"
#include "CAttackEffectScript.h"

#include "GameObject.h"

CAttackEffectScript::CAttackEffectScript()
	: CScript(SCRIPT_TYPE::ATTACKEFFECTSCRIPT)
{

}

CAttackEffectScript::~CAttackEffectScript()
{

}

void CAttackEffectScript::Begin()
{
    // 생성되자마자 0번 애니메이션 재생
    if (GetOwner()->FlipbookRender())
    {
        GetOwner()->FlipbookRender()->Play(0, 7.f, 1);
    }
}

void CAttackEffectScript::Tick()
{
    // 애니메이션이 끝나면 오브젝트 삭제
    if (GetOwner()->FlipbookRender()->CheckFinish())
    {
        GetOwner()->Destroy();
    }
}