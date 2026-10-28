#include "pch.h"
#include "CCooking.h"
#include "CCraftingSlot.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"

CCooking::CCooking()
{
}

CCooking::~CCooking()
{
	Release();
}

void CCooking::Initialize()
{
	m_tInfo.fCX = 170.f;
	m_tInfo.fCY = 150.f;
	m_tInfo.fX = 150.f;
	m_tInfo.fY = 200.f;

	m_tBmpInfo.fCX = 401.f;
	m_tBmpInfo.fCY = 368.f;

	m_eUiID = UI_CRAFTING;

	__super::Update_Rect();

	AddCraftingSlot(m_tRect.left + 40.f, m_tRect.top + 70.f, SLIME_FOOD);
	CCollisionMgr::Get_Instance()->Push_CollisionSlotList(m_vecCraftingSlot.back(), SLOT_CRAFTING);

	AddCraftingSlot(m_tRect.left + 85.f, m_tRect.top + 70.f, LARVA_FOOD);
	CCollisionMgr::Get_Instance()->Push_CollisionSlotList(m_vecCraftingSlot.back(), SLOT_CRAFTING);

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_UI");
}
