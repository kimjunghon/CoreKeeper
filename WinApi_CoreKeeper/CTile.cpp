#include "pch.h"
#include "CTile.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CWall.h"
#include "CObjMgr.h"

CTile::CTile() :
	m_eTileID(TILE_END),
	m_hTileDC(nullptr),
	m_hTileUnderDC(nullptr),
	m_bRender(true)
{
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tBmpInfo, sizeof(m_tBmpInfo));
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	m_tBmpInfo.fX = 0.f;
	m_tBmpInfo.fY = 0.f;

	m_eTileID = TILE_EDIT;
	m_hTileDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Edit");
}

void CTile::Update()
{
	Update_Rect();
}

void CTile::Late_Update()
{
	int y = (int)(m_tInfo.fY / TILECY);
	int x =	(int)(m_tInfo.fX / TILECX);
	int iIndex = x + (y * TILEX);

	CObj* pWall = (*(CObjMgr::Get_Instance()->Get_VecWall()))[iIndex];

	if (dynamic_cast<CWall*>(pWall)->Get_WallID() == WALL_NULL)
		m_bRender = true;
	else
		m_bRender = false;
}

void CTile::Render(HDC _hDC)
{
	if (m_eTileID == TILE_NULL)
		return;

		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		int	x = (int)m_tInfo.fX / TILECX;
		int	y = (int)m_tInfo.fY / TILECY;

		int		iCompIndex = y * TILEX + x;


		if (0 > iCompIndex || (size_t)iCompIndex >= CObjMgr::Get_Instance()->Get_VecWall()->size())
			return;

		if(dynamic_cast<CWall*>((*CObjMgr::Get_Instance()->Get_VecWall())[iCompIndex])->Get_WallID() == WALL_NULL)
		{
			if (m_bRender)
			{
				BitBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
					m_hTileDC, (int)m_tBmpInfo.fX, (int)m_tBmpInfo.fY, SRCCOPY);
				//GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
				//	m_hTileDC, (int)m_tBmpInfo.fX, (int)m_tBmpInfo.fY, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
			}
	}

	if (m_bUnder && m_hTileUnderDC)
	{
		BitBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.bottom - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hTileUnderDC, 0, 0, SRCCOPY);
		//GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.bottom - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		//	, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}

}

void CTile::Release()
{

}

void CTile::Set_TileID(TILEID _eTileID)
{
	m_eTileID = _eTileID;
	switch (m_eTileID)
	{
		case TILE_TURF:
			m_hTileDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Turf");
			m_hTileUnderDC = CBmpMgr::Get_Instance()->Find_Image(L"Down_Tile_Turf");
			m_tBmpInfo.fX = 120.f;
			m_tBmpInfo.fY = 0.f;
			break;
		case TILE_DIRT:
			m_hTileDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Dirt");
			m_hTileUnderDC = CBmpMgr::Get_Instance()->Find_Image(L"Down_Tile_Dirt");
			m_tBmpInfo.fX = 120.f;
			m_tBmpInfo.fY = 0.f;
			break;
		case TILE_CLAY:
			m_hTileDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Clay");
			m_hTileUnderDC = CBmpMgr::Get_Instance()->Find_Image(L"Down_Tile_Caly");
			m_tBmpInfo.fX = 120.f;
			m_tBmpInfo.fY = 0.f;
			break;
		case TILE_NULL:
			m_hTileDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Null");
			m_hTileUnderDC = nullptr;
			m_tBmpInfo.fX = 0.f;
			m_tBmpInfo.fY = 0.f;
			break;
		case TILE_EDIT:
			m_hTileDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Edit");
			m_hTileUnderDC = nullptr;
			m_tBmpInfo.fX = 0.f;
			m_tBmpInfo.fY = 0.f;
			break;
	}
}


void CTile::Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}