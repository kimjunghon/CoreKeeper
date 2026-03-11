#pragma once
#include "Define.h"

class CUi
{
public:
	CUi();
	virtual ~CUi();

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;

public:
	void	Update_Rect();
	void	Open();
	void	Close();

	void	Set_Pos(float _fX, float _fY) { m_tInfo.fX = _fX, m_tInfo.fY = _fY; }
	void	Set_PosX(float _fX) { m_tInfo.fX = _fX; }
	void	Set_PosY(float _fY) { m_tInfo.fY = _fY; }

	const INFO& Get_Info() { return m_tInfo; }
	const RECT& Get_Rect() { return m_tRect; }
	bool	IsRender() { return m_bRender; }
	void	Set_Render(bool _bRender) { m_bRender = _bRender; }

	UIID	Get_UiID() { return m_eUiID; }
protected:

	RECT		m_tRect;
	INFO		m_tInfo;
	BMPINFO		m_tBmpInfo;

	UIID		m_eUiID;
	bool		m_bOpen;
	bool		m_bRender;

	HDC			m_hUiDC;
};

