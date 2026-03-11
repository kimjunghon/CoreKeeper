#include "pch.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"

#include "CScrollMgr.h"
#include "CCameraMgr.h"


CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(TILECX * j) + (TILECX >> 1);
			float fY = (float)(TILECY * i) + (TILECY >> 1);
			
			CTile* pTile = CTileFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pTile);
		}
	}
}

void CTileMgr::Update()
{
	int iCameraX = abs((int)CCameraMgr::Get_Instance()->Get_RenderX() / TILECX);
	int iCameraY = abs((int)CCameraMgr::Get_Instance()->Get_RenderY() / TILECY);

	int iMinX = iCameraX;
	int iMinY = iCameraY - 1;

	int	iMaxX = iCameraX + (WINCX / TILECX) + 2;
	int	iMaxY = iCameraY + (WINCY / TILECY) + 2;

	for (int i = iMinY; i < iMaxY; ++i)
	{
		for (int j = iMinX; j < iMaxX; ++j)
		{
			int iIndex = j + (i * TILEX);
			
			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Update();
		}
	}
}

void CTileMgr::Late_Update()
{

	int iCameraX = abs((int)CCameraMgr::Get_Instance()->Get_RenderX() / TILECX);
	int iCameraY = abs((int)CCameraMgr::Get_Instance()->Get_RenderY() / TILECY);

	int iMinX = iCameraX;
	int iMinY = iCameraY-1;

	int	iMaxX = iCameraX + (WINCX / TILECX) + 2;
	int	iMaxY = iCameraY + (WINCY / TILECY) + 2;

	for (int i = iMinY; i < iMaxY; ++i)
	{
		for (int j = iMinX; j < iMaxX; ++j)
		{
			int iIndex = j + (i * TILEX);

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;
			
			Move_Frame(iIndex);

			m_vecTile[iIndex]->Late_Update();
		}
	}
}

void CTileMgr::Render(HDC _hDC)
{
	//int	iScrollIdxX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	//int	iScrollIdxY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iCameraX = abs((int)CCameraMgr::Get_Instance()->Get_RenderX() / TILECX);
	int iCameraY = abs((int)CCameraMgr::Get_Instance()->Get_RenderY() / TILECY);

	int iMinX = iCameraX;
	int iMinY = iCameraY - 1;

	int	iMaxX = iCameraX + (WINCX / TILECX) + 2;
	int	iMaxY = iCameraY + (WINCY / TILECY) + 2;

	for (int i = iMinY; i < iMaxY; ++i)
	{
		for (int j = iMinX; j < iMaxX; ++j)
		{
			int iIndex = j + (i*TILEX);

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Render(_hDC);
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CTile*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Change_Tile(POINT _pt, TILEID _eTileID)
{
	int	x = _pt.x / TILECX;
	int	y = _pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_TileID(_eTileID);
}

bool CTileMgr::Check_Tile(POINT _pt)
{
	int	x = _pt.x / TILECX;
	int	y = _pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size() || m_vecTile[iIndex]->Get_TileID() == TILE_EDIT)
		return false;


	return true;
}

const INFO* CTileMgr::Get_Tile_Info(POINT _pt)
{
	int	x = _pt.x / TILECX;
	int	y = _pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return nullptr;

	return m_vecTile[iIndex]->Get_Info();
}

const RECT* CTileMgr::Get_Tile_Rect(POINT _pt)
{
	int	x = _pt.x / TILECX;
	int	y = _pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return nullptr;

	return m_vecTile[iIndex]->Get_Rect();
}


void CTileMgr::Move_Frame(int _iIndex)
{
	if (m_vecTile[_iIndex]->Get_TileID() == TILE_NULL || m_vecTile[_iIndex]->Get_TileID() == TILE_EDIT)
		return;

	int dX[4] = { 0, 1, 0, -1 };
	int dY[4] = { 1, 0, -1, 0 };
	int dZ[4] = { 0,0,0,0}; //го, ©Л, ╩С, аб
	int Dir = 0;
	int iIndex = 0;

	for (Dir = 0; Dir < 4; Dir++)
	{
		iIndex = _iIndex + dX[Dir] + (dY[Dir] * TILEX);
		
		if (iIndex < 0 || iIndex >= TILEX * TILEY)
			continue;

		else if (m_vecTile[iIndex]->Get_TileID() != TILE_NULL && m_vecTile[iIndex]->Get_TileID() != TILE_EDIT)
		{
			dZ[Dir] = 1;
		}
	}

	if (dZ[0] == 0)
	{
		m_vecTile[_iIndex]->Set_Under(true);
	}
	else
	{
		m_vecTile[_iIndex]->Set_Under(false);
	}

	if (dZ[0] == 1)
	{
		if (dZ[1] == 1)
		{
			if (dZ[2] == 1)
			{
				if (dZ[3] == 1)
				{
					m_vecTile[_iIndex]->Set_BmpPos(40.f, 40.f);
				}
				else
				{
					m_vecTile[_iIndex]->Set_BmpPos(0.f, 40.f);
				}
			}
			else if (dZ[3] == 1)
			{
				m_vecTile[_iIndex]->Set_BmpPos(40, 0.f);
			}
			else
			{
				m_vecTile[_iIndex]->Set_BmpPos(0.f, 0.f);
			}
		}
		else if (dZ[2] == 1)
		{
			if (dZ[3] == 1)
			{
				m_vecTile[_iIndex]->Set_BmpPos(80.f, 40.f);
			}
			else
			{
				m_vecTile[_iIndex]->Set_BmpPos(120.f, 80.f);
			}
		}
		else if (dZ[3] == 1)
		{
			m_vecTile[_iIndex]->Set_BmpPos(80.f, 0.f);
		}
		else
		{
			m_vecTile[_iIndex]->Set_BmpPos(40.f, 120.f);
		}
	}
	else if (dZ[1] == 1)
	{
		if (dZ[2] == 1)
		{
			if (dZ[3] == 1)
			{
				m_vecTile[_iIndex]->Set_BmpPos(40.f, 80.f);
			}
			else
			{
				m_vecTile[_iIndex]->Set_BmpPos(0.f, 80.f);
			}
		}
		else if (dZ[3] == 1)
		{
			m_vecTile[_iIndex]->Set_BmpPos(120.f, 40.f);
		}
		else
		{
			m_vecTile[_iIndex]->Set_BmpPos(0.f, 120.f);
		}
	}
	else if(dZ[2] == 1)
	{
		if (dZ[3] == 1)
		{
			m_vecTile[_iIndex]->Set_BmpPos(80.f, 80.f);
		}
		else
		{
			m_vecTile[_iIndex]->Set_BmpPos(120.f, 120.f);
		}
	}
	else if (dZ[3] == 1)
	{
		m_vecTile[_iIndex]->Set_BmpPos(80.f, 120.f);
	}
	else
	{
		m_vecTile[_iIndex]->Set_BmpPos(120.f, 0.f);
	}
}


void CTileMgr::Save_Tile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int iTileID(0);
	DWORD	dwByte(0);

	for (auto& pTile : m_vecTile)
	{
		iTileID = pTile->Get_TileID();
		if (iTileID == TILE_EDIT)
			iTileID = TILE_NULL;

		WriteFile(hFile, pTile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iTileID , sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);

}

void CTileMgr::Load_Tile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat",
		GENERIC_READ,
		NULL, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int		iTileID(0);
	INFO	tInfo{};
	DWORD	dwByte(0);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iTileID, sizeof(int), &dwByte, NULL);

		if (dwByte == 0)
			break;

		CTile* pTile = CTileFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		pTile->Set_TileID((TILEID)iTileID);

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

}
