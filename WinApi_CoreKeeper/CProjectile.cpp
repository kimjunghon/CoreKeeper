#include "pch.h"
#include "CProjectile.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"

CProjectile::CProjectile()
{
}

CProjectile::~CProjectile()
{
}

void CProjectile::Initialize()
{
	m_tBmpInfo.fCX = 10.f;
	m_tBmpInfo.fCY = 10.f;

	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;
	m_tCollisionInfo.fCX = 10.f;
	m_tCollisionInfo.fCY = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 75;

	m_eDir = DIR_END;
	m_eObjID = OBJ_PROJECTILE;
	m_eRenderID = RENDER_OBJECT;

	m_fSpeed = 5.f;
	m_fMass = 0.f;

	m_bAttack = true;

	Create_CollisionRect();
}

int CProjectile::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    Move();

    __super::Update_Rect();
	__super::Update_CollisionRect();

    return OBJ_NOEVENT;
}

void CProjectile::Late_Update()
{
	Set_Bmp();
    Move_Frame();
	__super::Check_Render();
}

void CProjectile::Render(HDC _hDC)
{
	if(m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CProjectile::Release()
{
}

void CProjectile::Set_Bmp()
{
	if (m_fAngle >= -22.5f && m_fAngle < 22.5f)
	{
		m_hObjDC = m_hRight;
		m_tFrame.iMotion = 1;
	}
	else if (m_fAngle >= 22.5f && m_fAngle < 67.5f)
	{
		m_hObjDC = m_hRight;
		m_tFrame.iMotion = 3;
	}
	else if (m_fAngle >= 67.5f && m_fAngle < 112.5f)
	{
		m_hObjDC = m_hRight;
		m_tFrame.iMotion = 2;
	}
	else if (m_fAngle >= 112.f && m_fAngle < 157.5f)
	{
		m_hObjDC = m_hLeft;
		m_tFrame.iMotion = 3;
	}
	else if (m_fAngle >= 157.5f || m_fAngle < (-157.5f))
	{
		m_hObjDC = m_hLeft;
		m_tFrame.iMotion = 1;
	}
	else if (m_fAngle >=  (-157.5f) && m_fAngle < -112.5f)
	{
		m_hObjDC = m_hLeft;
		m_tFrame.iMotion = 4;
	}
	else if (m_fAngle >= -112.5f && m_fAngle < -67.5f)
	{
		m_hObjDC = m_hRight;
		m_tFrame.iMotion = 0;
	}
	else if (m_fAngle >= -67.5f && m_fAngle < -22.5f)
	{
		m_hObjDC = m_hRight;
		m_tFrame.iMotion = 4;
	}

}

void CProjectile::Set_Projectile(int _iItemCode)
{
	switch (_iItemCode)
	{
	case WOOD_BOW:
		m_hLeft = CBmpMgr::Get_Instance()->Find_Image(L"wood_projectile_L");
		m_hRight = CBmpMgr::Get_Instance()->Find_Image(L"wood_projectile_R");
		break;
	case IRON_BOW:
		m_hLeft = CBmpMgr::Get_Instance()->Find_Image(L"iron_projectile_L");
		m_hRight = CBmpMgr::Get_Instance()->Find_Image(L"iron_projectile_R");
		break;
	}
}

void CProjectile::Move()
{
    m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
    m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	if (m_tInfo.fX > iCameraX + WINCX || m_tInfo.fX < iCameraX || m_tInfo.fY >iCameraY +  WINCY  || m_tInfo.fY < iCameraY)
		m_bDead = true;
}

void CProjectile::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
		}
		m_tFrame.dwTime = GetTickCount();
	}
}

