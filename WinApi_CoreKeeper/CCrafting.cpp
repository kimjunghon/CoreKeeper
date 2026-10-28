#include "pch.h"
#include "CCrafting.h"
#include "CCraftingSlot.h"
#include "CUiMgr.h"
#include "CInventory.h"


CCrafting::CCrafting()
{
}

CCrafting::~CCrafting()
{
	Release();
}

void CCrafting::Initialize()
{

}

void CCrafting::Update()
{
	if (m_bOpen)
	{
		m_bRender = true;

		map<int, int> mapItemCount;
		static_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Collect_ItemCount(mapItemCount);

		for (auto& vecSlot : m_vecCraftingSlot)
		{
			static_cast<CCraftingSlot*>(vecSlot)->Refresh_Crafting(mapItemCount);
			vecSlot->Update();
			vecSlot->Set_Render(m_bRender);
		}
	}
	else
	{
		m_bRender = false;
		for (auto& vecSlot : m_vecCraftingSlot)
		{
			vecSlot->Set_Render(m_bRender);
		}
	}

	__super::Update_Rect();
}

void CCrafting::Late_Update()
{
	if(m_bRender)
	{
		for (auto& vecSlot : m_vecCraftingSlot)
		{
			vecSlot->Late_Update();
		}
	}
}

void CCrafting::Render(HDC _hDC)
{
	if(m_bRender)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

		for (auto& vecSlot : m_vecCraftingSlot)
		{
			vecSlot->Render(_hDC);
		}
	}
}

void CCrafting::Release()
{
	for_each(m_vecCraftingSlot.begin(), m_vecCraftingSlot.end(), Safe_Delete<CUi*>);
	m_vecCraftingSlot.clear();
	m_vecCraftingSlot.shrink_to_fit();
}
