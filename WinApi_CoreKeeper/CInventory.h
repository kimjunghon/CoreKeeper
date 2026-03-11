#pragma once
#include "CUi.h"
#include "CObj.h"

class CInventory : public CUi
{
public:
	CInventory();
	~CInventory();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	vector<CUi*>* Get_vecSlot() { return &m_vecSlot; }
	vector<CUi*>* Get_vecQuickSlot() { return &m_vecQuickSlot; }
	bool Add_Item(CObj* _pObj);
	CObj* Equip_Item(int _iIndex);

	bool Check_Crafting(int _iItemCode, int _iItemCount);
	void DecreaseItem(int _iItemCode, int _iItemCount);
	int Check_ItemCount(int _iItemCode);

private:
	vector<CUi*> m_vecQuickSlot;
	vector<CUi*> m_vecSlot;

	int m_iInvenSpace;
	int m_iMaxInvenSpace;
};

