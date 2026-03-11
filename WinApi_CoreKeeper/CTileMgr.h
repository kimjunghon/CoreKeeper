#pragma once
#include "Define.h"
#include "CTile.h"

class CTileMgr
{
private:
	CTileMgr();
	CTileMgr(const CTileMgr& rhs) = delete;
	CTileMgr& operator=(CTileMgr& rTileMgr) = delete;
	~CTileMgr();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
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
	static CTileMgr* m_pInstance;

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC _hDC);
	void		Release();

public:
	vector<CTile*>* Get_VecTile() { return& m_vecTile; }
	void		Change_Tile(POINT _pt, TILEID _eTileID);
	void		Move_Frame(int _iIndex);
	bool		Check_Tile(POINT _pt);
	const INFO* Get_Tile_Info(POINT _pt);
	const RECT* Get_Tile_Rect(POINT _pt);

	void	Save_Tile();
	void	Load_Tile();

private:
	vector<CTile*> m_vecTile;
};

