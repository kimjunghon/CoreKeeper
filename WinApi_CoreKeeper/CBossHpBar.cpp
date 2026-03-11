#include "pch.h"
#include "CBossHpBar.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CBossHPBarBackground.h"
#include "CAbstractFactory.h"

CBossHpBar::CBossHpBar() :
	m_iHp(0),
	m_iMaxHp(0),
	m_fHP_Percentage(0.f),
	m_fPerentageBmpfCX(0.f),
	m_pBackground(nullptr)
{
}

CBossHpBar::~CBossHpBar()
{
	Release();
}

void CBossHpBar::Initialize()
{
	m_tInfo.fCX = 188.f;
	m_tInfo.fCY = 6.f;

	m_tBmpInfo.fCX = 188.f;
	m_tBmpInfo.fCY = 6.f;

	m_bRender = false;

	m_pBackground = CUIFactory<CBossHPBarBackground>::Create();


	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Boss_HpBar");

	__super::Update_Rect();
}

void CBossHpBar::Update()
{
	if (m_bRender)
	{
		m_pBackground->Set_Pos(m_tInfo.fX, m_tInfo.fY);
		m_pBackground->Update();
		__super::Update_Rect();

	}

}

void CBossHpBar::Late_Update()
{
	if (m_bRender)
	{
		m_pBackground->Late_Update();
	}
}

void CBossHpBar::Render(HDC _hDC)
{

	if (m_bRender)
	{
		m_pBackground->Render(_hDC);

		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();
		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_fPerentageBmpfCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CBossHpBar::Release()
{
	Safe_Delete<CUi*>(m_pBackground);
}

void CBossHpBar::Set_HP(int _iHp, int _iMaxHp)
{
	m_iHp = _iHp;
	m_iMaxHp = _iMaxHp;
	m_fHP_Percentage = ((float)m_iHp / (float)m_iMaxHp);
	m_fPerentageBmpfCX = m_tBmpInfo.fCX * m_fHP_Percentage;
}
