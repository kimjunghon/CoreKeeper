#pragma once
#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	CScrollMgr(const CScrollMgr& rhs) = delete;
	CScrollMgr& operator=(CScrollMgr& rScrollMgr) = delete;
	~CScrollMgr();

public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CScrollMgr;
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
	static CScrollMgr* m_pInstance;

public:
	float		Get_ScrollX() { return m_fScrollX; }
	void		Set_ScrollX(float _fX) { m_fScrollX += _fX; }

	float		Get_ScrollY() { return m_fScrollY; }
	void		Set_ScrollY(float _fY) { m_fScrollY += _fY; }

	void		Scroll_Lock();


private:
	float				m_fScrollX;
	float				m_fScrollY;
};

