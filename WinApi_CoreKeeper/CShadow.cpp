#include "pch.h"
#include "CShadow.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"

#include "CTileMgr.h"

CShadow::CShadow()
{
}

CShadow::~CShadow()
{
}

void CShadow::Initialize()
{
	m_tInfo.fCX = 800;
	m_tInfo.fCY = 600;

	m_tBmpInfo.fCX = 800.f;
	m_tBmpInfo.fCY = 600.f;
	m_tBmpInfo.fX = 400.f;
	m_tBmpInfo.fY = 300.f;

	m_hShadowDC = CBmpMgr::Get_Instance()->Find_Image(L"Shadow");

}

void CShadow::Update()
{
	Update_Rect();
}

void CShadow::Late_Update()
{
}

void CShadow::Render(HDC _hDC)
{
	int iPlayerX = (int)CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;
	int iPlayerY = (int)CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY;
	BLENDFUNCTION bf = { 0,0,150,0 };
	//GdiTransparentBlt(MemDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	m_hShadowDC, (int)m_tBmpInfo.fX, (int)m_tBmpInfo.fY, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255,255,255));

	float fX(0.f), fY(0.f);
	if (iPlayerY <= 300)
	{
		fY = m_tBmpInfo.fY + (float)(300 - iPlayerY);
	}
	else if (iPlayerY >= 1500)
	{
		fY = m_tBmpInfo.fY - (float)(iPlayerY - 1500);
	}
	else
	{
		fY = m_tBmpInfo.fY;
	}

	if (iPlayerX <= 400)
	{
		fX = m_tBmpInfo.fX + (float)(400 - iPlayerX);
	}
	else if (iPlayerX >= 2000)
	{
		fX = m_tBmpInfo.fX - (float)(iPlayerX - 2000);
	}
	else
	{
		fX = m_tBmpInfo.fX;
	}

	GdiAlphaBlend(_hDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hShadowDC, (int)fX, (int)fY, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, bf);
}

void CShadow::Release()
{
}

void CShadow::Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}
