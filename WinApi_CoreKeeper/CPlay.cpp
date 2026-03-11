#include "pch.h"
#include "CPlay.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CSoundMgr.h"

//test header
#include "CSlime.h"
#include "CBoss_Slime.h"
#include "CAbstractFactory.h"
#include "CCraftingTable.h"
#include "CIron.h"
#include "CInvenCrafting.h"
#include "CWall_Item.h"
#include "CLarva.h"
#include "CBoss_Hive.h"
#include "CBoss_Core.h"
#include "CCookingPot.h"
#include "CMain_Core.h"
#include "CSlimeNPC.h"
#include "CForge.h"

float		g_fVolume(1.f);

CPlay::CPlay()
{
}

CPlay::~CPlay()
{
	Release();
}

void CPlay::Initialize()
{
	CTileMgr::Get_Instance()->Load_Tile();
	CObjMgr::Get_Instance()->Load_Wall();
	CObjMgr::Get_Instance()->Load_Player();
	CObjMgr::Get_Instance()->Load_Monster();
	CUiMgr::Get_Instance()->Initialize();

	CCollisionMgr::Get_Instance()->Initialize();
	CCameraMgr::Get_Instance()->Set_Target(CObjMgr::Get_Instance()->Get_Player());

	CSoundMgr::Get_Instance()->PlayBGM(L"Base_BGM.wav", g_fVolume);

	//TEST
	const INFO& tInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	float fX = tInfo.fX;
	float fY = tInfo.fY;

	CObj* pObj = CAbstractFactory<CCraftingTable>::Create(fX - 50.f, fY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILDING, pObj);

	pObj = CAbstractFactory<CCookingPot>::Create(fX + 100.f, fY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILDING, pObj);

	CObj* pSlimeCore = CAbstractFactory<CBoss_Core>::Create(fX - 75.f, fY-200.f);
	dynamic_cast<CBoss_Core*>(pSlimeCore)->Set_Boss_Core(SLIME_CORE);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILDING, pSlimeCore);

	CObj* pHiveCore = CAbstractFactory<CBoss_Core>::Create(fX + 195.f, fY - 200.f);
	dynamic_cast<CBoss_Core*>(pHiveCore)->Set_Boss_Core(HIVE_CORE);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILDING, pHiveCore);

	pObj = CAbstractFactory<CMain_Core>::Create(fX + 60.f, fY - 120.f);
	dynamic_cast<CMain_Core*>(pObj)->Set_SlimeCore(pSlimeCore);
	dynamic_cast<CMain_Core*>(pObj)->Set_HiveCore(pHiveCore);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILDING, pObj);

	pObj = CAbstractFactory<CForge>::Create(fX - 50.f, fY + 100.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILDING, pObj);

	pObj = CAbstractFactory<CSlimeNPC>::Create(fX + 100.f, fY + 100.f);

	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILDING, pObj);
	

	Shadw.Initialize();
}

void CPlay::Update()
{
	CTileMgr::Get_Instance()->Update();
	CUiMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	
	CCollisionMgr::Get_Instance()->Update();
}

void CPlay::Late_Update()
{
	CCameraMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Late_Update();
	CUiMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();

	CCollisionMgr::Get_Instance()->Late_Update();

}

void CPlay::Render(HDC _hDC)
{
	HDC hBackground = CBmpMgr::Get_Instance()->Find_Image(L"Play_Background");
	BitBlt(_hDC, 0, 0, WINCX, WINCY, hBackground, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(_hDC);

	CObjMgr::Get_Instance()->Render(_hDC);
	
	Shadw.Render(_hDC);
	
	CUiMgr::Get_Instance()->Render(_hDC);
}

void CPlay::Release()
{

}
