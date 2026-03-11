#include "pch.h"
#include "CHitDamage.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"


CHitDamage::CHitDamage() :
	m_iDamage(0)
{
}

CHitDamage::~CHitDamage()
{
}

void CHitDamage::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 30.f;

	m_dwEffectTime = GetTickCount();
	m_dwEffectDelay = 1000;

	m_eObjID = OBJ_EFFECT;
	m_eRenderID = RENDER_OBJECT;
}

int CHitDamage::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dwEffectTime + m_dwEffectDelay < GetTickCount())
		m_bDead = true;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CHitDamage::Late_Update()
{
}

void CHitDamage::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	if (m_iDamage >= 10)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left + 10 - iCameraX, (int)m_tRect.top - iCameraY, m_tInfo.fCX, m_tInfo.fCY,
			m_hObjDC, (m_iDamage / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

		GdiTransparentBlt(_hDC, (int)m_tRect.left + 20 - iCameraX, (int)m_tRect.top - iCameraY, m_tInfo.fCX, m_tInfo.fCY,
			m_hObjDC, (m_iDamage % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
	}
	else
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left + 10 - iCameraX, (int)m_tRect.top - iCameraY, m_tInfo.fCX, m_tInfo.fCY,
			m_hObjDC, (m_iDamage % 10) * 15, 0, 15, 45, RGB(0, 0, 0));

	}
}

void CHitDamage::Release()
{
}

void CHitDamage::Set_HitDamage(OBJID _eObjID, int _iDamage)
{
	switch (_eObjID)
	{
	case OBJ_PLAYER:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"HitFontNumber");
		break;
	case OBJ_MONSTER:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"FontNumber");
		break;
	}
	m_iDamage = _iDamage;
}
