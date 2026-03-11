#include "pch.h"
#include "CInvenCrafting.h"
#include "CAbstractFactory.h"
#include "CCraftingSlot.h"
#include "CBmpMgr.h"

CInvenCrafting::CInvenCrafting()
{
}

CInvenCrafting::~CInvenCrafting()
{
	Release();
}

void CInvenCrafting::Initialize()
{
	m_tInfo.fCX = 170.f;
	m_tInfo.fCY = 150.f;
	m_tInfo.fX = 150.f;
	m_tInfo.fY = 200.f;

	m_tBmpInfo.fCX = 401.f;
	m_tBmpInfo.fCY = 368.f;

	m_eUiID = UI_CRAFTING;

	__super::Update_Rect();

	CUi* pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left + 40.f, m_tRect.top + 70.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(WOOD_SWORD);
	m_vecCraftingSlot.push_back(pSlot);
	pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left +85.f, m_tRect.top + 70.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(WOOD_PICKAXE);
	m_vecCraftingSlot.push_back(pSlot);
	pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left + 130.f, m_tRect.top + 70.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(WOOD_BOW);
	m_vecCraftingSlot.push_back(pSlot);

	
	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_UI");
}
