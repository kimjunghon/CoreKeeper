#include "pch.h"
#include "CBow.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CCameraMgr.h"
#include "CProjectile.h"
#include "CSoundMgr.h"

CBow::CBow() :
	m_eWeaponGrade(WG_END),
	m_hLeftDC(nullptr),
	m_hRightDC(nullptr)
{
}

CBow::~CBow()
{
	Release();
}

void CBow::Initialize()
{
	m_tBmpInfo.fCX = 40.f;
	m_tBmpInfo.fCY = 40.f;

	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 75;

	m_eObjID = OBJ_ITEM;
	m_eRenderID = RENDER_OBJECT;

	m_eItemClass = IC_BOW;

	m_iCount = 1;
	m_iMaxCount = 1;

//	Create_CollisionRect();
}

int CBow::Update()
{
	if (!m_bEquip && !m_bInventory)
	{
		__super::Update_Rect();
	//	__super::Update_CollisionRect();
	}

	return OBJ_NOEVENT;
}

void CBow::Late_Update()
{
	if (m_bInventory)
	{
		m_bRender = false;

		if (m_bEquip)
		{
			Set_Bmp();
			Move_Frame();
			m_bRender = true;
		}
	}
	else
		__super::Check_Render();
}

void CBow::Render(HDC _hDC)
{
	if (m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CBow::Use()
{
	switch (m_eDir)
	{
	case DIR_UP:
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 3;
		break;
	case DIR_DOWN:
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 1;
		break;
	case DIR_LEFT:
		m_tFrame.iFrameStart = 4;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iMotion = 2;
		break;
	case DIR_RIGHT:
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 2;
		break;
	}
	
	m_bAttack = true;

	CObj* pProjectile = nullptr;

	switch (m_eWeaponGrade)
	{
	case WG_WOOD:
		pProjectile = CAbstractFactory<CProjectile>::Create(m_tInfo.fX, m_tInfo.fY, m_fAngle);
		pProjectile->Set_AtkPower(CObjMgr::Get_Instance()->Get_Player()->Get_AtkPower());
		dynamic_cast<CProjectile*>(pProjectile)->Set_Projectile(m_iItemCode);
		break;
	case WG_IRON:
		pProjectile = CAbstractFactory<CProjectile>::Create(m_tInfo.fX, m_tInfo.fY, m_fAngle);
		pProjectile->Set_AtkPower(CObjMgr::Get_Instance()->Get_Player()->Get_AtkPower());
		dynamic_cast<CProjectile*>(pProjectile)->Set_Projectile(m_iItemCode);
		break;
	}
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_PROJECTILE, pProjectile);

	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"Bow_Use.wav", SOUND_EFFECT, 0.8f);
}

void CBow::Move_Frame()
{
	if (m_bAttack)
	{
		if (m_eDir == DIR_LEFT)
		{
			if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
			{
				--m_tFrame.iFrameStart;

				if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
				{
					m_bAttack = false;
				}
				m_tFrame.dwTime = GetTickCount();
			}
		}
		else
		{
			if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
			{
				++m_tFrame.iFrameStart;

				if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				{
					m_bAttack = false;
				}

				m_tFrame.dwTime = GetTickCount();
			}
		}
	}	
	else
	{
		m_tFrame.iMotion = 0;

		switch (m_eDir)
		{
		case DIR_UP:
			m_tFrame.iFrameStart = 3;
			m_tFrame.iFrameEnd = 3;
			break;
		case DIR_DOWN:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 1;
			break;
		case DIR_LEFT:
			m_tFrame.iFrameStart = 3;
			m_tFrame.iFrameEnd = 3;
			break;
		case DIR_RIGHT:
			m_tFrame.iFrameStart = 2;
			m_tFrame.iFrameEnd = 2;
			break;
		}
	}
}
void CBow::Set_BowItem(int _iItemCode)
{
	m_iItemCode = _iItemCode;

	switch (m_iItemCode)
	{
	case WOOD_BOW:
		m_fAtkPower = 10.f;
		m_fMiningPower = 0.f;
		m_fMass = 2.f;
		m_eWeaponGrade = WG_WOOD;
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"wood_bow_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"wood_bow_R");
		m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"wood_bow_R");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"WoodBow_Info");
		break;
	case IRON_BOW:
		m_eWeaponGrade = WG_IRON;
		m_fAtkPower = 25.f;
		m_fMiningPower = 0.f;
		m_fMass = 4.f;
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"iron_bow_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"iron_bow_R");
		m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"iron_bow_R");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronBow_Info");
		break;
	}

}
void CBow::Set_Bmp()
{
	if (m_eDir == DIR_LEFT)
		m_hObjDC = m_hLeftDC;
	else
		m_hObjDC = m_hRightDC;
}

void CBow::Release()
{
}
