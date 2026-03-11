#include "pch.h"
#include "CWallMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"

CWallMgr* CWallMgr::m_pInstance = nullptr;

CWallMgr::CWallMgr()
{
	m_vecWall.reserve(TILEX * TILEY);
	ZeroMemory(m_hWallDC, sizeof(m_hWallDC));
}

CWallMgr::~CWallMgr()
{
	Release();
}

void CWallMgr::Initialize()
{


}

void CWallMgr::Update()
{

}

void CWallMgr::Late_Update()
{

}

void CWallMgr::Render(HDC _hDC)
{
	for (auto& pWall : m_vecWall)
	{
		pWall->Render(_hDC);
	}
}

void CWallMgr::Release()
{
	for_each(m_vecWall.begin(), m_vecWall.end(), Safe_Delete<CWall*>);
	m_vecWall.clear();
	m_vecWall.shrink_to_fit();
}

void CWallMgr::Change_Wall(POINT _pt, WALLID _eWallID)
{
	int	x = _pt.x / TILECX;
	int	y = _pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecWall.size())
		return;

	dynamic_cast<CWall*>(m_vecWall[iIndex])->Set_WallID(_eWallID);
}

void CWallMgr::Move_Roof_Frame(int _iIndex)
{
	if(m_vecWall[_iIndex]->Get_WallID() == WALL_NULL)
		return;

	int dX[4] = { 0, 1, 0, -1 };
	int dY[4] = { 1, 0, -1, 0 };
	int dZ[4] = { 0,0,0,0 }; //го, ©Л, ╩С, аб
	int Dir = 0;
	int iIndex = 0;

	for (Dir = 0; Dir < 4; Dir++)
	{
		iIndex = _iIndex + dX[Dir] + (dY[Dir] * TILEX);

		if (iIndex < 0 || iIndex >= TILEX * TILEY)
			continue;

		if (m_vecWall[iIndex]->Get_WallID() != WALL_NULL)
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
					m_vecWall[_iIndex]->Set_RoofBmpPos(16.f, 16.f);
				}
				else
				{
					m_vecWall[_iIndex]->Set_RoofBmpPos(0.f, 16.f);
				}
			}
			else if (dZ[3] == 1)
			{
				m_vecWall[_iIndex]->Set_RoofBmpPos(16.f, 0.f);
			}
			else
			{
				m_vecWall[_iIndex]->Set_RoofBmpPos(0.f, 0.f);
			}
		}
		else if (dZ[2] == 1)
		{
			if (dZ[3] == 1)
			{
				m_vecWall[_iIndex]->Set_RoofBmpPos(64.f, 16.f);
			}
			else
			{
				m_vecWall[_iIndex]->Set_RoofBmpPos(96.f, 32.f);
			}
		}
		else if (dZ[3] == 1)
		{
			m_vecWall[_iIndex]->Set_RoofBmpPos(64.f, 0.f);
		}
		else
		{
			m_vecWall[_iIndex]->Set_RoofBmpPos(96.f, 0.f);
		}
	}
	else if (dZ[1] == 1)
	{
		if (dZ[2] == 1)
		{
			if (dZ[3] == 1)
			{
				m_vecWall[_iIndex]->Set_RoofBmpPos(32.f, 32.f);
			}
			else
			{
				m_vecWall[_iIndex]->Set_RoofBmpPos(0.f, 32.f);
			}
		}
		else if (dZ[3] == 1)
		{
			m_vecWall[_iIndex]->Set_RoofBmpPos(112.f, 16.f);
		}
		else
		{
			m_vecWall[_iIndex]->Set_RoofBmpPos(80.f, 16.f);
		}
	}
	else if (dZ[2] == 1)
	{
		if (dZ[3] == 1)
		{
			m_vecWall[_iIndex]->Set_RoofBmpPos(64.f, 32.f);
		}
		else
		{
			m_vecWall[_iIndex]->Set_RoofBmpPos(96.f, 48.f);
		}
	}
	else if (dZ[3] == 1)
	{
		m_vecWall[_iIndex]->Set_RoofBmpPos(128.f, 16.f);
	}
	else
	{
		m_vecWall[_iIndex]->Set_RoofBmpPos(144.f, 0.f);
	}
}
