#include "pch.h"
#include "CUiMgr.h"
#include "CAbstractFactory.h"
#include "CInventory.h"
#include "CQuickInventory.h"
#include "CPlayer_HpBar.h"
#include "CPlayer_HpBar_Background.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CInvenCrafting.h"
#include "CIronCrafting.h"
#include "CEquip.h"
#include "CCooking.h"
#include "CSmelting.h"
#include "CPlayer_Stat.h"
#include "CMinimap.h"

CUiMgr* CUiMgr::m_pInstance = nullptr;

CUiMgr::CUiMgr() :
	m_bRender(true)
{
}

CUiMgr::~CUiMgr()
{
	Release();
}

void CUiMgr::Initialize()
{
	CUi* pUi = CUIFactory<CInventory>::Create();
	m_UiList[UI_INVENTORY].push_back(pUi);

	pUi = CUIFactory<CInvenCrafting>::Create();
	m_UiList[UI_INVENTORY].push_back(pUi);

	pUi = CUIFactory<CIronCrafting>::Create();
	m_UiList[UI_CRAFTING].push_back(pUi);

	pUi = CUIFactory<CPlayer_HpBar>::Create();
	CObj* pObj = CObjMgr::Get_Instance()->Get_Player();
	dynamic_cast<CPlayer*>(pObj)->Set_HpBar(pUi);
	m_UiList[UI_STATE].push_back(pUi);

	pUi = CUIFactory<CEquip>::Create();
	m_UiList[UI_INVENTORY].push_back(pUi);

	pUi = CUIFactory<CPlayer_Stat>::Create();
	m_UiList[UI_INVENTORY].push_back(pUi);

	pUi = CUIFactory<CCooking>::Create();
	m_UiList[UI_COOKING].push_back(pUi);

	pUi = CUIFactory<CMinimap>::Create();
	m_UiList[UI_MINIMAP].push_back(pUi);
}

void CUiMgr::Update()
{
	for(int i=0; i<UI_END; i++)
	{
		for (auto& pUi : m_UiList[i])
			pUi->Update();
	}
}

void CUiMgr::Late_Update()
{
	for (int i = 0; i < UI_END; i++)
	{
		for (auto& pUi : m_UiList[i])
			pUi->Late_Update();
	}
}

void CUiMgr::Render(HDC _hDC)
{
	if (!m_bRender)
		return;

	for (int i = 0; i < UI_END; i++)
	{
		for (auto& pUi : m_UiList[i])
			pUi->Render(_hDC);
	}
}

void CUiMgr::Release()
{
	for (int i = 0; i < UI_END; i++)
	{
		for_each(m_UiList[i].begin(), m_UiList[i].end(), Safe_Delete<CUi*>);
		m_UiList[i].clear();
	}
}



void CUiMgr::Add_Object(UIID _eUiID, CUi* _pUi)
{
	if (_eUiID >= UI_END || _eUiID < 0 || _pUi == nullptr)
		return;

	m_UiList[_eUiID].push_back(_pUi);
}

void CUiMgr::InventoryOpen()
{
	for (auto& pUi : m_UiList[UI_CRAFTING])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_COOKING])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_SLIE_CORE])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_HIVE_CORE])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_SMELTING])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Open();

	for (auto& pUi : m_UiList[UI_STATE])
		pUi->Open();
}

void CUiMgr::InventoryClose()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_STATE])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Open();
}

void CUiMgr::CraftingOpen()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_COOKING])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_STATE])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Close();

	m_UiList[UI_INVENTORY].front()->Open();
	
	for (auto& pUi : m_UiList[UI_CRAFTING])
		pUi->Open();

}

void CUiMgr::CraftingClose()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_CRAFTING])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Open();
}

void CUiMgr::SlimeCoreOpen()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_STATE])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Close();

	m_UiList[UI_INVENTORY].front()->Open();

	for (auto& pUi : m_UiList[UI_SLIE_CORE])
		pUi->Open();
}

void CUiMgr::SlimeCoreClose()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_SLIE_CORE])
		pUi->Close();
	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Open();
}

void CUiMgr::HiveCoreOpen()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_STATE])
		pUi->Close();

	m_UiList[UI_INVENTORY].front()->Open();

	for (auto& pUi : m_UiList[UI_HIVE_CORE])
		pUi->Open();
}

void CUiMgr::HiveCoreClose()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_HIVE_CORE])
		pUi->Close();
	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Open();
}

void CUiMgr::CookingOpen()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Close();

	m_UiList[UI_INVENTORY].front()->Open();

	for (auto& pUi : m_UiList[UI_COOKING])
		pUi->Open();
}

void CUiMgr::CookingClose()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_COOKING])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Open();
}

void CUiMgr::DialogueOpen()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_SLIME_DIALOGUE])
		pUi->Open();
}

void CUiMgr::DialogueClose()
{
	for (auto& pUi : m_UiList[UI_SLIME_DIALOGUE])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Open();

}

void CUiMgr::SmeltingOpen()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Close();

	m_UiList[UI_INVENTORY].front()->Open();

	for (auto& pUi : m_UiList[UI_SMELTING])
		pUi->Open();
}

void CUiMgr::SmeltingClose()
{
	for (auto& pUi : m_UiList[UI_INVENTORY])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_SMELTING])
		pUi->Close();

	for (auto& pUi : m_UiList[UI_MINIMAP])
		pUi->Open();
}


CUi* CUiMgr::Get_Cursor()
{
	if (m_UiList[UI_MOUSE].empty())
	{
			return nullptr;
	}

	return m_UiList[UI_MOUSE].front();
}

