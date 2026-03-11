#include "pch.h"
#include "CBossHPBarBackground.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"

CBossHPBarBackground::CBossHPBarBackground()
{
}

CBossHPBarBackground::~CBossHPBarBackground()
{
}

void CBossHPBarBackground::Initialize()
{
	m_tInfo.fCX = 194.f;
	m_tInfo.fCY = 8.f;
	
	m_tBmpInfo.fCX = 194.f;
	m_tBmpInfo.fCY = 8.f;

	m_bRender = false;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Boss_HpBar_Background");

	__super::Update_Rect();
}

void CBossHPBarBackground::Update()
{

	__super::Update_Rect();
	
}

void CBossHPBarBackground::Late_Update()
{
}

void CBossHPBarBackground::Render(HDC _hDC)
{

	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	
}

void CBossHPBarBackground::Release()
{
}
