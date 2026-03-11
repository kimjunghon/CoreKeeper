#include "pch.h"
#include "CClear.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"

CClear::CClear()
{
}

CClear::~CClear()
{
}

void CClear::Initialize()
{
}

void CClear::Update()
{
	if(CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		DestroyWindow(g_hWnd);
		return;
	}
}

void CClear::Late_Update()
{
}

void CClear::Render(HDC _hDC)
{
	HDC hBackground = CBmpMgr::Get_Instance()->Find_Image(L"ClearScene");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hBackground, 0,0, SRCCOPY);
}

void CClear::Release()
{
}
