#include "pch.h"
#include "CIronSword.h"

CIronSword::CIronSword()
{
}

CIronSword::~CIronSword()
{
}

void CIronSword::Initialize()
{
	m_tBmpInfo.fCX = 40.f;
	m_tBmpInfo.fCY = 40.f;

	m_tCollisionInfo.fCX = 40.f;
	m_tCollisionInfo.fCY = 40.f;

	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 50;

	m_eDir = DIR_END;
	m_eObjID = OBJ_ITEM;
	m_eRenderID = RENDER_OBJECT;
	m_eItemClass = IC_MELEE_SWORD;

	m_fAtkPower = 30.f;
	m_fMiningPower = 0.f;
	m_fMass = 5.f;

	m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_L");
	m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_R");
	m_hUpDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_UP");
	m_hDownDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_DOWN");

	m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_L");

	m_iCount = 1;
	m_iMaxCount = 1;

	m_iItemCode = WOOD_SWORD;

	Create_CollisionRect();
}
