#include "pch.h"
#include "CEffect.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CSoundMgr.h"
#include "CSceneMgr.h"

CEffect::CEffect() :
	m_iEffectNumber(0)
{
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	m_eObjID = OBJ_EFFECT;
	m_eRenderID = RENDER_OBJECT;

	m_tFrame.dwTime = GetTickCount();
}

int CEffect::Update()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	if (m_iEffectNumber == EFFECT_POWERUP)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());

		m_tInfo.fX = pPlayer->Get_Info().fX;
		m_tInfo.fY = pPlayer->Get_Info().fY + 5.f;
	}

	if (m_iEffectNumber == EFFECT_END_POTAL)
	{
		const RECT& tRect = CObjMgr::Get_Instance()->Get_Player()->Get_Rect();
		RECT rc{};

		if (IntersectRect(&rc, &tRect, &m_tRect))
		{
			CSceneMgr::Get_Instance()->Play_EndingScene();

			CSoundMgr::Get_Instance()->StopAll();
		}
	}


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
	Move_Frame();

	__super::Check_Render();
}

void CEffect::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	if (m_iEffectNumber == EFFECT_HIT)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(182, 185, 184));
	}
	else if (m_iEffectNumber == EFFECT_DEATH || m_iEffectNumber == EFFECT_POTAL || m_iEffectNumber == EFFECT_POWERUP || m_iEffectNumber == EFFECT_END_POTAL)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0,255, 0));
	}
	else
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CEffect::Release()
{
}

void CEffect::Set_Effect(int _iEffectNumber)
{
	m_iEffectNumber = _iEffectNumber;

	switch (m_iEffectNumber)
	{
	case EFFECT_HIT:
		m_tInfo.fCX = 32.f;
		m_tInfo.fCY = 32.f;
		m_tBmpInfo.fCX = 32.f;
		m_tBmpInfo.fCY = 32.f;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Hit_Effect");
		break;
	case EFFECT_ACID_IMPACT:
		m_tInfo.fCX = 40.f;
		m_tInfo.fCY = 40.f;
		m_tBmpInfo.fCX = 40.f;
		m_tBmpInfo.fCY = 40.f;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.dwSpeed = 60;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"AcidImpact");
		break;
	case EFFECT_BOSS_SLIME_DEATH:
		m_tInfo.fCX = 200.f;
		m_tInfo.fCY = 200.f;
		m_tBmpInfo.fCX = 384.f;
		m_tBmpInfo.fCY = 384.f;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Slime_Death");
		break;
	case EFFECT_EGG_HATCHING:
		m_tInfo.fCX = 40.f;
		m_tInfo.fCY = 40.f;
		m_tBmpInfo.fCX = 420.f;
		m_tBmpInfo.fCY = 420.f;
		m_tFrame.iFrameEnd = 9;
		m_tFrame.dwSpeed = 80;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Egg_Hatching");
		break; 
	case EFFECT_BOSS_HIVE_DEATH:
		m_tInfo.fCX = 200.f;
		m_tInfo.fCY = 200.f;
		m_tBmpInfo.fCX = 420.f;
		m_tBmpInfo.fCY = 420.f;
		m_tFrame.iFrameEnd = 10;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Hive_Death");
		break;
	case EFFECT_WARNING:
		m_tInfo.fCX = 40.f;
		m_tInfo.fCY = 40.f;
		m_tBmpInfo.fCX = 160.f;
		m_tBmpInfo.fCY = 160.f;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Warning");
		break;
	case EFFECT_DEATH:
		m_tInfo.fCX = 80.f;
		m_tInfo.fCY = 80.f;
		m_tBmpInfo.fCX = 64.f;
		m_tBmpInfo.fCY = 64.f;
		m_tFrame.iFrameEnd = 6;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Death_Effect");
		break;
	case EFFECT_LARVA_DEATH:		
		m_tInfo.fCX = 60.f;
		m_tInfo.fCY = 60.f;
		m_tBmpInfo.fCX = 420.f;
		m_tBmpInfo.fCY = 420.f;
		m_tFrame.iFrameEnd = 10;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Hive_Death");
		break;
	case EFFECT_POTAL:
		m_tInfo.fCX = 40.f;
		m_tInfo.fCY = 60.f;
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 23.f;
		m_tFrame.iFrameEnd = 8;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Potal");
		break;
	case EFFECT_POWERUP:
		m_tInfo.fCX = 40.f;
		m_tInfo.fCY = 30.f;
		m_tBmpInfo.fCX = 30.f;
		m_tBmpInfo.fCY = 21.f;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.dwSpeed = 200;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"PowerUp");
		break;
	case HIVE_SCENE_EFFECT:
		m_tInfo.fCX = 400.f;
		m_tInfo.fCY = 300.f;
		m_tBmpInfo.fCX = 420.f;
		m_tBmpInfo.fCY = 420.f;
		m_tFrame.iFrameEnd = 9;
		m_tFrame.dwSpeed = 80;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Egg_Hatching");
		break;
	case EFFECT_END_POTAL:
		m_tInfo.fCX = 40.f;
		m_tInfo.fCY = 60.f;
		m_tBmpInfo.fCX = 16.f;
		m_tBmpInfo.fCY = 23.f;
		m_tFrame.iFrameEnd = 8;
		m_tFrame.dwSpeed = 100;
		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Potal");
		break;

	}
	m_tFrame.iMotion = 0;
	m_tFrame.iFrameStart = 0;
}

void CEffect::Move_Frame()
{
	if (m_iEffectNumber == EFFECT_POTAL)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 0;
				++m_tFrame.iMotion;
				if (m_tFrame.iMotion > 1)
				{
					m_bDead = true;

				}
			}

			m_tFrame.dwTime = GetTickCount();
		}
	}
	else if (m_iEffectNumber == EFFECT_END_POTAL)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;

			
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 5;
				//m_tFrame.iFrameStart = 0;
				//++m_tFrame.iMotion;
				//if (m_tFrame.iMotion > 1)
				//{
				//	m_bDead = true;
				//
				//}
			}

			m_tFrame.dwTime = GetTickCount();
		}
	}
	else
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{

			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{

				if (m_iEffectNumber != EFFECT_WARNING)
					m_bDead = true;
				else
					m_tFrame.iFrameStart = 0;
			}

			m_tFrame.dwTime = GetTickCount();
		}
	}
	
}
