#include "pch.h"
#include "CDirt_Wall_Item.h"
#include "CBmpMgr.h"

CDirt_Wall_Item::CDirt_Wall_Item()
{
}

CDirt_Wall_Item::~CDirt_Wall_Item()
{
}

void CDirt_Wall_Item::Initialize()
{
	m_tBmpInfo.fCX = 16.f;
	m_tBmpInfo.fCY = 16.f;

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_tCollisionInfo.fCX = 40.f;
	m_tCollisionInfo.fCY = 40.f;

	m_eObjID = OBJ_ITEM;

	m_eRenderID = RENDER_OBJECT;

	m_eItemClass = IC_USE;

	m_fAtkPower = 0.f;
	m_fMiningPower = 0.f;
	m_fMass = 0.f;

	m_iCount = 1;
	m_iMaxCount = 64;

	m_iItemCode = DIRT_WALL;

	m_eWallID = WALL_DIRT;

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Dirt_Wall_Item");
	m_hSlotDC = m_hObjDC;

	__super::Create_CollisionRect();
}
