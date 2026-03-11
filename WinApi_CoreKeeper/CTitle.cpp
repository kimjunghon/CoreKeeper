#include "pch.h"
#include "CTitle.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CUiMgr.h"
#include "CAbstractFactory.h"
#include "CMouse.h"

CTitle::CTitle()
{
	ZeroMemory(&m_tBackgroundFrame, sizeof(m_tBackgroundFrame));
	ZeroMemory(&m_tTextInfo, sizeof(m_tTextInfo));
}

CTitle::~CTitle()
{
}

void CTitle::Initialize()
{
	m_tBackgroundFrame.dwSpeed = 200;
	m_tBackgroundFrame.dwTime = GetTickCount();
	m_tBackgroundFrame.iFrameStart = 0;
	m_tBackgroundFrame.iFrameEnd = 1;
	m_tBackgroundFrame.iMotion = 0;

	m_tTextInfo.fCX = 165.f;
	m_tTextInfo.fCY = 71.f;
	m_tTextInfo.fX = 150.f;
	m_tTextInfo.fY = 100.f;

	CBmpMgr::Get_Instance()->Load_Bmp();

	CUiMgr::Get_Instance()->Add_Object(UI_MOUSE, CUIFactory<CMouse>::Create());

}

void CTitle::Update()
{
	CUiMgr::Get_Instance()->Update();
}

void CTitle::Late_Update()
{
	CUiMgr::Get_Instance()->Late_Update();
	Move_Frame();
	
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		CSceneMgr::Get_Instance()->Change_Scene(SC_PLAY);
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		CSceneMgr::Get_Instance()->Change_Scene(SC_EDIT);
		return;
	}
}

void CTitle::Render(HDC _hDC)
{
	HDC hBackground = CBmpMgr::Get_Instance()->Find_Image(L"Title_Background");
	HDC hTitleText = CBmpMgr::Get_Instance()->Find_Image(L"Title_Text");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hBackground, WINCX * m_tBackgroundFrame.iFrameStart, WINCY * m_tBackgroundFrame.iMotion, SRCCOPY);

	GdiTransparentBlt(_hDC,
		(int)m_tTextInfo.fX, (int)m_tTextInfo.fY, 
		250,150,
		hTitleText,
		0,
		0,
		(int)m_tTextInfo.fCX,
		(int)m_tTextInfo.fCY,
		RGB(255, 255, 255));

	CUiMgr::Get_Instance()->Render(_hDC);
}

void CTitle::Release()
{
}

void CTitle::Move_Frame()
{
	if (m_tBackgroundFrame.dwTime + m_tBackgroundFrame.dwSpeed < GetTickCount())
	{
		++m_tBackgroundFrame.iFrameStart;

		if (m_tBackgroundFrame.iFrameStart > m_tBackgroundFrame.iFrameEnd)
		{
			m_tBackgroundFrame.iFrameStart = 0;
			
			++m_tBackgroundFrame.iMotion;

			if (m_tBackgroundFrame.iMotion > 1)
				m_tBackgroundFrame.iMotion = 0;
		}

		m_tBackgroundFrame.dwTime = GetTickCount();

	}
}
