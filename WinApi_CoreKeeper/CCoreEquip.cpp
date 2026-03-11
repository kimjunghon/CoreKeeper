#include "pch.h"
#include "CCoreEquip.h"
#include "CAbstractFactory.h"
#include "CSlot.h"
#include "CCollisionMgr.h"
#include "CBmpMgr.h"

CCoreEquip::CCoreEquip() :
	m_pSlot(nullptr)
{
}

CCoreEquip::~CCoreEquip()
{
	Release();
}

void CCoreEquip::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 150.f;
	m_tInfo.fX = 400.f;
	m_tInfo.fY = 200.f;

	m_tBmpInfo.fCX = 401.f;
	m_tBmpInfo.fCY = 278.f;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Core_Equip");
	__super::Update_Rect();
}

void CCoreEquip::Update()
{
	if (m_bOpen)
	{
		m_bRender = true;
	}
	else
	{
		m_bRender = false;
	}

	m_pSlot->Update();
	m_pSlot->Set_Render(m_bRender);
}

void CCoreEquip::Late_Update()
{
	m_pSlot->Late_Update();
}

void CCoreEquip::Render(HDC _hDC)
{
	if (m_bRender)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

		m_pSlot->Render(_hDC);
	}
	
}

void CCoreEquip::Release()
{
	Safe_Delete<CUi*>(m_pSlot);
}

bool CCoreEquip::IsCoreEquip()
{
	if (dynamic_cast<CSlot*>(m_pSlot)->Get_Item())
		return true;
	else
		return false;
}

void CCoreEquip::Set_CoreEquip(int _iItemCode)
{
	switch (_iItemCode)
	{
	case SLIME_CORE:
		m_eUiID = UI_SLIE_CORE;
		m_pSlot = CUIFactory<CSlot>::Create(m_tInfo.fX, m_tInfo.fY);
		dynamic_cast<CSlot*>(m_pSlot)->Set_SlotID(SLOT_SLIME_CORE);
		CCollisionMgr::Get_Instance()->Push_CollisionSlotList(m_pSlot, SLOT_SLIME_CORE);
		break;
	case HIVE_CORE:
		m_eUiID = UI_HIVE_CORE;
		m_pSlot = CUIFactory<CSlot>::Create(m_tInfo.fX, m_tInfo.fY);
		dynamic_cast<CSlot*>(m_pSlot)->Set_SlotID(SLOT_HIVE_CORE);
		CCollisionMgr::Get_Instance()->Push_CollisionSlotList(m_pSlot, SLOT_HIVE_CORE);
		break;
	}
}
