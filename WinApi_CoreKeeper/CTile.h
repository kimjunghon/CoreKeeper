#pragma once
#include "Define.h"

class CTile
{
public:
	CTile();
	~CTile();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	void Set_TileID(TILEID _eTileID);
	const TILEID& Get_TileID() { return m_eTileID; }
	void Set_Pos(float _fX, float _fY) { m_tInfo.fX = _fX, m_tInfo.fY = _fY; }
	void Set_BmpPos(float _fX, float _fY) { m_tBmpInfo.fX = _fX, m_tBmpInfo.fY = _fY; }

	void Set_Under(bool _bUnder) { m_bUnder = _bUnder; }
	const INFO* Get_Info() { return &m_tInfo; }
	const RECT* Get_Rect() { return &m_tRect; }
	
private:
	void Update_Rect();

private:
	RECT m_tRect;
	INFO m_tInfo;
	INFO m_tBmpInfo;
	TILEID m_eTileID;

	HDC m_hTileDC;
	HDC m_hTileUnderDC;

	bool m_bRender;
	bool m_bUnder;
};

