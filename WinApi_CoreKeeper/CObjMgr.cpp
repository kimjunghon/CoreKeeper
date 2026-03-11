#include "pch.h"
#include "CWall.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CTileMgr.h"
#include "CCameraMgr.h"
#include "CSlime.h"
#include "CBoss_Slime.h"
#include "CLarva.h"
#include "CBoss_Hive.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
	m_vecWall.reserve(TILEX * TILEY);
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if (eID >= OBJ_END || eID < 0 || pObj == nullptr)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(TILECX * j) + (TILECX >> 1);
			float fY = (float)(TILECY * i) + (TILECY >> 1);

			CObj* pWall = CAbstractFactory<CWall>::Create(fX, fY, 0.f);
			m_vecWall.push_back(pWall);
		}
	}
}

void CObjMgr::Update()
{
	if(!m_vecWall.empty())
	{
		int iCameraX = abs((int)CCameraMgr::Get_Instance()->Get_RenderX() / TILECX);
		int iCameraY = abs((int)CCameraMgr::Get_Instance()->Get_RenderY() / TILECY);

		int iMinX = iCameraX;
		int iMinY = iCameraY;

		int	iMaxX = iCameraX + (WINCX / TILECX) + 2;
		int	iMaxY = iCameraY + (WINCY / TILECY) + 2;

		for (int i = iMinY; i < iMaxY; ++i)
		{
			for (int j = iMinX; j < iMaxX; ++j)
			{
				int iIndex = j + (i * TILEX);
				if (0 > iIndex || (size_t)iIndex >= m_vecWall.size())
					continue;

				m_vecWall[iIndex]->Update();
			}
		}
	}

	for (unsigned int i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{

			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update()
{

	if (!m_vecWall.empty())
	{
		int iCameraX = abs((int)CCameraMgr::Get_Instance()->Get_RenderX() / TILECX);
		int iCameraY = abs((int)CCameraMgr::Get_Instance()->Get_RenderY() / TILECY);

		int iMinX = iCameraX;
		int iMinY = iCameraY;

		int	iMaxX = iCameraX + (WINCX / TILECX) + 2;
		int	iMaxY = iCameraY + (WINCY / TILECY) + 2;

		for (int i = iMinY; i < iMaxY; ++i)
		{
			for (int j = iMinX; j < iMaxX; ++j)
			{
				int iIndex = j + (i * TILEX);

				if (0 > iIndex || (size_t)iIndex >= m_vecWall.size())
					continue;

				Move_Wall_Roof_Frame(iIndex);

				m_vecWall[iIndex]->Late_Update();

				CWall* pWall = dynamic_cast<CWall*>(m_vecWall[iIndex]);

				if (!pWall || pWall->Get_WallID() == WALL_NULL)
					continue;

				RENDERID eRender = m_vecWall[iIndex]->Get_RenderID();
				m_ObjRenderList[eRender].push_back(m_vecWall[iIndex]);
			}
		}
	}

	for (unsigned int i = 0; i < OBJ_END; ++i)
	{

		for (auto& pObj : m_ObjList[i])
		{
			pObj->Check_Render();
			if(pObj->IsRender())
			{
				pObj->Late_Update();

				RENDERID eRender = pObj->Get_RenderID();
				m_ObjRenderList[eRender].push_back(pObj);
			}
		}
	}

}

void CObjMgr::Render(HDC hDC)
{
	for (unsigned int i = 0; i < RENDER_END; ++i)
	{
		m_ObjRenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
			{
				return pDst->Get_Info().fY < pSrc->Get_Info().fY;
			});


		for (auto& pObj : m_ObjRenderList[i])
			pObj->Render(hDC);
	
		m_ObjRenderList[i].clear();
	}
}

void CObjMgr::Release()
{
	for (unsigned int i = 0; i < OBJ_END; ++i)
	{

		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}

	for_each(m_vecWall.begin(), m_vecWall.end(), Safe_Delete<CObj*>);
	m_vecWall.clear();
	m_vecWall.shrink_to_fit();
}



CObj* CObjMgr::Get_Player()
{
	if (m_ObjList[OBJ_PLAYER].empty())
		return nullptr;

	
	return m_ObjList[OBJ_PLAYER].front();
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete<CObj*>(pObj);

	m_ObjList[eID].clear();
}

void CObjMgr::Change_Wall(POINT _pt, WALLID _eWallID)
{
	int	x = _pt.x / TILECX;
	int	y = _pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecWall.size())
		return;

	dynamic_cast<CWall*>(m_vecWall[iIndex])->Set_WallID(_eWallID);
}

void CObjMgr::Move_Wall_Roof_Frame(int _iIndex)
{
	CWall* pWall = dynamic_cast<CWall*>(m_vecWall[_iIndex]);

	if (!pWall)
		return;

	if (pWall->Get_WallID() == WALL_NULL)
		return;

	int dX[4] = { 0, 1, 0, -1 };
	int dY[4] = { 1, 0, -1, 0 };
	int dZ[4] = { 0,0,0,0 }; //하, 우, 상, 좌
	int Dir = 0;
	int iIndex = 0;

	for (Dir = 0; Dir < 4; Dir++)
	{
		iIndex = _iIndex + dX[Dir] + (dY[Dir] * TILEX);

		if (iIndex < 0 || iIndex >= TILEX * TILEY)
			continue;

		CWall* pTemp = dynamic_cast<CWall*>(m_vecWall[iIndex]);

		if (!pTemp)
			continue;

		if (pTemp->Get_WallID() != WALL_NULL)
		{
			dZ[Dir] = 1;
		}
	}

	if (dZ[0] == 1)
	{
		if (dZ[1] == 1)
		{
			if (dZ[2] == 1)
			{
				if (dZ[3] == 1)
				{
					pWall->Set_BmpPos(40.f, 80.f);
				}
				else
				{
					pWall->Set_BmpPos(0.f, 80.f);
				}
			}
			else if (dZ[3] == 1)
			{
				pWall->Set_BmpPos(80.f, 0.f);
			}
			else
			{
				pWall->Set_BmpPos(40.f, 0.f);
			}
		}
		else if (dZ[2] == 1)
		{
			if (dZ[3] == 1)
			{
				pWall->Set_BmpPos(80.f, 80.f);
			}
			else
			{
				pWall->Set_BmpPos(240.f, 80.f);
			}
		}
		else if (dZ[3] == 1)
		{
			pWall->Set_BmpPos(120.f, 0.f);
		}
		else
		{
			pWall->Set_BmpPos(280.f, 0.f);
		}
	}
	else if (dZ[1] == 1)
	{
		if (dZ[2] == 1)
		{
			if (dZ[3] == 1)
			{
				pWall->Set_BmpPos(200.f, 0.f);
			}
			else
			{
				pWall->Set_BmpPos(160.f, 0.f);
			}
		}
		else if (dZ[3] == 1)
		{
			pWall->Set_BmpPos(160.f, 80.f);
		}
		else
		{
			pWall->Set_BmpPos(120.f, 80.f);
		}
	}
	else if (dZ[2] == 1)
	{
		if (dZ[3] == 1)
		{
			pWall->Set_BmpPos(240.f, 0.f);
		}
		else
		{
			pWall->Set_BmpPos(280.f, 80.f);
		}
	}
	else if (dZ[3] == 1)
	{
		pWall->Set_BmpPos(200.f, 80.f);
	}
	else
	{
		pWall->Set_BmpPos(0.f, 0.f);
	}
}

void CObjMgr::Save_Player()
{
	HANDLE	hFile = CreateFile(L"../Data/Player.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	
	DWORD	dwByte(0);
	
	for (auto& pPlayer : m_ObjList[OBJ_PLAYER])
	{
		WriteFile(hFile, &(pPlayer->Get_Info()), sizeof(INFO), &dwByte, NULL);
	}
	
	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Player Save", L"성공", MB_OK);
}

void CObjMgr::Load_Player()
{
	HANDLE	hFile = CreateFile(L"../Data/Player.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	INFO	tInfo{};

	DWORD	dwByte(0);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);

		if (dwByte == 0)
			break;

		CObj* pObj = CAbstractFactory<CPlayer>::Create(tInfo.fX, tInfo.fY, 0.f);
		m_ObjList[OBJ_PLAYER].push_back(pObj);
	}

	CloseHandle(hFile);

}

void CObjMgr::Save_Wall()
{
	HANDLE	hFile = CreateFile(L"../Data/Wall.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	
	int iWallID(0);
	
	DWORD	dwByte(0);

	for (auto& vecWall : m_vecWall)
	{
		iWallID = dynamic_cast<CWall*>(vecWall)->Get_WallID();
		WriteFile(hFile, &(vecWall->Get_Info()), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iWallID, sizeof(int), &dwByte, NULL);
	}
	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Wall Save", L"성공", MB_OK);
}

void CObjMgr::Save_Monster()
{
	HANDLE	hFile = CreateFile(L"../Data/Monster.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int iMonsterID(0);

	DWORD	dwByte(0);

	for (auto& m_pMonster : m_ObjList[OBJ_MONSTER])
	{
		iMonsterID = dynamic_cast<CMonster*>(m_pMonster)->Get_MonsterID();
		WriteFile(hFile, &(m_pMonster->Get_Info()), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iMonsterID, sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Player Save", L"성공", MB_OK);
}

void CObjMgr::Load_Wall()
{
	HANDLE	hFile = CreateFile(L"../Data/Wall.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	INFO	tInfo{};
	int		iWallID(0);

	DWORD	dwByte(0);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iWallID, sizeof(int), &dwByte, NULL);

		if (dwByte == 0)
			break;

		CObj* pWall = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY, 0.f);
		dynamic_cast<CWall*>(pWall)->Set_WallID((WALLID)iWallID);
		m_vecWall.push_back(pWall);
	}

	CloseHandle(hFile);

}

void CObjMgr::Load_Monster()
{
	HANDLE	hFile = CreateFile(L"../Data/Monster.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	INFO	tInfo{};
	int		iMonsterID(0);

	DWORD	dwByte(0);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iMonsterID, sizeof(int), &dwByte, NULL);
		CObj* pMonster = nullptr;

		if (dwByte == 0)
			break;
		switch(iMonsterID)
		{
		case MON_SLIME:
			pMonster = CAbstractFactory<CSlime>::Create(tInfo.fX, tInfo.fY, 0.f);
			break;
		case MON_BOSS_SLIME:
			pMonster = CAbstractFactory<CBoss_Slime>::Create(tInfo.fX, tInfo.fY, 0.f);
			break;
		case MON_LARVA:
			pMonster = CAbstractFactory<CLarva>::Create(tInfo.fX, tInfo.fY, 0.f);
			break;
		case MON_BOSS_HIVE:
			pMonster = CAbstractFactory<CBoss_Hive>::Create(tInfo.fX, tInfo.fY, 0.f);
			break;
		}
		if(pMonster)
			m_ObjList[OBJ_MONSTER].push_back(pMonster);
	}

	CloseHandle(hFile);
}


