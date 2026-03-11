#include "pch.h"
#include "CEquip.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CSlot.h"
#include "CObjMgr.h"

CEquip::CEquip()
{
}

CEquip::~CEquip()
{
	Release();
}

void CEquip::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 200.f;
	m_tInfo.fX = 600.f;
	m_tInfo.fY = 150.f;

	m_tBmpInfo.fCX = 548.f;
	m_tBmpInfo.fCY = 580.f;

	m_eUiID = UI_EQUIP;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"EquipUi");

	__super::Update_Rect();

	m_pSlot[0] = CUIFactory<CSlot>::Create(m_tRect.left + 60.f, m_tRect.top + 60.f);
	dynamic_cast<CSlot*>(m_pSlot[0])->Set_SlotID(SLOT_EQUIP_HELM);

	m_pSlot[1] = CUIFactory<CSlot>::Create(m_tRect.left + 60.f, m_tRect.top + 115.f);
	dynamic_cast<CSlot*>(m_pSlot[1])->Set_SlotID(SLOT_EQUIP_CHEST);

	m_pSlot[2] = CUIFactory<CSlot>::Create(m_tRect.left + 60.f, m_tRect.top + 170.f);
	dynamic_cast<CSlot*>(m_pSlot[2])->Set_SlotID(SLOT_EQUIP_PANTS);

}

void CEquip::Update()
{
	if (m_bOpen)
	{
		m_bRender = true;
		m_hPlayerDC = *(CObjMgr::Get_Instance()->Get_Player()->Get_SlotDC());
	}
	else
	{
		m_bRender = false;
	}
	for (auto& pSlot : m_pSlot)
	{
		pSlot->Update();
		pSlot->Set_Render(m_bRender);
	}
}

void CEquip::Late_Update()
{
	for (auto& pSlot : m_pSlot)
	{
		pSlot->Late_Update();
	}
}

void CEquip::Render(HDC _hDC)
{
	if (m_bRender)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

		GdiTransparentBlt(_hDC, 590, 130, 70, 70,
			m_hPlayerDC, 0, 0, 26, 26, RGB(255, 255, 255));




		for (auto& pSlot : m_pSlot)
		{
			pSlot->Render(_hDC);
		}
	}
}

void CEquip::Release()
{
	for(int i=0; i<3; i++)
	{
		Safe_Delete<CUi*>(m_pSlot[i]);
	}
}
