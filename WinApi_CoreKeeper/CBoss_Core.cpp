#include "pch.h"
#include "CBoss_Core.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CCoreEquip.h"
#include "CAbstractFactory.h"
#include "CUiMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CPlayer.h"
#include "CSoundMgr.h"

CBoss_Core::CBoss_Core() :
	m_pCoreUi(nullptr),
	m_bEquipCore(false),
	m_bUiOpen(false),
	m_bMoveFrame(false)
{
}

CBoss_Core::~CBoss_Core()
{
}

void CBoss_Core::Initialize()
{
	m_tInfo.fCX = 120.f;
	m_tInfo.fCY = 160.f;

	m_tBmpInfo.fCX = 144.f;
	m_tBmpInfo.fCY = 183.f;

	m_eObjID = OBJ_BUILDING;
	m_eRenderID = RENDER_OBJECT;


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;

	m_tFrame.dwSpeed = 1000;

	m_tCollisionInfo.fCX = 120.f;
	m_tCollisionInfo.fCY = 120.f;
	m_fDistanceY = 60.f;
	__super::Create_CollisionRect();

}

int CBoss_Core::Update()
{
	__super::Update_Rect();
	__super::Update_CollisionRect();

	if (m_pCoreUi)
	{
		if (dynamic_cast<CCoreEquip*>(m_pCoreUi)->IsCoreEquip())
		{
			if (m_bMoveFrame == false)
			{
				m_tFrame.dwTime = GetTickCount();
				CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT2);
				CSoundMgr::Get_Instance()->PlaySound(L"EquipCore.wav", SOUND_EFFECT2, 1.f);
			}

			m_bMoveFrame = true;
		}
		else
		{
			m_bMoveFrame = false;
		}
	}

	if (Check_CanOpen())
	{
		if(!m_bUiOpen)
		{
			if (CKeyMgr::Get_Instance()->Key_Down('E'))
			{
				if (m_iEquipItemCode == SLIME_CORE)
				{
					CUiMgr::Get_Instance()->SlimeCoreOpen();
				}
				else
				{
					CUiMgr::Get_Instance()->HiveCoreOpen();
				}
				dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(true);
				m_bUiOpen = true;
			}
		}
		else
		{
			if (CKeyMgr::Get_Instance()->Key_Down('E'))
			{
				if (m_iEquipItemCode == SLIME_CORE)
				{
					CUiMgr::Get_Instance()->SlimeCoreClose();
				}
				else
				{
					CUiMgr::Get_Instance()->HiveCoreClose();
				}
				dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(false);
				m_bUiOpen = false;
			}

		}
	}
	else
	{
		if(m_bUiOpen)
		{
			if (m_iEquipItemCode == SLIME_CORE)
			{
				CUiMgr::Get_Instance()->SlimeCoreClose();
			}
			else
			{
				CUiMgr::Get_Instance()->HiveCoreClose();
			}
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(false);
			m_bUiOpen = false;
		}

	}

	return OBJ_NOEVENT;
}

void CBoss_Core::Late_Update()
{
	if (m_bMoveFrame)
	{
		Move_Frame();
	}
	else
	{
		m_tFrame.iFrameStart = 0;
	}
}

void CBoss_Core::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

#ifdef _DEBUG
	float fCenterX = m_tInfo.fX - iCameraX + m_fDistanceX;
	float fCenterY = m_tInfo.fY - iCameraY + m_fDistanceY - 20.f;

	float	fRadiusX = m_tCollisionInfo.fCX * 0.5f;
	float	fRadiusY = m_tCollisionInfo.fCY * 0.5f;

	RECT rc = { fCenterX - fRadiusX, fCenterY - fRadiusY,
		fCenterX + fRadiusX, fCenterY + fRadiusY };

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));

	FrameRect(_hDC, &rc, hBrush);

	DeleteObject(hBrush);
#endif
}

void CBoss_Core::Release()
{
}

void CBoss_Core::Set_Boss_Core(int _iItemCode)
{
	m_iEquipItemCode = _iItemCode;

	switch (_iItemCode)
	{
	case SLIME_CORE:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_Core_Buliding");
		m_pCoreUi = CUIFactory<CCoreEquip>::Create();
		dynamic_cast<CCoreEquip*>(m_pCoreUi)->Set_CoreEquip(SLIME_CORE);
		CUiMgr::Get_Instance()->Add_Object(UI_SLIE_CORE, m_pCoreUi);
		break;
	case HIVE_CORE:
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Hive_Core_Buliding");
		m_pCoreUi = CUIFactory<CCoreEquip>::Create();
		dynamic_cast<CCoreEquip*>(m_pCoreUi)->Set_CoreEquip(HIVE_CORE);
		CUiMgr::Get_Instance()->Add_Object(UI_HIVE_CORE, m_pCoreUi);
		break;
	}

}

void CBoss_Core::Move_Frame()
{
	if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
	{
		if(m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			m_tFrame.dwTime = GetTickCount();
		}
	}
	else
	{
		m_bEquipCore = true;
	}
}

bool CBoss_Core::Check_CanOpen()
{
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();

	int iX = (int)m_tInfo.fX / TILECX;
	int iY = (int)m_tInfo.fY / TILECY;

	int iIndex = iX + (iY * TILEX);

	int iPlayerX = (int)pPlayer->Get_Info().fX / TILECX;
	int iPlayerY = (int)pPlayer->Get_Info().fY / TILECY;

	int iPlayerIndex = iPlayerX + (iPlayerY * TILEX);

	if (abs(iX - iPlayerX) > 2 || abs(iY - iPlayerY) > 3)
		return false;
	else
		return true;
}
