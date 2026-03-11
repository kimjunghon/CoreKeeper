#pragma once

#include "CMyBmp.h"

class CBmpMgr
{
private:
	CBmpMgr();
	CBmpMgr(const CBmpMgr& rhs) = delete;
	CBmpMgr& operator=(CBmpMgr& rBmpMgr) = delete;
	~CBmpMgr();

public:
	static CBmpMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBmpMgr;
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
	static CBmpMgr* m_pInstance;

public:
	void		Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC			Find_Image(const TCHAR* pImgKey);

	void		Load_Bmp();
	void		Delete_Bmp(const TCHAR* pImgKey);
	void		Release();

private:
	map<const TCHAR*, CMyBmp*>		m_mapBit;
};

