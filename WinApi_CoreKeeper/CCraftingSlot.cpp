#include "pch.h"
#include "CCraftingSlot.h"
#include "CBmpMgr.h"
#include "CUiMgr.h"
#include "CInventory.h"
#include "CAbstractFactory.h"
#include "CMelee.h"
#include "CBow.h"
#include "CObjMgr.h"
#include "CArmor.h"
#include "CSoundMgr.h"

CCraftingSlot::CCraftingSlot() :
	m_iCraftingItemCode(0),
	m_iNeedItemCode(0),
	m_iNeedItemCount(0),
	m_bCrafting(false)
{
}

CCraftingSlot::~CCraftingSlot()
{
	Release();
}

void CCraftingSlot::Initialize()
{
	m_tInfo.fCX = SLOTCX;
	m_tInfo.fCY = SLOTCY;

	m_tBmpInfo.fCX = 54.f;
	m_tBmpInfo.fCY = 54.f;

	m_eSlotID = SLOT_CRAFTING;

}

void CCraftingSlot::Update()
{
	if (IsCrafting())
		m_bCrafting = true;
	else
		m_bCrafting = false;

	__super::Update_Rect();
}

void CCraftingSlot::Late_Update()
{
}

void CCraftingSlot::Render(HDC _hDC)
{
	if(m_bCrafting)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, (int)m_tBmpInfo.fCX, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
	else
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}

}

void CCraftingSlot::Release()
{
}

bool CCraftingSlot::IsCrafting()
{
	return 	dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Check_Crafting(m_iNeedItemCode, m_iNeedItemCount);
}

CObj* CCraftingSlot::Crafting()
{
	if(!m_bCrafting)
		return nullptr;

	CObj* pItem = nullptr;
	CInventory* pInven = dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory());

	switch (m_iCraftingItemCode)
	{
	case WOOD_SWORD:
		pItem = CAbstractFactory<CMelee>::Create();
		dynamic_cast<CMelee*>(pItem)->Set_MeleeItem(WOOD_SWORD);
		break;
	case WOOD_BOW:
		pItem = CAbstractFactory<CBow>::Create();
		dynamic_cast<CBow*>(pItem)->Set_BowItem(WOOD_BOW);
		break;
	case WOOD_PICKAXE:
		pItem = CAbstractFactory<CMelee>::Create();
		dynamic_cast<CMelee*>(pItem)->Set_MeleeItem(WOOD_PICKAXE);
		break; 
	case IRON_SWORD:
		pItem = CAbstractFactory<CMelee>::Create();
		dynamic_cast<CMelee*>(pItem)->Set_MeleeItem(IRON_SWORD);
		break;
	case IRON_PICKAXE:
		pItem = CAbstractFactory<CMelee>::Create();
		dynamic_cast<CMelee*>(pItem)->Set_MeleeItem(IRON_PICKAXE);
		break;
	case IRON_BOW:
		pItem = CAbstractFactory<CBow>::Create();
		dynamic_cast<CBow*>(pItem)->Set_BowItem(IRON_BOW);
		break;
	case IRON_HELM:
		pItem = CAbstractFactory<CArmor>::Create();
		dynamic_cast<CArmor*>(pItem)->Set_Armor(IRON_HELM);
		break;
	case IRON_CHEST:
		pItem = CAbstractFactory<CArmor>::Create();
		dynamic_cast<CArmor*>(pItem)->Set_Armor(IRON_CHEST);
		break;
	case IRON_PANTS:
		pItem = CAbstractFactory<CArmor>::Create();
		dynamic_cast<CArmor*>(pItem)->Set_Armor(IRON_PANTS);
		break;	
	case SLIME_FOOD:
		pItem = CAbstractFactory<CItem>::Create();
		dynamic_cast<CItem*>(pItem)->Set_Item(SLIME_FOOD);
		break;
	case LARVA_FOOD:
		pItem = CAbstractFactory<CItem>::Create();
		dynamic_cast<CItem*>(pItem)->Set_Item(LARVA_FOOD);
		break;
	}

	if (pItem)
	{
		pInven->DecreaseItem(m_iNeedItemCode, m_iNeedItemCount);
		dynamic_cast<CItem*>(pItem)->Set_Inventory(true);
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);


		CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
		CSoundMgr::Get_Instance()->PlaySound(L"Crafting.wav", SOUND_UI, 0.8f);
	}

	return pItem;
}

void CCraftingSlot::Set_CraftingItem(int _iCraftingItemCode)
{
	m_iCraftingItemCode = _iCraftingItemCode;

	switch (m_iCraftingItemCode)
	{
	case WOOD_SWORD:
		m_iNeedItemCode = TURF_WALL;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_Sword_wood");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"WoodSword_Crafting_Info");
		break;
	case WOOD_PICKAXE:
		m_iNeedItemCode = TURF_WALL;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_pickaxe_wood");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"WoodPickaxe_Crafting_Info");
		break;
	case WOOD_BOW:
		m_iNeedItemCode = TURF_WALL;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_wood_bow");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"WoodBow_Crafting_Info");
		break;
	case IRON_SWORD:
		m_iNeedItemCode = IRON;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_Sword_Iron");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronSword_Crafting_Info");
		break;
	case IRON_PICKAXE:
		m_iNeedItemCode = IRON;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_pickaxe_iron");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronPickaxe_Crafting_Info");
		break;
	case IRON_BOW:
		m_iNeedItemCode = IRON;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_iron_bow");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronBow_Crafting_Info");
		break;
	case IRON_HELM:
		m_iNeedItemCode = IRON;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_iron_helm");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronHelm_Crafting_Info");
		break;
	case IRON_CHEST:
		m_iNeedItemCode = IRON;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_iron_chest");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronChest_Crafting_Info");
		break;
	case IRON_PANTS:
		m_iNeedItemCode = IRON;
		m_iNeedItemCount = 5;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Crafting_iron_pants");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"IronPants_Crafting_Info");
		break;
	case SLIME_FOOD:
		m_iNeedItemCode = SLIME_MEAT;
		m_iNeedItemCount = 1;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Cooking_Slime_food");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"SlimeFood_Crafting_Info");
		break;
	case LARVA_FOOD:
		m_iNeedItemCode = LARVA_MEAT;
		m_iNeedItemCount = 1;
		m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Cooking_Larva_food");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"LarvaFood_Crafting_Info");
		break;
	}
}			 
