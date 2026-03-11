#include "pch.h"
#include "CMain_Core.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CBoss_Core.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CAbstractFactory.h"
#include "CEffect.h"
#include "CSoundMgr.h"

CMain_Core::CMain_Core() :
	m_pSlimeCore(nullptr),
	m_pHiveCore(nullptr),
	m_bSlimeCore(false),
	m_bHiveCore(false)
{
}

CMain_Core::~CMain_Core()
{
}

void CMain_Core::Initialize()
{
	m_tInfo.fCX = 160.f;
	m_tInfo.fCY = 160.f;

	m_tBmpInfo.fCX = 240.f;
	m_tBmpInfo.fCY = 240.f;

	m_eObjID = OBJ_BUILDING;
	m_eRenderID = RENDER_OBJECT;


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;

	m_tFrame.dwSpeed = 1000;

	m_tCollisionInfo.fCX = 160.f;
	m_tCollisionInfo.fCY = 160.f;

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Core_Buliding");

	__super::Create_CollisionRect();
}

int CMain_Core::Update()
{
	__super::Update_Rect();
	__super::Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CMain_Core::Late_Update()
{
	CBoss_Core* pSlimeCore = dynamic_cast<CBoss_Core*>(m_pSlimeCore);
	CBoss_Core* pHiveCore = dynamic_cast<CBoss_Core*>(m_pHiveCore);

	if (pSlimeCore->IsEquipCore() && !m_bSlimeCore)
	{
		Move_Frame(SLIME_CORE);
	}
	else if (pHiveCore->IsEquipCore() && !m_bHiveCore)
	{
		Move_Frame(HIVE_CORE);
	}
	else
	{
		m_tFrame.iFrameStart = 0;
	}


	__super::Check_Render();
}

void CMain_Core::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));

#ifdef _DEBUG
	float fCenterX = m_tInfo.fX - iCameraX + m_fDistanceX;
	float fCenterY = m_tInfo.fY - iCameraY + m_fDistanceY;

	float	fRadiusX = m_tCollisionInfo.fCX * 0.5f;
	float	fRadiusY = m_tCollisionInfo.fCY * 0.5f;

	RECT rc = { fCenterX - fRadiusX, fCenterY - fRadiusY,
		fCenterX + fRadiusX, fCenterY + fRadiusY };

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));

	FrameRect(_hDC, &rc, hBrush);

	DeleteObject(hBrush);
#endif
}

void CMain_Core::Release()
{
}

void CMain_Core::Move_Frame(int _iCore)
{
	if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			m_tFrame.dwTime = GetTickCount();

			CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT2);
			CSoundMgr::Get_Instance()->PlaySound(L"PowerUpEffect.wav", SOUND_EFFECT2, 0.8f);
		}
	}
	else
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		
		switch (_iCore)
		{
		case SLIME_CORE:
			m_bSlimeCore = true;
			pPlayer->PowerUp(5.f, 5.f, 5.f, 50);
			break;
		case HIVE_CORE:
			m_bHiveCore = true;
			pPlayer->PowerUp(10.f, 10.f, 10.f, 50);
			CObj* pEffect = CAbstractFactory<CEffect>::Create(1200.f, 1000);
			dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_END_POTAL);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
			break;
		}

		CObj* pEffect = CAbstractFactory<CEffect>::Create();
		dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_POWERUP);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT2);
		CSoundMgr::Get_Instance()->PlaySound(L"PowerUp.wav", SOUND_EFFECT2, 1.f);

		m_tFrame.iFrameStart = 0;
	}
}
