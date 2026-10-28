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

	// 화면 범위가 맵 끝을 넘으면 다음 줄 타일까지 처리하지 않도록 제한
	if (iMaxX > TILEX)
		iMaxX = TILEX;
	if (iMaxY > TILEY)
		iMaxY = TILEY;

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

	// 화면 범위가 맵 끝을 넘으면 다음 줄 타일까지 처리하지 않도록 제한
	if (iMaxX > TILEX)
		iMaxX = TILEX;
	if (iMaxY > TILEY)
		iMaxY = TILEY;

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

	// 화면 범위가 맵 끝을 넘으면 다음 줄 타일까지 처리하지 않도록 제한
	if (iMaxX > TILEX)
		iMaxX = TILEX;
	if (iMaxY > TILEY)
		iMaxY = TILEY;

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

	m_vecTile[iIndex]->Set_TileID(_eTileID);
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

	// 하, 우, 상, 좌
	static const int dX[4] = { 0, 1, 0, -1 };
	static const int dY[4] = { 1, 0, -1, 0 };

	// 이웃 타일 조합(하 1 / 우 2 / 상 4 / 좌 8)별 타일 스프라이트 위치
	static const POINT ptTile[16] =
	{
		{ 120,   0 },	//  0 : 없음
		{  40, 120 },	//  1 : 하
		{   0, 120 },	//  2 : 우
		{   0,   0 },	//  3 : 하+우
		{ 120, 120 },	//  4 : 상
		{ 120,  80 },	//  5 : 하+상
		{   0,  80 },	//  6 : 우+상
		{   0,  40 },	//  7 : 하+우+상
		{  80, 120 },	//  8 : 좌
		{  80,   0 },	//  9 : 하+좌
		{ 120,  40 },	// 10 : 우+좌
		{  40,   0 },	// 11 : 하+우+좌
		{  80,  80 },	// 12 : 상+좌
		{  80,  40 },	// 13 : 하+상+좌
		{  40,  80 },	// 14 : 우+상+좌
		{  40,  40 },	// 15 : 전체
	};

	int iX = _iIndex % TILEX;
	int iY = _iIndex / TILEX;
	int iMask = 0;

	for (int iDir = 0; iDir < 4; ++iDir)
	{
		int iNearX = iX + dX[iDir];
		int iNearY = iY + dY[iDir];

		// 맵 밖(좌우 끝에서 다음 줄로 넘어가는 경우 포함)은 빈 칸으로 취급
		if (iNearX < 0 || iNearX >= TILEX || iNearY < 0 || iNearY >= TILEY)
			continue;

		size_t iNearIndex = (size_t)(iNearX + iNearY * TILEX);
		if (iNearIndex >= m_vecTile.size())
			continue;

		TILEID eNearID = m_vecTile[iNearIndex]->Get_TileID();
		if (eNearID != TILE_NULL && eNearID != TILE_EDIT)
			iMask |= (1 << iDir);
	}

	// 아래쪽에 타일이 없으면 타일 옆면(Under)을 그림
	m_vecTile[_iIndex]->Set_Under(!(iMask & 1));
	m_vecTile[_iIndex]->Set_BmpPos((float)ptTile[iMask].x, (float)ptTile[iMask].y);
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
