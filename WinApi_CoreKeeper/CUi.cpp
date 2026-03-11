#include "pch.h"
#include "CUi.h"

CUi::CUi() :
	m_bOpen(false),
	m_bRender(false),
	m_eUiID(UI_END),
	m_hUiDC(nullptr)
{
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tBmpInfo, sizeof(m_tBmpInfo));
}

CUi::~CUi()
{
}

void CUi::Update_Rect()
{
	m_tRect.left	= (LONG)(m_tInfo.fX - m_tInfo.fCX * 0.5f);
	m_tRect.right	= (LONG)(m_tInfo.fX + m_tInfo.fCX * 0.5f);
	m_tRect.top		= (LONG)(m_tInfo.fY - m_tInfo.fCY * 0.5f);
	m_tRect.bottom	= (LONG)(m_tInfo.fY + m_tInfo.fCY * 0.5f);
}

void CUi::Open()
{
	m_bOpen = true;
}

void CUi::Close()
{
	m_bOpen = false;
}
