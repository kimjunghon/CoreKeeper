#include "pch.h"
#include "CItem.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
CItem::CItem() :
	m_eItemClass(IC_END),
	m_bEquip(false),
	m_bInventory(false),
	m_iCount(0),
	m_iMaxCount(0)
{
}

CItem::~CItem()
{
}

void CItem::Initialize()
{
	m_tCollisionInfo.fCX = 20.f;
	m_tCollisionInfo.fCY = 20.f;

	m_eObjID = OBJ_ITEM;

	m_eRenderID = RENDER_OBJECT;

	m_eItemClass = IC_USE;

	m_dwDropTime = GetTickCount();
	m_dwDropDelay = 500;

	m_fAtkPower = 0.f;
	m_fMiningPower = 0.f;
	m_fMass = 0.f;
}

int CItem::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bEquip && !m_bInventory)
	{
		__super::Update_Rect();
		if(m_dwDropTime + m_dwDropDelay < GetTickCount())
			__super::Update_CollisionRect();
	}


	return OBJ_NOEVENT;
}

void CItem::Late_Update()
{
	if (m_bInventory)
	{
		m_bRender = false;

		if (m_bEquip)
		{
			m_bRender = true;
		}
	}
	else
	{
		__super::Check_Render();
	}
}

void CItem::Render(HDC _hDC)
{
	if(m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

	}
}

void CItem::Release()
{
}

void CItem::Use()
{
	if (m_iItemCode == SLIME_FOOD)
	{
		--m_iCount;
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->EatFood(30);
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"FoodEat.wav", SOUND_EFFECT, 0.5f);
		if (m_iCount <= 0)
			m_bDead = true;
	}
	else if(m_iItemCode == LARVA_FOOD)
	{
		--m_iCount;
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->EatFood(50);
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"FoodEat.wav", SOUND_EFFECT, 0.5f);
		if (m_iCount <= 0)
			m_bDead = true;
	}
	else
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"Player_Attack_NoEqiup.wav", SOUND_EFFECT, 0.8f);
	}
}

void CItem::Move_Frame()
{
}

void CItem::Set_Item(int _iItemCode)
{
	m_iItemCode = _iItemCode;

	switch (m_iItemCode)
	{
	case IRON_STONE:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 64;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Stone");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronStone_Info");
		break;
	case IRON:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 64;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Info");
		break;
	case SLIME_CORE:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 1;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_Core");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"SlimeCore_Info");
		break;
	case HIVE_CORE:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 1;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Hive_Core");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"HiveCore_Info");
		break;
	case SLIME_MEAT:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 64;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_Meat");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_Info");
		break;
	case SLIME_FOOD:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 64;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_Food");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"SlimeFood_Info");
		break;
	case LARVA_MEAT:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 64;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Larva_Meat");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"LarvaMeat_Info");
		break;
	case LARVA_FOOD:
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 16.f;
		m_tInfo.fCX = 36.f;
		m_tInfo.fCY = 36.f;
		m_iCount = 1;
		m_iMaxCount = 64;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Larva_Food");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"LarvaFood_Info");
		break;
	}

	__super::Create_CollisionRect();
	m_hSlotDC = m_hObjDC;
}

void CItem::IncreaseCount(int _iItemCount)
{
	if (m_iCount + _iItemCount < m_iMaxCount)
	{
		m_iCount += _iItemCount;
	}
}
