#include "pch.h"
#include "CPlayer_HpBar.h"
#include "CPlayer_HpBar_Background.h"
#include "CAbstractFactory.h"
#include "CBmpMgr.h"

CPlayer_HpBar::CPlayer_HpBar() :
	m_iHp(0),
	m_iMaxHp(0),
	m_fHP_Percentage(0.f),
	m_fPerentageBmpfCX(0.f),
	m_pBackground(nullptr)
{
}

CPlayer_HpBar::~CPlayer_HpBar()
{
	Release();
}

void CPlayer_HpBar::Initialize()
{
	m_tInfo.fCX = 242.f;
	m_tInfo.fCY = 14.f;
	m_tInfo.fX = 142.f;
	m_tInfo.fY = 50.f;

	m_tBmpInfo.fCX = 242.f;
	m_tBmpInfo.fCY = 14.f;

	m_pBackground = CUIFactory<CPlayer_HpBar_Background>::Create();

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_HpBar");
	
	__super::Update_Rect();
}

void CPlayer_HpBar::Update()
{
}

void CPlayer_HpBar::Late_Update()
{
}

void CPlayer_HpBar::Render(HDC _hDC)
{
	m_pBackground->Render(_hDC);

	GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_fPerentageBmpfCX, (int)m_tInfo.fCY,
		m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
}

void CPlayer_HpBar::Release()
{
	Safe_Delete<CUi*>(m_pBackground);
}

void CPlayer_HpBar::Set_HP(int _iHp, int _iMaxHp)
{
	m_iHp = _iHp;
	m_iMaxHp = _iMaxHp;
	m_fHP_Percentage = ((float)m_iHp / (float)m_iMaxHp);
	m_fPerentageBmpfCX = m_tBmpInfo.fCX * m_fHP_Percentage;
}
