#include "pch.h"
#include "CIronCrafting.h"
#include "CBmpMgr.h"
#include "CCraftingSlot.h"

CIronCrafting::CIronCrafting()
{
}

CIronCrafting::~CIronCrafting()
{
	Release();
}

void CIronCrafting::Initialize()
{
	AddCraftingSlot(m_tRect.left + 40.f, m_tRect.top + 70.f, IRON_SWORD);
	AddCraftingSlot(m_tRect.left + 85.f, m_tRect.top + 70.f, IRON_PICKAXE);
	AddCraftingSlot(m_tRect.left + 130.f, m_tRect.top + 70.f, IRON_BOW);

	AddCraftingSlot(m_tRect.left + 40.f, m_tRect.top + 120.f, IRON_HELM);
	AddCraftingSlot(m_tRect.left + 85.f, m_tRect.top + 120.f, IRON_CHEST);
	AddCraftingSlot(m_tRect.left + 130.f, m_tRect.top + 120.f, IRON_PANTS);

	m_tInfo.fCX = 170.f;
	m_tInfo.fCY = 150.f;
	m_tInfo.fX = 150.f;
	m_tInfo.fY = 200.f;

	m_tBmpInfo.fCX = 401.f;
	m_tBmpInfo.fCY = 368.f;

	m_eUiID = UI_CRAFTING;

	__super::Update_Rect();



	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_UI");
}
