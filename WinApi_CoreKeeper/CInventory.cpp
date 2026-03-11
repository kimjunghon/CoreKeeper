#include "pch.h"
#include "CInventory.h"
#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CWoodBow.h"
#include "CWoodPickaxe.h"
#include "CWoodSword.h"
#include "CObjMgr.h"
#include "CItem.h"
#include "CSlot.h"

CInventory::CInventory() :
	m_iInvenSpace(0),
	m_iMaxInvenSpace(0)
{
	m_vecQuickSlot.reserve(DEFAULT_QUICK);
	m_vecSlot.reserve(DEFAULT_INVEN);
}

CInventory::~CInventory()
{
	Release();
}

void CInventory::Initialize()
{
	m_tInfo.fCX = 500.f;
	m_tInfo.fCY = 275.f;
	m_tInfo.fX = 400.f;
	m_tInfo.fY = 450.f;

	m_tBmpInfo.fCX = 500.f;
	m_tBmpInfo.fCY = 275.f;

	m_iMaxInvenSpace = 20;

	__super::Update_Rect();

	m_eUiID = UI_INVENTORY;

	for (int i = 0; i < DEFAULT_QUICK; i++)
	{
		float fX = (m_tRect.left + (SLOTCX * 0.5f)) + (i * SLOTCX) + (5.f * i) + 23.f;
		CUi* pSlot = CUIFactory<CSlot>::Create(fX, WINCY - (SLOTCY * 0.5f));
		dynamic_cast<CSlot*>(pSlot)->Set_SlotID(SLOT_QUICK);

		m_vecQuickSlot.push_back(pSlot);
	}

	for (int i = 0; i < 2; i++)
	{
		for(int j= 0; j< DEFAULT_INVEN / 2; j++)
		{
			float fX = (m_tRect.left + (SLOTCX * 0.5f)) + (j * SLOTCX) + (5.f * j) + 23.f;
			float fY = (m_tRect.top + (SLOTCY * 0.5f)) + (i * SLOTCY) + (7.f*i) + 80.f;

			CUi* pSlot = CUIFactory<CSlot>::Create(fX, fY);
			dynamic_cast<CSlot*>(pSlot)->Set_SlotID(SLOT_INVEN);

			m_vecSlot.push_back(pSlot);
		}
	}

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Inven_Background");
}

void CInventory::Update()
{
	if (m_bOpen)
	{
		m_bRender = true;
		for (auto& vecQuick : m_vecQuickSlot)
		{
			vecQuick->Update();
			vecQuick->Set_PosY(WINCY - (SLOTCY * 0.5f) - 230.f);
			vecQuick->Set_Render(m_bRender);
		}
	}
	else
	{
		m_bRender = false;
		for (auto& vecQuick : m_vecQuickSlot)
		{
			vecQuick->Update();
			vecQuick->Set_PosY(WINCY - (SLOTCY * 0.5f));
			vecQuick->Set_Render(m_bRender);
		}
	}

	for (auto& vecSlot : m_vecSlot)
	{
		vecSlot->Update();
		vecSlot->Set_Render(m_bRender);
	}
}

void CInventory::Late_Update()
{
	for (auto& vecQuick : m_vecQuickSlot)
	{
		vecQuick->Late_Update();
	}

	for (auto& vecSlot : m_vecSlot)
	{
		vecSlot->Late_Update();
	}
}

void CInventory::Render(HDC _hDC)
{
	if(m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

		for (auto& vecSlot : m_vecSlot)
		{
			vecSlot->Render(_hDC);
		}
	}
	for (auto& vecQuick : m_vecQuickSlot)
	{
		vecQuick->Render(_hDC);
	}
}

void CInventory::Release()
{
	for_each(m_vecSlot.begin(), m_vecSlot.end(), Safe_Delete<CUi*>);
	m_vecSlot.clear();
	m_vecSlot.shrink_to_fit();

	for_each(m_vecQuickSlot.begin(), m_vecQuickSlot.end(), Safe_Delete<CUi*>);
	m_vecSlot.clear();
	m_vecSlot.shrink_to_fit();
}

