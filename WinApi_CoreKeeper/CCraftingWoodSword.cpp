#include "pch.h"
#include "CCraftingWoodSword.h"
#include "CBmpMgr.h"

CCraftingWoodSword::CCraftingWoodSword()
{
}

CCraftingWoodSword::~CCraftingWoodSword()
{
}

void CCraftingWoodSword::Initialize()
{
	m_tInfo.fCX = SLOTCX;
	m_tInfo.fCY = SLOTCY;

	m_tBmpInfo.fCX = 54.f;
	m_tBmpInfo.fCY = 54.f;


	m_iCraftingItemCode = WOOD_SWORD;
	m_iNeedItemCode = TURF_WALL;
	m_iNeedItemCount = 5;

	m_eSlotID = SLOT_CRAFTING;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_Sword_wood");
}
