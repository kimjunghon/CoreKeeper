#include "pch.h"
#include "CLarva_Egg.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CAbstractFactory.h"
#include "CLarva.h"
#include "CObjMgr.h"
#include "CEffect.h"
#include "CSoundMgr.h"
#include "CHitDamage.h"

CLarva_Egg::CLarva_Egg() :
    m_bCanHit(true)
{
}

CLarva_Egg::~CLarva_Egg()
{
    Release();
}

void CLarva_Egg::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;

    m_tBmpInfo.fCX = 50.f;
    m_tBmpInfo.fCY = 50.f;

    m_tCollisionInfo.fCX = 40.f;
    m_tCollisionInfo.fCY = 40.f;
    m_fDistanceY = 5.f;

    m_tFrame.dwTime = GetTickCount();
    m_tFrame.dwSpeed = 200;
    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 7;
    m_tFrame.iMotion = 0;

    m_iMaxHp = 100;
    m_iHp = m_iMaxHp;

    m_eObjID = OBJ_MONSTER;
    m_eRenderID = RENDER_OBJECT;

    m_eMonsterID = MON_LARVA;

    m_dwCanHitDelay = 320;

    m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"LarvaEgg");

    __super::Create_CollisionRect();
}

int CLarva_Egg::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dwCanHitTime + m_dwCanHitDelay < GetTickCount())
	{
		m_bCanHit = true;
	}


	__super::Update_Rect();
	__super::Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CLarva_Egg::Late_Update()
{
    Move_Frame();

    __super::Check_Render();
}

void CLarva_Egg::Render(HDC _hDC)
{
    if (m_bRender)
    {
        int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
        int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

        GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
            m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));
    }
}

void CLarva_Egg::Release()
{
}

void CLarva_Egg::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{
	if (m_bCanHit)
	{
		m_iHp -= (int)_fAtkDmg;

		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"larvaHit.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);
		
		INFO tInfo = m_tInfo;
		switch (_eDir)
		{
		case DIR_LEFT:
			tInfo.fX = m_tRect.left + 10.f;
			break;
		case DIR_RIGHT:
			tInfo.fX = m_tRect.right - 10.f;
			break;
		case DIR_UP:
			tInfo.fY = m_tRect.top + 10.f;
			break;
		case DIR_DOWN:
			tInfo.fY = m_tRect.bottom - 10.f;
			break;
		}
		CObj* pObj = CAbstractFactory<CEffect>::Create(tInfo.fX, tInfo.fY);
		dynamic_cast<CEffect*>(pObj)->Set_Effect(EFFECT_HIT);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

		pObj = CAbstractFactory<CHitDamage>::Create(m_tInfo.fX, m_tRect.top + 20.f);
		dynamic_cast<CHitDamage*>(pObj)->Set_HitDamage(OBJ_MONSTER, _fAtkDmg);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

		if (m_iHp <= 0)
		{
			m_bDead = true;
			CObj* pObj = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY);
			dynamic_cast<CEffect*>(pObj)->Set_Effect(EFFECT_EGG_HATCHING);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);
			return;
		}

		m_bCanHit = false;
		m_dwCanHitTime = GetTickCount();


	}
}

void CLarva_Egg::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

        if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
        {
            m_tFrame.iFrameStart = 0;
            ++m_tFrame.iMotion;

            if (m_tFrame.iMotion > 2)
            {
                CObj* pObj = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY);
                dynamic_cast<CEffect*>(pObj)->Set_Effect(EFFECT_EGG_HATCHING);
                CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

                CObj* pLarva = CAbstractFactory<CLarva>::Create(m_tInfo.fX, m_tInfo.fY);
                CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pLarva);

                m_bDead = true;
            }
        }

		m_tFrame.dwTime = GetTickCount();

	}
}

void CLarva_Egg::Move()
{
}


void CLarva_Egg::IsFindTarget()
{
}

void CLarva_Egg::Change_State()
{
}
