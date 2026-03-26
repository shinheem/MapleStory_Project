#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BOSSPARTSCRIPT,
	CAMMOVESCRIPT,
	HONTAILBOSSSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	PLAYERSCRIPT,
};

using namespace std;

class CScript;

class ScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
	static const wchar_t* GetScriptNameByType(SCRIPT_TYPE type);
};
