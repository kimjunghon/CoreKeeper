#pragma once
#include "CUi.h"
#include "CObj.h"

class CSlot : public CUi
{
public:
	CSlot();
	~CSlot();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	void Set_SlotID(SLOTID _eSlotID);
	CObj* Get_Item()
	{
		if (m_pItem)
			return m_pItem;
		else
			return nullptr;
	}

	void Set_Item(CObj* _pObj) { m_pItem = _pObj; }

	bool MouseOver();


private:
	CObj* m_pItem;
	CObj* m_pPreItem;
	int m_iItemCount;
protected:
	SLOTID m_eSlotID;
};

