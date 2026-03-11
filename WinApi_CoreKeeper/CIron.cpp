#include "pch.h"
#include "CIron.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"

CIron::CIron()
{
}

CIron::~CIron()
{
	Release();
}

void CIron::Initialize()
{
	m_tBmpInfo.fCX = 16.f;
	m_tBmpInfo.fCY = 16.f;

	m_tInfo.fCX = 36.f;
	m_tInfo.fCY = 36.f;

	m_tCollisionInfo.fCX = 30.f;
	m_tCollisionInfo.fCY = 30.f;

	m_eObjID = OBJ_ITEM;

	m_eRenderID = RENDER_OBJECT;

	m_eItemClass = IC_USE;

	m_fAtkPower = 0.f;
	m_fMiningPower = -10.f;
	m_fMass = 0.f;

	m_iCount = 1;
	m_iMaxCount = 64;

	m_iItemCode = IRON;

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron");

	m_hSlotDC = m_hObjDC;

	__super::Create_CollisionRect();
}

int CIron::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bEquip && !m_bInventory)
	{
		__super::Update_Rect();
		__super::Update_CollisionRect();
	}


	return OBJ_NOEVENT;
}

void CIron::Late_Update()
{
	if (m_bEquip || m_bInventory)
	{

	}
	else
	{
		__super::Check_Render();
	}
}

void CIron::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hObjDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
}

void CIron::Release()
{
}

void CIron::Use()
{
}

void CIron::Move_Frame()
{
}
