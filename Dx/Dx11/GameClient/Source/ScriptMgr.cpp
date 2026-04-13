#include "pch.h"
#include "ScriptMgr.h"

#include "Scripts/CBossPartScript.h"
#include "Scripts/CCamMoveScript.h"
#include "Scripts/CHonTailBossScript.h"
#include "Scripts/CMissileScript.h"
#include "Scripts/CMonsterScript.h"
#include "Scripts/CPlayerScript.h"
#include "Scripts/CInventoryScript.h"
#include "Scripts/CGaugeBarScript.h"
#include "Scripts/CSlotScript.h"
#include "Scripts/CItemScript.h"
#include "Scripts/CCursorScript.h" 
#include "Scripts/CFadeScript.h"
#include "Scripts/CMonsterSpawner.h"
#include "Scripts/CPortalScript.h"
#include "Scripts/CLeftBossAloneScript.h"
#include "Scripts/CRightBossAloneScript.h"
#include "Scripts/CAttackEffectScript.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBossPartScript");
	_vec.push_back(L"CCamMoveScript");
	_vec.push_back(L"CHonTailBossScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CInventoryScript");
	_vec.push_back(L"CGaugeBarScript");
	_vec.push_back(L"CSlotScript");
	_vec.push_back(L"CItemScript");
	_vec.push_back(L"CCursorScript");
	_vec.push_back(L"CFadeScript");
	_vec.push_back(L"CMonsterSpawner");
	_vec.push_back(L"CPortalScript");
	_vec.push_back(L"CLeftBossAloneScript");
	_vec.push_back(L"CRightBossAloneScript");
	_vec.push_back(L"CAttackEffectScript");
}

CScript * ScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBossPartScript" == _strScriptName)
		return new CBossPartScript;
	if (L"CCamMoveScript" == _strScriptName)
		return new CCamMoveScript;
	if (L"CHonTailBossScript" == _strScriptName)
		return new CHonTailBossScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript; 
	if (L"CInventoryScript" == _strScriptName)
		return new CInventoryScript;
	if (L"CGaugeBarScript" == _strScriptName)
		return new CGaugeBarScript;
	if (L"CSlotScript" == _strScriptName)
		return new CSlotScript;
	if (L"CItemScript" == _strScriptName)
		return new CItemScript;
	if (L"CCursorScript" == _strScriptName)
		return new CCursorScript;
	if (L"CFadeScript" == _strScriptName)
		return new CFadeScript;
	if (L"CMonsterSpawner" == _strScriptName)
		return new CMonsterSpawner;
	if (L"CPortalScript" == _strScriptName)
		return new CPortalScript;
	if (L"CLeftBossAloneScript" == _strScriptName)
		return new CLeftBossAloneScript;
	if (L"CRightBossAloneScript" == _strScriptName)
		return new CRightBossAloneScript;
	if (L"CAttackEffectScript" == _strScriptName)
		return new CAttackEffectScript;
	return nullptr;
}

CScript * ScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BOSSPARTSCRIPT:
		return new CBossPartScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMMOVESCRIPT:
		return new CCamMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::HONTAILBOSSSCRIPT:
		return new CHonTailBossScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::INVENTORYSCRIPT:
		return new CInventoryScript;
		break;
	case (UINT)SCRIPT_TYPE::GAUGEBARSCRIPT:
		return new CGaugeBarScript;
		break;
	case (UINT)SCRIPT_TYPE::SLOTSCRIPT:
		return new CSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMSCRIPT:
		return new CItemScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSORSCRIPT:
		return new CCursorScript;
		break;
	case (UINT)SCRIPT_TYPE::FADESCRIPT:
		return new CFadeScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSPAWNER:
		return new CMonsterSpawner;
		break;
	case (UINT)SCRIPT_TYPE::PORTALSCRIPT:
		return new CPortalScript;
		break;
	case (UINT)SCRIPT_TYPE::LEFTBOSSALONESCRIPT:
		return new CLeftBossAloneScript;
		break;
	case (UINT)SCRIPT_TYPE::RIGHTBOSSALONESCRIPT:
		return new CRightBossAloneScript;
		break;
	case (UINT)SCRIPT_TYPE::ATTACKEFFECTSCRIPT:
		return new CAttackEffectScript;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BOSSPARTSCRIPT:
		return L"CBossPartScript";
		break;

	case SCRIPT_TYPE::CAMMOVESCRIPT:
		return L"CCamMoveScript";
		break;

	case SCRIPT_TYPE::HONTAILBOSSSCRIPT:
		return L"CHonTailBossScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::INVENTORYSCRIPT:
		return L"CInventoryScript";
		break;

	case SCRIPT_TYPE::GAUGEBARSCRIPT:
		return L"CGaugeBarScript";
		break;

	case SCRIPT_TYPE::SLOTSCRIPT:
		return L"CSlotScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"CItemScript";
		break;
	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
		break;
	case SCRIPT_TYPE::FADESCRIPT:
		return L"CFadeScript";
		break;
	case SCRIPT_TYPE::MONSTERSPAWNER:
		return L"CMonsterSpawner";
		break;
	case SCRIPT_TYPE::PORTALSCRIPT:
		return L"CPortalScript";
		break;
	case SCRIPT_TYPE::LEFTBOSSALONESCRIPT:
		return L"CLeftBossAloneScript";
		break;
	case SCRIPT_TYPE::RIGHTBOSSALONESCRIPT:
		return L"CRightBossAloneScript";
		break;
	case SCRIPT_TYPE::ATTACKEFFECTSCRIPT:
		return L"CAttackEffectScript";
		break;
	}
	return nullptr;
}
const wchar_t* ScriptMgr::GetScriptNameByType(SCRIPT_TYPE type)
{
	switch (type)
	{
	case SCRIPT_TYPE::BOSSPARTSCRIPT:
		return L"CBossPartScript";
	case SCRIPT_TYPE::CAMMOVESCRIPT:
		return L"CCamMoveScript";
	case SCRIPT_TYPE::HONTAILBOSSSCRIPT:
		return L"CHonTailBossScript";
	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
	case SCRIPT_TYPE::INVENTORYSCRIPT:
		return L"CInventoryScript";
	case SCRIPT_TYPE::GAUGEBARSCRIPT:
		return L"CGaugeBarScript";
	case SCRIPT_TYPE::SLOTSCRIPT:
		return L"CSlotScript";
	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"CItemScript";
	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
	case SCRIPT_TYPE::FADESCRIPT:
		return L"CFadeScript";
	case SCRIPT_TYPE::MONSTERSPAWNER:
		return L"CMonsterSpawner";
	case SCRIPT_TYPE::PORTALSCRIPT:
		return L"CPortalScript";
	case SCRIPT_TYPE::LEFTBOSSALONESCRIPT:
		return L"CLeftBossAloneScript";
	case SCRIPT_TYPE::RIGHTBOSSALONESCRIPT:
		return L"CRightBossAloneScript";
	case SCRIPT_TYPE::ATTACKEFFECTSCRIPT:
		return L"CAttackEffectScript";
	default:
		return nullptr;
	}
}
