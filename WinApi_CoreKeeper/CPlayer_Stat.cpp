#include "pch.h"
#include "CPlayer_Stat.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

CPlayer_Stat::CPlayer_Stat()
{
}

CPlayer_Stat::~CPlayer_Stat()
{
}

void CPlayer_Stat::Initialize()
{
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 250.f;
	m_tInfo.fX = 750.f;
	m_tInfo.fY = 200.f;

	m_tBmpInfo.fCX = 100.f;
	m_tBmpInfo.fCY = 250.f;

	m_eUiID = UI_STATE;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Stat");

	__super::Update_Rect();
}

void CPlayer_Stat::Update()
{
	if (m_bOpen)
	{
		m_bRender = true;
	}
	else
	{
		m_bRender = false;
	}
}

void CPlayer_Stat::Late_Update()
{
}

void CPlayer_Stat::Render(HDC _hDC)
{
	if (m_bRender)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));

		int iAtkPower =	(int)CObjMgr::Get_Instance()->Get_Player()->Get_AtkPower();
		int iDefPower = (int)CObjMgr::Get_Instance()->Get_Player()->Get_DefPower();
		int iHp = CObjMgr::Get_Instance()->Get_Player()->Get_Hp();
		int iMaxHp = CObjMgr::Get_Instance()->Get_Player()->Get_MaxHp();
		int iMiningPower = (int)CObjMgr::Get_Instance()->Get_Player()->Get_MiningPower();

		HDC hNumber = CBmpMgr::Get_Instance()->Find_Image(L"FontNumber");

		if (iHp > 1)
		{
			if (iHp >= 100)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 15, (int)m_tRect.top + 50, 5, 15,
					hNumber, (iHp / 100) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 25, (int)m_tRect.top + 50, 5, 15,
					hNumber, ((iHp % 100) / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 35, (int)m_tRect.top + 50, 5, 15,
					hNumber, (iHp % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
			else if (iHp >= 10)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 25, (int)m_tRect.top + 50, 5, 15,
					hNumber, (iHp / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 35, (int)m_tRect.top + 50, 5, 15,
					hNumber, (iHp % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 35, (int)m_tRect.top + 50, 5, 15,
					hNumber, (iHp % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
		}
		if (iMaxHp > 1)
		{
			if (iMaxHp >= 100)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 60, (int)m_tRect.top + 50, 5, 15,
					hNumber, (iMaxHp / 100) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 70, (int)m_tRect.top + 50, 5, 15,
					hNumber, ((iMaxHp %100) / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 80, (int)m_tRect.top + 50, 5, 15,
					hNumber, (iMaxHp % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
		}

		if (iAtkPower > 1)
		{
			if (iAtkPower >= 10)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 40, (int)m_tRect.top + 95, 10, 30,
					hNumber, (iAtkPower / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 50, (int)m_tRect.top + 95, 10, 30,
					hNumber, (iAtkPower % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 45, (int)m_tRect.top + 95, 10, 30,
					hNumber, (iAtkPower % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
		}

		if (iDefPower > 1)
		{

			if (iDefPower >= 10)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 40, (int)m_tRect.top + 150, 10, 30,
					hNumber, (iDefPower / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 50, (int)m_tRect.top + 150, 10, 30,
					hNumber, (iDefPower % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 45, (int)m_tRect.top + 150, 10, 30,
					hNumber, (iDefPower % 10) * 15, 0, 15, 45, RGB(0, 0, 0));

			}
		}

		if (iMiningPower > 1)
		{

			if (iMiningPower >= 10)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 40, (int)m_tRect.top + 205, 10, 30,
					hNumber, (iMiningPower / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 50, (int)m_tRect.top + 205, 10, 30,
					hNumber, (iMiningPower % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left + 45, (int)m_tRect.top + 205, 10, 30,
					hNumber, (iMiningPower % 10) * 15, 0, 15, 45, RGB(0, 0, 0));

			}
		}
	}
}

void CPlayer_Stat::Release()
{
}
