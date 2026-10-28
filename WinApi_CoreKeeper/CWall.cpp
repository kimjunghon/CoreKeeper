#include "pch.h"
#include "CWall.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CWall_Item.h"
#include "CItem.h"
#include "CSoundMgr.h"

CWall::CWall() :
	m_fBmpPosX(0.f),
	m_fBmpPosY(0.f),
	m_eWallID(WALL_END)
{
}

CWall::~CWall()
{
}

void CWall::Initialize()
{
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_tCollisionInfo.fCX = 40.f;
	m_tCollisionInfo.fCY = 40.f;

	m_tBmpInfo.fCX = 16.f;
	m_tBmpInfo.fCY = 32.f;

	m_fBmpPosX = 0.f;
	m_fBmpPosY = 0.f;

	m_eObjID = OBJ_BUILDING;
	m_eRenderID = RENDER_OBJECT;
	m_eWallID = WALL_NULL;

	m_iMaxHp = 0;
	m_iHp = m_iMaxHp;

	m_bHit = false;
	m_dwHitTime = GetTickCount();
	m_dwHitDelay = 400;

	m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"");
	__super::Create_CollisionRect();
}

int CWall::Update()
{
	if (!m_bHit && m_dwHitTime + m_dwHitDelay < GetTickCount())
	{
		m_bHit = true;
	}

	__super::Update_Rect();
	__super::Update_CollisionRect();
	return OBJ_NOEVENT;
}

void CWall::Late_Update()
{
}

void CWall::Render(HDC _hDC)
{
	if(m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();


		if (m_eWallID != WALL_END && m_eWallID != WALL_NULL)
		{
			//GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)(m_tRect.top-40.f) - iCameraY, (int)m_tInfo.fCX, (int)(m_tInfo.fCY + 40.f),
			//	m_hObjDC, (int)m_fBmpPosX, (int)m_fBmpPosY, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));	
			BitBlt(_hDC, (int)m_tRect.left - iCameraX, (int)(m_tRect.top -40.f) - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY + 40.f,
				m_hObjDC, (int)m_fBmpPosX, (int)m_fBmpPosY, SRCCOPY);
		}

#ifdef _DEBUG
	/*	float fCenterX = m_tInfo.fX - iCameraX + m_fDistanceX;
		float fCenterY = m_tInfo.fY - iCameraY + m_fDistanceY;

		float	fRadiusX = m_tCollisionInfo.fCX * 0.5f;
		float	fRadiusY = m_tCollisionInfo.fCY * 0.5f;

		RECT rc = { fCenterX - fRadiusX, fCenterY - fRadiusY,
			fCenterX + fRadiusX, fCenterY + fRadiusY };

		HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));

		FrameRect(_hDC, &rc, hBrush);

		DeleteObject(hBrush);*/
#endif
	}
}

void CWall::Release()
{
}

void CWall::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{
	if (!m_bHit)
		return;

	m_iHp -= (int)_fAtkDmg;

	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(m_iHp <= 0 ? L"Wall_Destroy.wav" : L"Wall_Damage.wav", SOUND_EFFECT, g_fEffectVolume);

	if (m_iHp <= 0)
	{
		// 벽 종류별로 드롭할 벽 아이템
		int iDropItemCode = -1;
		switch (m_eWallID)
		{
		case WALL_TURF:			iDropItemCode = TURF_WALL;	break;
		case WALL_DIRT:			iDropItemCode = DIRT_WALL;	break;
		case WALL_CLAY:
		case WALL_CLAY_IRON:	iDropItemCode = CLAY_WALL;	break;
		default:				break;
		}

		if (iDropItemCode != -1)
		{
			CWall_Item* pWall_Item = static_cast<CWall_Item*>(CAbstractFactory<CWall_Item>::Create(m_tInfo.fX, m_tInfo.fY - 15.f));
			pWall_Item->Set_WallItem(iDropItemCode);
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pWall_Item);
		}

		// 철광석이 박힌 벽은 철광석도 함께 드롭
		if (m_eWallID == WALL_CLAY_IRON)
		{
			CItem* pIron = static_cast<CItem*>(CAbstractFactory<CItem>::Create(m_tInfo.fX + 10.f, m_tInfo.fY - 15.f));
			pIron->Set_Item(IRON_STONE);
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pIron);
		}

		Set_WallID(WALL_NULL);
	}

}



void CWall::Set_WallID(WALLID _eWallID)
{
	m_eWallID = _eWallID;
	switch (m_eWallID)
	{
	case WALL_TURF:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Turf_Wall");
		m_iMaxHp = 30;
		m_iHp = m_iMaxHp;
		break;
	case WALL_DIRT:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Dirt_Wall");
		m_iMaxHp = 60;
		m_iHp = m_iMaxHp;
		break;
	case WALL_CLAY:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Clay_Wall");
		m_iMaxHp = 100;
		m_iHp = m_iMaxHp;
		break;
	case WALL_CLAY_IRON:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Clay_Wall_Iron");
		m_iMaxHp = 100;
		m_iHp = m_iMaxHp;
		break;
	default:
		break;
	}
	m_bHit = false;
	m_dwHitTime = GetTickCount();

}

