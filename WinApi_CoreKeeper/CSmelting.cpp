#include "pch.h"
#include "CSmelting.h"
#include "CSlot.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CBmpMgr.h"
#include "CItem.h"
#include "CObjMgr.h"

CSmelting::CSmelting() :
	m_pSmeltingSlot(nullptr),
	m_pResultSlot(nullptr),
	m_dwSmeltingTime(0),
	m_dwSmeltingDelay(0),
	m_iSmeltingCount(0),
	m_iSmeltingCountMax(0)
{
}

CSmelting::~CSmelting()
{
	Release();
}

void CSmelting::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 180.f;
	m_tInfo.fX = 400.f;
	m_tInfo.fY = 200.f;

	m_tBmpInfo.fCX = 401.f;
	m_tBmpInfo.fCY = 278.f;

	m_eUiID = UI_SMELTING;

	m_iSmeltingCount = 0;
	m_iSmeltingCountMax = 4;
	m_dwSmeltingDelay = 1000;

	m_pSmeltingSlot = CUIFactory<CSlot>::Create(m_tInfo.fX - 50, m_tInfo.fY);
	dynamic_cast<CSlot*>(m_pSmeltingSlot)->Set_SlotID(SLOT_FORGE);

	m_pResultSlot = CUIFactory<CSlot>::Create(m_tInfo.fX + 50, m_tInfo.fY);
	dynamic_cast<CSlot*>(m_pResultSlot)->Set_SlotID(SLOT_FORGE_RESULT);

	CCollisionMgr::Get_Instance()->Push_CollisionSlotList(m_pSmeltingSlot, SLOT_FORGE);
	CCollisionMgr::Get_Instance()->Push_CollisionSlotList(m_pResultSlot, SLOT_FORGE_RESULT);


	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Core_Equip");
	m_hFireDC = CBmpMgr::Get_Instance()->Find_Image(L"Forge_Fire");

	__super::Update_Rect();
}

void CSmelting::Update()
{
	if (m_bOpen)
	{
		m_bRender = true;
		

		m_pSmeltingSlot->Set_Render(m_bRender);

		m_pResultSlot->Set_Render(m_bRender);
		
	}
	else
	{
		m_bRender = false;
	
		m_pSmeltingSlot->Set_Render(m_bRender);

		m_pResultSlot->Set_Render(m_bRender);
	}

	m_pSmeltingSlot->Update();
	m_pResultSlot->Update();
	
	CSlot* pSmeltingSlot = dynamic_cast<CSlot*>(m_pSmeltingSlot);

	if (pSmeltingSlot->Get_Item())
	{
		CSlot* pResultSlot = dynamic_cast<CSlot*>(m_pResultSlot);

		m_bSmelting = true;

		if (m_dwSmeltingTime + m_dwSmeltingDelay < GetTickCount())
		{
			++m_iSmeltingCount;

			if (m_iSmeltingCount > m_iSmeltingCountMax)
			{
				m_iSmeltingCount = 0;

				CItem* pItem = dynamic_cast<CItem*>(pSmeltingSlot->Get_Item());
				pItem->DecreaseCount(1);

				if (pResultSlot->Get_Item())
				{
					CItem* pResultItem = dynamic_cast<CItem*>(pResultSlot->Get_Item());
					pResultItem->IncreaseCount(1);
				}
				else
				{
					CObj* pResult = CAbstractFactory<CItem>::Create();
					dynamic_cast<CItem*>(pResult)->Set_Item(IRON);
					dynamic_cast<CItem*>(pResult)->Set_Inventory(true);
					CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pResult);
					pResultSlot->Set_Item(pResult);
				}
			}

			m_dwSmeltingTime = GetTickCount();
		}
	}
	else
	{
		m_dwSmeltingTime = GetTickCount();
		m_bSmelting = false;
	}


	__super::Update_Rect();
}

void CSmelting::Late_Update()
{
}

void CSmelting::Render(HDC _hDC)
{
	if (m_bRender)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

		GdiTransparentBlt(_hDC, (int)m_tInfo.fX - 20, (int)m_tInfo.fY - 20, 40, 40,
			m_hFireDC, m_iSmeltingCount * 16, 0, 16, 19, RGB(0, 255, 0));

		m_pSmeltingSlot->Render(_hDC);
		m_pResultSlot->Render(_hDC);
	}
}

void CSmelting::Release()
{
	Safe_Delete<CUi*>(m_pSmeltingSlot);
	Safe_Delete<CUi*>(m_pResultSlot);
}
