#pragma once

#include "CWall.h"

class CWallMgr
{
private:
	CWallMgr();
	CWallMgr(const CWallMgr& rhs) = delete;
	CWallMgr& operator=(CWallMgr& rObjMgr) = delete;
	~CWallMgr();

public:
	static CWallMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CWallMgr;
			m_pInstance->Initialize();
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CWallMgr* m_pInstance;

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC _hDC);
	void		Release();

public:
	void		Change_Wall(POINT _pt, WALLID _eTileID);
	void		Move_Wall_Roof_Frame(int _iIndex);
	
private:
	vector<CWall*> m_vecWall;
	HDC m_hWallDC[WALL_END];

};
