#include "pch.h"
#include "CSlimeNPC.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CUiMgr.h"
#include "CPlayer.h"
#include "CDialogue.h"
#include "CAbstractFactory.h"


CSlimeNPC::CSlimeNPC() :
	m_bUiOpen(false)
{
}

CSlimeNPC::~CSlimeNPC()
{
}

void CSlimeNPC::Initialize()
{
	m_tBmpInfo.fCX = 30.f;
	m_tBmpInfo.fCY = 30.f;

	m_tInfo.fCX = 60.f;
	m_tInfo.fCY = 60.f;

	m_tCollisionInfo.fCX = 40.f;
	m_tCollisionInfo.fCY = 40.f;
	m_fDistanceY = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 100;

	m_eObjID = OBJ_BUILDING;
	m_eRenderID = RENDER_OBJECT;

	__super::Create_CollisionRect();

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC");
	m_hQuestDC = CBmpMgr::Get_Instance()->Find_Image(L"QuestIcon");

	m_pDialogue = CUIFactory<CDialogue>::Create();
	CUiMgr::Get_Instance()->Add_Object(UI_SLIME_DIALOGUE, m_pDialogue);
}

int CSlimeNPC::Update()
{

	__super::Update_Rect();
	__super::Update_CollisionRect();

	if (Check_Dialogue() && !m_bUiOpen)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('E'))
		{
			CUiMgr::Get_Instance()->DialogueOpen();
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(true);
			m_bUiOpen = true;
		}
	}
	else
	{
		m_bUiOpen = false;
	}
//	else if (Check_Dialogue() && m_bUiOpen)
//	{
//		if (CKeyMgr::Get_Instance()->Key_Down('E'))
//		{
//			CUiMgr::Get_Instance()->DialogueClose();
//			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(false);
//			
//		}
//	}

	return OBJ_NOEVENT;
}

void CSlimeNPC::Late_Update()
{
	Move_Frame();
}

void CSlimeNPC::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	int iQuestFrame = dynamic_cast<CDialogue*>(m_pDialogue)->QuestFrame();

	if (iQuestFrame >= 0)
	{
		GdiTransparentBlt(_hDC, (int)m_tInfo.fX - iCameraX - 10, (int)m_tRect.top - iCameraY - 40, 20, 30,
			m_hQuestDC, 20 * iQuestFrame, 0, 20, 30, RGB(255, 255, 255));
	}

	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));
}

void CSlimeNPC::Release()
{
}

void CSlimeNPC::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
		}

		m_tFrame.dwTime = GetTickCount();
	}
}

bool CSlimeNPC::Check_Dialogue()
{
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();

	int iX = (int)m_tInfo.fX / TILECX;
	int iY = (int)m_tInfo.fY / TILECY;

	int iIndex = iX + (iY * TILEX);

	int iPlayerX = (int)pPlayer->Get_Info().fX / TILECX;
	int iPlayerY = (int)pPlayer->Get_Info().fY / TILECY;

	int iPlayerIndex = iPlayerX + (iPlayerY * TILEX);

	if (abs(iX - iPlayerX) > 1 || abs(iY - iPlayerY) > 1)
		return false;
	else
		return true;
}
