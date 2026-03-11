#include "pch.h"
#include "CStart.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CStart::CStart()
{
}

CStart::~CStart()
{
	Release();
}

void CStart::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Logo/Logo.bmp", L"LOGO");
}

void CStart::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Change_Scene(SC_TITLE);
		return;
	}
}

void CStart::Late_Update()
{
}

void CStart::Render(HDC _hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"LOGO");
	BitBlt(_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CStart::Release()
{
}
