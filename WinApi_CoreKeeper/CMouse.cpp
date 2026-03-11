#include "pch.h"
#include "CMouse.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CScrollMgr.h"
#include "CItem.h"


CMouse::CMouse() :
	m_pItem(nullptr),
	m_hInfoDC(nullptr),
	m_iFrame(0)
{
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tBmpInfo.fCX = 32.f;
	m_tBmpInfo.fCY = 32.f;

	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;

	m_eUiID = UI_MOUSE;
}

void CMouse::Update()
{
	POINT		pt{};

	GetCursorPos(&pt);

	ScreenToClient(g_hWnd, &pt);

	//pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	
	pt.x += (LONG)CCameraMgr::Get_Instance()->Get_RenderX();
	pt.y += (LONG)CCameraMgr::Get_Instance()->Get_RenderY();

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;


	__super::Update_Rect();

	ShowCursor(FALSE);
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC _hDC)
{
	HDC hMouse = CBmpMgr::Get_Instance()->Find_Image(L"Cursor");
	
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();
	
	if (m_pItem)
	{
		HDC hItemDC = *(m_pItem->Get_SlotDC());
		BMPINFO tBmpInfo = (m_pItem)->Get_BmpInfo();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hItemDC, 0, 0, (int)tBmpInfo.fCX, (int)tBmpInfo.fCY, RGB(255, 255, 255));

		int iItemCount = dynamic_cast<CItem*>(m_pItem)->Get_ItemCount();
		if (iItemCount > 1)
		{
			HDC hItemCountDC = CBmpMgr::Get_Instance()->Find_Image(L"FontNumber");

			if (iItemCount >= 10)
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX + 10, (int)m_tRect.top - iCameraY, 5, 15,
					hItemCountDC, (iItemCount / 10) * 15, 0, 15, 45, RGB(0, 0, 0));

				GdiTransparentBlt(_hDC, (int)m_tRect.left + 15, (int)m_tRect.top, 5, 15,
					hItemCountDC, (iItemCount % 10) * 15, 0, 15, 45, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX + 10, (int)m_tRect.top - iCameraY, 5, 15,
					hItemCountDC, (iItemCount % 10) * 15, 0, 15, 45, RGB(0, 0, 0));

			}
		}
	}

	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMouse, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

	if (m_hInfoDC)
	{
		BitBlt(_hDC, (int)m_tRect.right - iCameraX, (int)m_tRect.top - iCameraY, 200, 200,
			*m_hInfoDC, 200*m_iFrame, 0, SRCCOPY);
	}
}

void CMouse::Release()
{
}

void CMouse::Set_InfoDC(HDC* _pInfoDC, int _iFrame)
{
	m_hInfoDC = _pInfoDC;
	m_iFrame = _iFrame;
}
