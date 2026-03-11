#include "pch.h"
#include "CButton.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"

CButton::CButton()
{
}

CButton::~CButton()
{
}

void CButton::Initialize()
{
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 40.f;

	m_tInfo.fY = 470.f;

	m_tBmpInfo.fCX = 100.f;
	m_tBmpInfo.fCY = 40.f;

}

void CButton::Update()
{
	if (!m_bRender)
	{
		return;
	}

	__super::Update_Rect();
}

void CButton::Late_Update()
{
	if (!m_bRender)
	{
		return;
	}
}

void CButton::Render(HDC _hDC)
{
	if (m_bRender)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}

}

void CButton::Release()
{
}

bool CButton::ButtonClick()
{
	if (!m_bRender)
	{
		return false;
	}

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			return true;
		}
	}

	return false;
}


void CButton::Set_Button(BUTTONID _eButtinID)
{
	switch (_eButtinID)
	{
	case BT_START:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Button_Start");
		m_tInfo.fX = 170.f;
		break;
	case BT_STOP:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Button_Stop");
		m_tInfo.fX = 280.f;
		break;
	case BT_ACCEPT:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Button_Accept");
		m_tInfo.fX = 170.f;
		break;
	case BT_REFUSE:
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Button_Refuse");
		m_tInfo.fX = 280.f;
		break;
	}
}
