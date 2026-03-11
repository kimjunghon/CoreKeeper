#include "pch.h"
#include "CWoodSword.h"
#include "CBmpMgr.h"

CWoodSword::CWoodSword()
{
}

CWoodSword::~CWoodSword()
{
}

void CWoodSword::Initialize()
{

	m_eObjID = OBJ_ITEM;
	m_eRenderID = RENDER_OBJECT;
	m_eItemClass = IC_MELEE_SWORD;
	
	m_fAtkPower = 30.f;
	m_fMiningPower = 0.f;
	m_fMass = 5.f;

	m_hLeftDC  = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_L");
	m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_R");
	m_hUpDC    = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_UP");
	m_hDownDC  = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_DOWN");

	m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_L");

	m_iItemCode = WOOD_SWORD;

	Create_CollisionRect();

}