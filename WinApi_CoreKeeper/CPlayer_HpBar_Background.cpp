#include "pch.h"
#include "CPlayer_HpBar_Background.h"
#include "CBmpMgr.h"
CPlayer_HpBar_Background::CPlayer_HpBar_Background()
{
}

CPlayer_HpBar_Background::~CPlayer_HpBar_Background()
{
	Release();
}

void CPlayer_HpBar_Background::Initialize()
{
	m_tInfo.fCX = 244.f;
	m_tInfo.fCY = 16.f;
	m_tInfo.fX = 142.f;
	m_tInfo.fY = 50.f;

	m_tBmpInfo.fCX = 244.f;
	m_tBmpInfo.fCY = 16.f;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Player_HpBar_Background");

	__super::Update_Rect();
}

void CPlayer_HpBar_Background::Update()
{
}

void CPlayer_HpBar_Background::Late_Update()
{
}

void CPlayer_HpBar_Background::Render(HDC _hDC)
{
	GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
}

void CPlayer_HpBar_Background::Release()
{
}
