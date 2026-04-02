#pragma once
class ItemMgr
	: public singleton<ItemMgr>
{
	SINGLE(ItemMgr)
private:
	
public:
	void CreateItemDrop(ITEM_TYPE _eType, const wstring& _strName, Vec3 _vSpawnPos);
};

