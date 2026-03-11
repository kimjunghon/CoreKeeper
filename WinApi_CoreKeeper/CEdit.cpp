#include "pch.h"
#include "CEdit.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CUiMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CCameraMgr.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CBmpMgr.h"
#include "CSlime.h"
#include "CBoss_Slime.h"
#include "CLarva.h"
#include "CBoss_Hive.h"

CEdit::CEdit() :
	m_eTileID(TILE_END),
	m_eWallID(WALL_END)
{
}

CEdit::~CEdit()
{
}

void CEdit::Initialize()
{
	m_eTileID = TILE_TURF;
	m_eWallID = WALL_TURF;


	CTileMgr::Get_Instance()->Initialize();
	CObjMgr::Get_Instance()->Initialize();
	
}

void CEdit::Update()
{
	
	Key_Input();

	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	CUiMgr::Get_Instance()->Update();
	
}

void CEdit::Late_Update()
{

	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
	CUiMgr::Get_Instance()->Late_Update();

}

void CEdit::Render(HDC _hDC)
{

	Rectangle(_hDC, 0, 0, WINCX, WINCY);

	CTileMgr::Get_Instance()->Render(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	CUiMgr::Get_Instance()->Render(_hDC);
}

void CEdit::Release()
{
}

void CEdit::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	//	CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
		CCameraMgr::Get_Instance()->Set_RenderX(-5.f);
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	//	CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
		CCameraMgr::Get_Instance()->Set_RenderX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	//	CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
		CCameraMgr::Get_Instance()->Set_RenderY(-5.f);
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	//	CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);
		CCameraMgr::Get_Instance()->Set_RenderY(5.f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
		m_eTileID = TILE_TURF;
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
		m_eTileID = TILE_DIRT;
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_F3))
		m_eTileID = TILE_CLAY;
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_F4))
		m_eTileID = TILE_EDIT;

	if (CKeyMgr::Get_Instance()->Key_Down('1'))
		m_eWallID = WALL_TURF;
	else if (CKeyMgr::Get_Instance()->Key_Down('2'))
		m_eWallID = WALL_DIRT;
	else if (CKeyMgr::Get_Instance()->Key_Down('3'))
		m_eWallID = WALL_CLAY;
	else if (CKeyMgr::Get_Instance()->Key_Down('4'))
		m_eWallID = WALL_CLAY_IRON;
	else if (CKeyMgr::Get_Instance()->Key_Down('5'))
		m_eWallID = WALL_NULL;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		CTileMgr::Get_Instance()->Change_Tile(ptMouse, m_eTileID);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		{
			CObjMgr::Get_Instance()->Change_Wall(ptMouse, m_eWallID);
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		{
			CObj* pPlayer = CAbstractFactory<CPlayer>::Create((float)ptMouse.x, (float)ptMouse.y, 0.f);
			dynamic_cast<CPlayer*>(pPlayer)->Set_Edit(true);

			CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('V'))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		{
			CObj* Monster = CAbstractFactory<CSlime>::Create((float)ptMouse.x, (float)ptMouse.y, 0.f);
			dynamic_cast<CMonster*>(Monster)->Set_Edit(true);

			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, Monster);
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('B'))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		{
			CObj* Monster = CAbstractFactory<CBoss_Slime>::Create((float)ptMouse.x, (float)ptMouse.y, 0.f);
			dynamic_cast<CMonster*>(Monster)->Set_Edit(true);

			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, Monster);
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down('N'))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		{
			CObj* Monster = CAbstractFactory<CLarva>::Create((float)ptMouse.x, (float)ptMouse.y, 0.f);
			dynamic_cast<CMonster*>(Monster)->Set_Edit(true);

			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, Monster);
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down('M'))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		{
			CObj* Monster = CAbstractFactory<CBoss_Hive>::Create((float)ptMouse.x, (float)ptMouse.y, 0.f);
			dynamic_cast<CMonster*>(Monster)->Set_Edit(true);

			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, Monster);
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CTileMgr::Get_Instance()->Save_Tile();
		CObjMgr::Get_Instance()->Save_Player();
		CObjMgr::Get_Instance()->Save_Wall();
		CObjMgr::Get_Instance()->Save_Monster();
	}

}