bool CInventory::Add_Item(CObj* _pObj)
{
	if (!_pObj)
		return false;

	CItem* pAddItem = dynamic_cast<CItem*>(_pObj);
	

	for (auto& vecSlot : m_vecSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == pAddItem->Get_ItemCode())
			{
				if (!pSlotItem->IsItemFullCount())
				{
					pSlotItem->IncreaseCount(pAddItem->Get_ItemCount());
					_pObj->Set_Dead();
					return true;
				}
			}
		}
	}
	for (auto& vecSlot : m_vecQuickSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == pAddItem->Get_ItemCode())
			{
				if (!pSlotItem->IsItemFullCount())
				{
					pSlotItem->IncreaseCount(pAddItem->Get_ItemCount());
					_pObj->Set_Dead();
					return true;
				}
			}
		}
	}

	for (auto& vecSlot : m_vecSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));
		if (!pSlotItem)
		{
			dynamic_cast<CSlot*>(vecSlot)->Set_Item(_pObj);
			return true;
		}
	}

	for (auto& vecSlot : m_vecQuickSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));
		if (!pSlotItem)
		{
			dynamic_cast<CSlot*>(vecSlot)->Set_Item(_pObj);
			return true;
		}
	}

	return false;
}

CObj* CInventory::Equip_Item(int _iIndex)
{
	return dynamic_cast<CSlot*>(m_vecQuickSlot[_iIndex])->Get_Item();
}

bool CInventory::Check_Crafting(int _iItemCode, int _iItemCount)
{
	int iTotal(0);

	for (auto& vecSlot : m_vecSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == _iItemCode)
			{
				if (pSlotItem->Get_ItemCount() >= _iItemCount)
				{
					return true;
				}
				else
				{
					iTotal += pSlotItem->Get_ItemCount();
				}
			}
		}
	}

	for (auto& vecSlot : m_vecQuickSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == _iItemCode)
			{
				if (pSlotItem->Get_ItemCount() >= _iItemCount)
				{
					return true;
				}
				else
				{
					iTotal += pSlotItem->Get_ItemCount();
				}
			}
		}
	}
	
	if (iTotal >= _iItemCount)
		return true;
	

	return false;
}

void CInventory::DecreaseItem(int _iItemCode, int _iItemCount)
{
	int iTotal = _iItemCount;

	for (auto& vecSlot : m_vecSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == _iItemCode)
			{
				if (pSlotItem->Get_ItemCount() >= iTotal)
				{
					pSlotItem->DecreaseCount(iTotal);
				}
				else
				{
					iTotal -= pSlotItem->Get_ItemCount();
					pSlotItem->DecreaseCount(pSlotItem->Get_ItemCount());
				}
			}
		}
	}

	for (auto& vecSlot : m_vecQuickSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == _iItemCode)
			{
				if (pSlotItem->Get_ItemCount() >= iTotal)
				{
					pSlotItem->DecreaseCount(iTotal);
				}
				else
				{
					iTotal -= pSlotItem->Get_ItemCount();
					pSlotItem->DecreaseCount(pSlotItem->Get_ItemCount());
				}
			}
		}
	}
}

int CInventory::Check_ItemCount(int _iItemCode)
{
	int iTotal(0);

	for (auto& vecSlot : m_vecSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == _iItemCode)
			{
				iTotal += pSlotItem->Get_ItemCount();
			}
		}
	}

	for (auto& vecSlot : m_vecQuickSlot)
	{
		CItem* pSlotItem = dynamic_cast<CItem*>((dynamic_cast<CSlot*>(vecSlot)->Get_Item()));

		if (pSlotItem)
		{
			if (pSlotItem->Get_ItemCode() == _iItemCode)
			{
				iTotal += pSlotItem->Get_ItemCount();
			}
		}
	}

	return iTotal;
}


