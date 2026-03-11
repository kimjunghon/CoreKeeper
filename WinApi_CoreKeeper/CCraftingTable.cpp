#include "pch.h"
#include "CCraftingTable.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CPlayer.h"

CCraftingTable::CCraftingTable() :
	m_bUiOpen(false)
{
}

CCraftingTable::~CCraftingTable()
{
	Release();
}

void CCraftingTable::Initialize()
{
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_tBmpInfo.fCX = 48.f;
	m_tBmpInfo.fCY = 48.f;

	m_eObjID = OBJ_BUILDING;
	m_eRenderID = RENDER_OBJECT;
	
	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"CraftingTable");
//	m_hSlotDC;

	m_tCollisionInfo.fCX = 40.f;
	m_tCollisionInfo.fCY = 40.f;

	__super::Create_CollisionRect();
}

int CCraftingTable::Update()
{
	__super::Update_Rect();
	__super::Update_CollisionRect();

	if (Check_CanOpen() && !m_bUiOpen)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('E'))
		{
			CUiMgr::Get_Instance()->CraftingOpen();
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(true);
			m_bUiOpen = true;
		}
	}
	else if (Check_CanOpen() && m_bUiOpen)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('E'))
		{
			CUiMgr::Get_Instance()->CraftingClose();
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(false);
			m_bUiOpen = false;
		}
	}
	else if(!Check_CanOpen() && m_bUiOpen)
	{
		CUiMgr::Get_Instance()->CraftingClose();
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(false);
		m_bUiOpen = false;
	}

	return OBJ_NOEVENT;
}

void CCraftingTable::Late_Update()
{
	__super::Check_Render();
}

void CCraftingTable::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hObjDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

#ifdef _DEBUG
	float fCenterX = m_tInfo.fX - iCameraX + m_fDistanceX;
	float fCenterY = m_tInfo.fY - iCameraY + m_fDistanceY;

	float	fRadiusX = m_tCollisionInfo.fCX * 0.5f;
	float	fRadiusY = m_tCollisionInfo.fCY * 0.5f;

	RECT rc = { fCenterX - fRadiusX, fCenterY - fRadiusY,
		fCenterX + fRadiusX, fCenterY + fRadiusY };

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));

	FrameRect(_hDC, &rc, hBrush);

	DeleteObject(hBrush);
#endif
}

void CCraftingTable::Release()
{
}

void CCraftingTable::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{

}

bool CCraftingTable::Check_CanOpen()
{
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();

	int iX = (int)m_tInfo.fX / TILECX;
	int iY = (int)m_tInfo.fY / TILECY;

	int iIndex = iX + (iY * TILEX);

	int iPlayerX = (int)pPlayer->Get_Info().fX / TILECX;
	int iPlayerY = (int)pPlayer->Get_Info().fY / TILECY;

	int iPlayerIndex = iPlayerX + (iPlayerY * TILEX);

	if (abs(iX - iPlayerX) > 1 || abs(iY - iPlayerY) > 1)
		return false;
	else
		return true;

}