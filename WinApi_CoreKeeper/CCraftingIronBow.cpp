#include "pch.h"
#include "CCraftingIronBow.h"
#include "CBmpMgr.h"

CCraftingIronBow::CCraftingIronBow()
{
}

CCraftingIronBow::~CCraftingIronBow()
{
}

void CCraftingIronBow::Initialize()
{
	m_tInfo.fCX = SLOTCX;
	m_tInfo.fCY = SLOTCY;

	m_tBmpInfo.fCX = 54.f;
	m_tBmpInfo.fCY = 54.f;


	m_iCraftingItemCode = IRON_BOW;
	m_iNeedItemCode = IRON;
	m_iNeedItemCount = 5;

	m_eSlotID = SLOT_CRAFTING;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_iron_bow");
}
