#pragma once

#include "Source\Scripts\CInventoryScript.h"

class ItemMgr
	: public singleton<ItemMgr>
{
	SINGLE(ItemMgr)
private:
	
public:
	void CreateItemDrop(Ptr<tItemInfo> _pInfo, Vec3 _vPos);
};

