#pragma once
#include "Define.h"
#include "CUi.h"

class CUiMgr
{
private:
	CUiMgr();
	CUiMgr(const CUiMgr& rhs) = delete;
	CUiMgr& operator=(const CUiMgr& rUimgr) = delete;
	~CUiMgr();

public:
	static CUiMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CUiMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CUiMgr* m_pInstance;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	CUi* Get_PlayerInventory() { return m_UiList[UI_INVENTORY].front(); }
	list<CUi*>* Get_Inventory() { return &m_UiList[UI_INVENTORY]; }
	list<CUi*>* Get_Crafting() { return &m_UiList[UI_CRAFTING]; }
	void Add_Object(UIID _eUiID, CUi* _pUi);
	void InventoryOpen();
	void InventoryClose();
	void CraftingOpen();
	void CraftingClose();
	void SlimeCoreOpen();
	void SlimeCoreClose();
	void HiveCoreOpen();
	void HiveCoreClose();
	void CookingOpen();
	void CookingClose();
	void DialogueOpen();
	void DialogueClose();
	void SmeltingOpen();
	void SmeltingClose();

	CUi* Get_Cursor();

	void Set_Render(bool _bRender) {
		m_bRender = _bRender;
	}

private:
	list<CUi*> m_UiList[UI_END];
	bool m_bRender;
};

