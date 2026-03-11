#include "pch.h"
#include "CArmor.h"
#include "CBmpMgr.h"

CArmor::CArmor()
{
}

CArmor::~CArmor()
{
}

void CArmor::Initialize()
{
	m_tBmpInfo.fCX = 16.f;
	m_tBmpInfo.fCY = 16.f;

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_eObjID = OBJ_ITEM;

	m_eRenderID = RENDER_OBJECT;

	m_fAtkPower = 0.f;
	m_fMiningPower = 0.f;
	m_fMass = 0.f;
	
	m_iCount = 1;
	m_iMaxCount = 1;

	m_hSlotDC = m_hObjDC;
}

int CArmor::Update()
{
	if (!m_bEquip && !m_bInventory)
	{
		__super::Update_Rect();
		__super::Update_CollisionRect();
	}

	return OBJ_NOEVENT;
}

void CArmor::Late_Update()
{
	if (m_bEquip || m_bInventory)
	{

	}
	else
	{
		__super::Check_Render();
	}
}

void CArmor::Render(HDC _hDC)
{
}

void CArmor::Release()
{
}

void CArmor::Use()
{
}

void CArmor::Move_Frame()
{
}

void CArmor::Set_Armor(int _iItemCode)
{
	m_iItemCode = _iItemCode;

	switch (m_iItemCode)
	{
	case IRON_HELM:
		m_eItemClass = IC_ARMOR_HELM;
		m_fDefPower = 5.f;
		m_iMaxHp = 20;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Helm");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronHelm_Info");
		break;
	case IRON_CHEST:
		m_eItemClass = IC_ARMOR_CHEST;
		m_fDefPower = 10.f;
		m_iMaxHp = 30;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Chest");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronChest_Info");
		break;
	case IRON_PANTS:
		m_eItemClass = IC_ARMOR_PANTS;
		m_fDefPower = 5.f;
		m_iMaxHp = 20;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Pants");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronPants_Info");
		break;
	}

	m_hSlotDC = m_hObjDC;
}
