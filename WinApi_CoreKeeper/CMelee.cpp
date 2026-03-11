#include "pch.h"
#include "CWoodSword.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CSoundMgr.h"

CMelee::CMelee() :
	m_bAttack(false),
	m_hLeftDC(nullptr),
	m_hRightDC(nullptr),
	m_hUpDC(nullptr),
	m_hDownDC(nullptr)
{
}

CMelee::~CMelee()
{
}

void CMelee::Initialize()
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

	m_eObjID = OBJ_ITEM;
	m_eRenderID = RENDER_OBJECT;

	m_iCount = 1;
	m_iMaxCount = 1;

	Create_CollisionRect();
}

int CMelee::Update()
{
	if (!m_bEquip || !m_bInventory)
	{
		__super::Update_Rect();
	}

	__super::Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CMelee::Late_Update()
{
	if (m_bInventory)
	{
		m_bRender = false;

		if (m_bEquip)
		{
			Set_Bmp();
			Move_Frame();
			m_bRender = true;
		}
	}
	else
		__super::Check_Render();
}

void CMelee::Render(HDC _hDC)
{
	if (m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CMelee::Use()
{
	m_tFrame.iFrameStart = 1;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.dwTime = GetTickCount();
	m_bAttack = true;

	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"SWING.wav", SOUND_EFFECT, 0.8f);
}

void CMelee::Set_Bmp()
{
	switch (m_eDir)
	{
	case DIR_LEFT:
		m_hObjDC = m_hLeftDC;
		break;
	case DIR_UP:
		m_hObjDC = m_hUpDC;
		break;
	case DIR_DOWN:
		m_hObjDC = m_hDownDC;
		break;
	case DIR_RIGHT:
		m_hObjDC = m_hRightDC;
		break;

	default:
		break;
	}
}

void CMelee::Set_MeleeItem(int _iItemCode)
{
	m_iItemCode = _iItemCode;

	switch (m_iItemCode)
	{
	case WOOD_SWORD:
		m_fAtkPower = 25.f;
		m_fMiningPower = 0.f;
		m_fMass = 5.f;
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_R");
		m_hUpDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_UP");
		m_hDownDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_DOWN");
		m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_wood_L");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"WoodSword_Info");
		break;
	case WOOD_PICKAXE:	
		m_eItemClass = IC_MELEE_PICKAXE;
		m_fAtkPower = 5.f;
		m_fMiningPower = 20.f;
		m_fMass = 5.f;
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxeWood_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxeWood_R");
		m_hUpDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxeWood_UP");
		m_hDownDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxeWood_DOWN");
		m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxeWood_L");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"WoodPickaxe_Info");
		break;
	case IRON_SWORD:
		m_fAtkPower = 45.f;
		m_fMiningPower = 0.f;
		m_fMass = 7.f;
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_iron_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_iron_R");
		m_hUpDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_iron_UP");
		m_hDownDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_iron_DOWN");
		m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"sword_iron_L");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronSword_Info");
		break;
	case IRON_PICKAXE:
		m_eItemClass = IC_MELEE_PICKAXE;
		m_fAtkPower = 5.f;
		m_fMiningPower = 50.f;
		m_fMass = 7.f;
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxe_iron_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxe_iron_R");
		m_hUpDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxe_iron_UP");
		m_hDownDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxe_iron_DOWN");
		m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"pickaxe_iron_L");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronPickaxe_Info");
		break;
	}
}

void CMelee::Move_Frame()
{
	if(m_bAttack)
	{

		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 0;

				++m_tFrame.iMotion;

				if (m_tFrame.iMotion >= 1)
				{
					m_tFrame.iFrameEnd = 1;
				}
				
				if (m_tFrame.iMotion > 2)
				{
					m_tFrame.iMotion = 0;
					m_tFrame.iFrameStart = 0;
					m_tFrame.iFrameEnd = 0;
					m_bAttack = false;
				}
			}
			m_tFrame.dwTime = GetTickCount();

		}
	}
}

void CMelee::Release()
{
}
