#include "pch.h"
#include "CMinimap.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"

CMinimap::CMinimap() :
	m_bFull(false)
{
	m_vecMinimapTile.reserve(TILEX * TILEY);
}

CMinimap::~CMinimap()
{
	Release();
}

void CMinimap::Initialize()
{
	for (auto& vecTile : *(CTileMgr::Get_Instance()->Get_VecTile()))
	{
		MINIMAPINFO miniInfo = { false, TILE_NULL };
		miniInfo.eTileID = vecTile->Get_TileID();

		m_vecMinimapTile.push_back(miniInfo);
	}

	m_tInfo.fX = 700.f;
	m_tInfo.fY = 75.f;
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 150.f;
	m_tBmpInfo.fCX = 200.f;
	m_tBmpInfo.fCY = 150.f;

	m_tMinimapTileInfo.fCX = 10.f;
	m_tMinimapTileInfo.fCY = 10.f;

	m_eUiID = UI_MINIMAP;

	m_bOpen = true;
	m_bRender = true;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Background");
	m_hMinimapTile[0] = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Turf");
	m_hMinimapTile[1] = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Dirt");
	m_hMinimapTile[2] = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Clay");
	m_hMinimapTile[3] = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Null");

	m_hMinimapPlayer = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Player");

	__super::Update_Rect();

}

void CMinimap::Update()
{
	if (m_bOpen)
	{
		m_bRender = true;
	}
	else
	{
		m_bRender = false;
	}

	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();

	if (!pPlayer || !pPlayer->IsRender())
		return;

	const INFO& tPlayerInfo = pPlayer->Get_Info();
	
	int	iMinX = (int)(tPlayerInfo.fX / TILECX) -1;
	int	iMinY = ((int)(tPlayerInfo.fY +32) / TILECY) -1;

	int	iMaxX = (int)(tPlayerInfo.fX / TILECX) + 2;
	int	iMaxY = ((int)(tPlayerInfo.fY +32) / TILECY) + 2;

	for (int i = iMinY; i < iMaxY; ++i)
	{
		for (int j = iMinX; j < iMaxX; ++j)
		{
			int iIndex = j + (i * TILEX);

			if (0 > iIndex || (size_t)iIndex >= m_vecMinimapTile.size())
				continue;

			m_vecMinimapTile[iIndex].bPassed = true;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('M'))
	{
		if (m_bFull)
		{
			m_bFull = false;
			m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Background");
		}
		else
		{
			m_bFull = true;
			m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Minimap_Full_Background");
		}
		CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
		CSoundMgr::Get_Instance()->PlaySound(L"Minimap.wav", SOUND_UI, 0.8f);
	}

}

void CMinimap::Late_Update()
{
}

void CMinimap::Render(HDC _hDC)
{
	if (!m_bRender)
		return;

	int iMinX(0);
	int iMinY(0);

	int	iMaxX(0);
	int	iMaxY(0);

	int iDistanceX(0);
	int iDistanceY(0);

	if (m_bFull)
	{
		m_tInfo.fX = 400.f;
		m_tInfo.fY = 300.f;
		m_tInfo.fCX = 700.f;
		m_tInfo.fCY = 470.f;
		m_tBmpInfo.fCX = 700.f;
		m_tBmpInfo.fCY = 470.f;
		iMinX = 0;
		iMinY = 0;

		iMaxX = TILEX;
		iMaxY = TILEY;

		iDistanceX = 50;
		iDistanceY = 10;

		__super::Update_Rect();
	}
	else
	{
		m_tInfo.fX = 690.f;
		m_tInfo.fY = 85.f;
		m_tInfo.fCX = 210.f;
		m_tInfo.fCY = 160.f;

		m_tBmpInfo.fCX = 200.f;
		m_tBmpInfo.fCY = 150.f;

		iDistanceX = 5;
		iDistanceY = 5;

		int iCameraX = abs((int)CCameraMgr::Get_Instance()->Get_RenderX() / TILECX);
		int iCameraY = abs((int)CCameraMgr::Get_Instance()->Get_RenderY() / TILECY);

		iMinX = iCameraX;
		iMinY = iCameraY;

		iMaxX = iCameraX + (WINCX / TILECX);
		iMaxY = iCameraY + (WINCY / TILECY);

		__super::Update_Rect();
	}

	GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

	int iCount = 0;
	int jCount = 0;

	for (int i = iMinY; i < iMaxY; ++i)
	{
		jCount = 0;

		for (int j = iMinX; j < iMaxX; ++j)
		{
			int iIndex = j + (i * TILEX);

			if (0 > iIndex || (size_t)iIndex >= m_vecMinimapTile.size())
				continue;

			if(m_vecMinimapTile[iIndex].bPassed)
			{
				BitBlt(_hDC, (int)m_tRect.left + (jCount * (int)m_tMinimapTileInfo.fCX) + iDistanceX, (int)m_tRect.top + (iCount * (int)m_tMinimapTileInfo.fCY) + iDistanceY,
					(int)m_tMinimapTileInfo.fCX, (int)m_tMinimapTileInfo.fCY,
					m_hMinimapTile[m_vecMinimapTile[iIndex].eTileID], 0, 0, SRCCOPY);
			}

			++jCount;
		}
		++iCount;
	}

	//Player
	{
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
		if (!pPlayer || !pPlayer->IsRender())
			return;

		const INFO& tInfo = pPlayer->Get_Info();

		int iPlayerX(0);
		int iPlayerY(0);

		if (m_bFull)
		{
			iPlayerX = (int)tInfo.fX / TILECX;
			iPlayerY = ((int)tInfo.fY) / TILECY;
		}
		else
		{
			int iCameraX = CCameraMgr::Get_Instance()->Get_RenderX();
			int iCameraY = CCameraMgr::Get_Instance()->Get_RenderY();

			iPlayerX = ((int)tInfo.fX - iCameraX) / TILECX;
			iPlayerY = (((int)tInfo.fY +32) - iCameraY) / TILECY;

		}
		int		iCompIndex = (iPlayerY) * TILEX + (iPlayerX);

		if (0 > iCompIndex || (size_t)iCompIndex >= m_vecMinimapTile.size())
			return;

		GdiTransparentBlt(_hDC, (int)m_tRect.left + (iPlayerX * (int)m_tMinimapTileInfo.fCX) + iDistanceX,
			(int)m_tRect.top + (iPlayerY * (int)m_tMinimapTileInfo.fCY) + iDistanceY, 
			(int)m_tMinimapTileInfo.fCX, (int)m_tMinimapTileInfo.fCY,
			m_hMinimapPlayer, 0, 0, m_tMinimapTileInfo.fCX, m_tMinimapTileInfo.fCY, RGB(0, 255, 0));
	}


}

void CMinimap::Release()
{
	m_vecMinimapTile.clear();
	m_vecMinimapTile.shrink_to_fit();
}
