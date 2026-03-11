#include "pch.h"
#include "CSlot.h"
#include "CBmpMgr.h"
#include "CItem.h"

CSlot::CSlot() :
	m_pItem(nullptr),
	m_pPreItem(nullptr),
	m_eSlotID(SLOT_END),
	m_iItemCount(0)
{

}

CSlot::~CSlot()
{
	Release();
}

void CSlot::Initialize()
{
	m_tInfo.fCX = SLOTCX;
	m_tInfo.fCY = SLOTCY;

	m_tBmpInfo.fCX = 40.f;
	m_tBmpInfo.fCY = 40.f;

}

void CSlot::Update()
{
	if(m_pItem)
	{
		if (m_pItem->Is_Dead())
			m_pItem = nullptr;
		else
			m_iItemCount = dynamic_cast<CItem*>(m_pItem)->Get_ItemCount();
	}

	__super::Update_Rect();
}

void CSlot::Late_Update()
{
}

void CSlot::Render(HDC _hDC)
{
	//GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

	BitBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hUiDC, 0,0, SRCCOPY);

	if (!m_pItem && m_eSlotID == SLOT_EQUIP_CHEST)
	{
		HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(L"Equip_Slot_Chest");
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hItemDC, 0, 0, 16, 16, RGB(255, 255, 255));
	}
	else if (!m_pItem && m_eSlotID == SLOT_EQUIP_HELM)
	{
		HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(L"Equip_Slot_Helm");
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hItemDC, 0, 0, 16, 16, RGB(255, 255, 255));
	}
	else if (!m_pItem && m_eSlotID == SLOT_EQUIP_PANTS)
	{
		HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(L"Equip_Slot_Pants");
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hItemDC, 0, 0, 16, 16, RGB(255, 255, 255));
	}
	else if (!m_pItem && m_eSlotID == SLOT_SLIME_CORE)
	{
		HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_Core_Slot");
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hItemDC, 0, 0, 16, 16, RGB(255, 255, 255));
	}
	else if (!m_pItem && m_eSlotID == SLOT_HIVE_CORE)
	{
		HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(L"Hive_Core_Slot");
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hItemDC, 0, 0, 16, 16, RGB(255, 255, 255));
	}



	if (m_pItem)
	{
		HDC hItemDC = *(m_pItem->Get_SlotDC());
		BMPINFO tBmpInfo = (m_pItem)->Get_BmpInfo();

		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hItemDC, 0, 0, (int)tBmpInfo.fCX, (int)tBmpInfo.fCY, RGB(255, 255, 255));

		if (m_iItemCount > 1)
		{
			HDC hItemCountDC = CBmpMgr::Get_Instance()->Find_Image(L"FontNumber");

			if (m_iItemCount >= 10)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 10, (int)m_tRect.top, 5, 15,
					hItemCountDC, (m_iItemCount / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 15, (int)m_tRect.top, 5, 15,
					hItemCountDC, (m_iItemCount%10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 10, (int)m_tRect.top, 5, 15,
					hItemCountDC, (m_iItemCount % 10) * 15, 0, 15, 45, RGB(0, 0, 0));

			}
		}

//		if (MouseOver())
//		{
//			HDC hInfoDC = *(dynamic_cast<CItem*>(m_pItem)->Get_InfoDC());
//
//
//		}
	}
}

void CSlot::Release()
{
}

void CSlot::Set_SlotID(SLOTID _eSlotID)
{
	m_eSlotID = _eSlotID;

	switch (m_eSlotID)
	{
	case SLOT_INVEN:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Slot");
		break;
	case SLOT_QUICK:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Quick_Slot");
		break;
	case SLOT_EQUIP_HELM:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Quick_Slot");
		break;
	case SLOT_EQUIP_CHEST:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Quick_Slot");
		break;
	case SLOT_EQUIP_PANTS:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Quick_Slot");
		break;
	case SLOT_SLIME_CORE:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Core_Slot");
		break;
	case SLOT_HIVE_CORE:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Core_Slot");
		break;
	case SLOT_FORGE:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Core_Slot");
		break;
	case SLOT_FORGE_RESULT:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Core_Slot");
		break;
	}
}

bool CSlot::MouseOver()
{
	if (!m_bRender)
	{
		return false;
	}

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt) && m_pItem)
	{
		return true;
	}

	return false;
}

