#include "pch.h"
#include "CWall_Item.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CObjMgr.h"
#include "CWall.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"

CWall_Item::CWall_Item() :
	m_eWallID(WALL_END)
{
}

CWall_Item::~CWall_Item()
{
	Release();
}

void CWall_Item::Initialize()
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

	m_dwDropTime = GetTickCount();
	m_dwDropDelay = 500;

	__super::Create_CollisionRect();
}

int CWall_Item::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bEquip && !m_bInventory)
	{
		__super::Update_Rect();
		if (m_dwDropTime + m_dwDropDelay < GetTickCount())
			__super::Update_CollisionRect();
	}

	return OBJ_NOEVENT;
}

void CWall_Item::Late_Update()
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

void CWall_Item::Render(HDC _hDC)
{
	if(m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CWall_Item::Release()
{
}

void CWall_Item::Use()
{
	CUi* pUi = CUiMgr::Get_Instance()->Get_Cursor();
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();

	int iMouseX = (int)pUi->Get_Info().fX / TILECX;
	int iMouseY = (int)pUi->Get_Info().fY / TILECY;

	int iMouseIndex = iMouseX + (iMouseY * TILEX);

	int iPlayerX = (int)pPlayer->Get_Info().fX / TILECX;
	int iPlayerY = (int)pPlayer->Get_Info().fY / TILECY;

	int iPlayerIndex = iPlayerX + (iPlayerY * TILEX);

	if (iMouseIndex == iPlayerIndex || abs(iMouseX - iPlayerX) > 1 || abs(iMouseY - iPlayerY) > 1)
		return;

	CObj* pWall = (*(CObjMgr::Get_Instance()->Get_VecWall()))[iMouseIndex];

	if (dynamic_cast<CWall*>(pWall)->Get_WallID() == WALL_NULL)
	{
		dynamic_cast<CWall*>(pWall)->Set_WallID(m_eWallID);
		--m_iCount;

		if (m_iCount <= 0)
			m_bDead = true;


		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"Use_Wall.wav", SOUND_EFFECT, 0.8f);
	}
}

void CWall_Item::Move_Frame()
{
}

void CWall_Item::Set_WallItem(int _iItemCode)
{
	m_iItemCode = _iItemCode;

	switch (m_iItemCode)
	{
	case TURF_WALL:
		m_eWallID = WALL_TURF;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Turf_Wall_Item");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"Turf_Wall_Info");
		break;
	case DIRT_WALL:
		m_eWallID = WALL_DIRT;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Dirt_Wall_Item");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"Dirt_Wall_Info");
		break;
	case CLAY_WALL:
		m_eWallID = WALL_CLAY;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Clay_Wall_Item");
		m_hInfoDC = CBmpMgr::Get_Instance()->Find_Image(L"Clay_Wall_Info");
		break;
	}

	m_hSlotDC = m_hObjDC;
}
