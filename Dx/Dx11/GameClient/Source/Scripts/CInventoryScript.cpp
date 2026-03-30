#include "pch.h"
#include "CInventoryScript.h"

#include "../../LevelMgr.h"
#include "../../KeyMgr.h"
#include "../../GameObject.h"

CInventoryScript::CInventoryScript()
	: CScript(SCRIPT_TYPE::INVENTORYSCRIPT)
    , m_pInventoryUI(nullptr)
{
}

void CInventoryScript::Begin()
{
    if (GetOwner()->GetParent() != nullptr)
        return;

    GetOwner()->SetActive(false);
}

CInventoryScript::~CInventoryScript()
{
}

void CInventoryScript::Tick()
{
    if (GetOwner()->GetParent() != nullptr)
        return;

    if (KEY_TAP(KEY::I))
    {
        GetOwner()->SetActive(!GetOwner()->IsActive());
    }
    else if (KEY_TAP(KEY::ESC))
    {
        GetOwner()->SetActive(false);
    }
}

