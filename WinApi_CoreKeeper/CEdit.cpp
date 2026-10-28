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

namespace
{
	// 마우스 커서 위치를 카메라 위치를 더한 월드 좌표로 변환
	POINT Get_MouseWorldPos()
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += (int)CCameraMgr::Get_Instance()->Get_RenderX();
		ptMouse.y += (int)CCameraMgr::Get_Instance()->Get_RenderY();

		return ptMouse;
	}

	// 편집 모드로 몬스터를 생성해 배치
	template<typename T>
	void Place_Monster(const POINT& _ptMouse)
	{
		CObj* pMonster = CAbstractFactory<T>::Create((float)_ptMouse.x, (float)_ptMouse.y, 0.f);
		static_cast<CMonster*>(pMonster)->Set_Edit(true);

		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pMonster);
	}
}

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

	POINT ptMouse = Get_MouseWorldPos();

	// 좌클릭 : 타일 배치
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
		CTileMgr::Get_Instance()->Change_Tile(ptMouse, m_eTileID);

	// 우클릭 : 벽 배치 (편집용 빈 타일 위에는 배치하지 않음)
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON) && CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		CObjMgr::Get_Instance()->Change_Wall(ptMouse, m_eWallID);

	// C : 플레이어 시작 위치
	if (CKeyMgr::Get_Instance()->Key_Down('C') && CTileMgr::Get_Instance()->Check_Tile(ptMouse))
	{
		CObj* pPlayer = CAbstractFactory<CPlayer>::Create((float)ptMouse.x, (float)ptMouse.y, 0.f);
		static_cast<CPlayer*>(pPlayer)->Set_Edit(true);

		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);
	}

	// V, B, N, M : 몬스터 배치
	if (CKeyMgr::Get_Instance()->Key_Down('V') && CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		Place_Monster<CSlime>(ptMouse);
	if (CKeyMgr::Get_Instance()->Key_Down('B') && CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		Place_Monster<CBoss_Slime>(ptMouse);
	if (CKeyMgr::Get_Instance()->Key_Down('N') && CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		Place_Monster<CLarva>(ptMouse);
	if (CKeyMgr::Get_Instance()->Key_Down('M') && CTileMgr::Get_Instance()->Check_Tile(ptMouse))
		Place_Monster<CBoss_Hive>(ptMouse);

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CTileMgr::Get_Instance()->Save_Tile();
		CObjMgr::Get_Instance()->Save_Player();
		CObjMgr::Get_Instance()->Save_Wall();
		CObjMgr::Get_Instance()->Save_Monster();
	}

}
