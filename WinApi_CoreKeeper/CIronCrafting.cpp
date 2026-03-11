#include "pch.h"
#include "CIronCrafting.h"
#include "CBmpMgr.h"
#include "CCraftingSlot.h"
#include "CAbstractFactory.h"

CIronCrafting::CIronCrafting()
{
}

CIronCrafting::~CIronCrafting()
{
	Release();
}

void CIronCrafting::Initialize()
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
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(IRON_SWORD);
	m_vecCraftingSlot.push_back(pSlot);
	pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left + 85.f, m_tRect.top + 70.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(IRON_PICKAXE);
	m_vecCraftingSlot.push_back(pSlot);
	pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left + 130.f, m_tRect.top + 70.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(IRON_BOW);
	m_vecCraftingSlot.push_back(pSlot);

	pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left + 40.f, m_tRect.top + 120.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(IRON_HELM);
	m_vecCraftingSlot.push_back(pSlot);
	pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left + 85.f, m_tRect.top + 120.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(IRON_CHEST);
	m_vecCraftingSlot.push_back(pSlot);
	pSlot = CUIFactory<CCraftingSlot>::Create(m_tRect.left + 130.f, m_tRect.top + 120.f);
	dynamic_cast<CCraftingSlot*>(pSlot)->Set_CraftingItem(IRON_PANTS);
	m_vecCraftingSlot.push_back(pSlot);

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_UI");
}
