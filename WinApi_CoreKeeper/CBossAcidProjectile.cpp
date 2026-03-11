#include "pch.h"
#include "CBossAcidProjectile.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CEffect.h"
#include "CSoundMgr.h"

CBossAcidProjectile::CBossAcidProjectile() :
	m_bFalling(false),
	m_fTargetX(0.f),
	m_fTargetY(0.f),
	m_pWarningEffect(nullptr)
{
}

CBossAcidProjectile::~CBossAcidProjectile()
{
}

void CBossAcidProjectile::Initialize()
{
	m_tBmpInfo.fCX = 15.f;
	m_tBmpInfo.fCY = 16.f;

	m_tInfo.fCX = 15.f;
	m_tInfo.fCY = 32.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 75;

	m_eDir = DIR_END;
	m_eObjID = OBJ_PROJECTILE;
	m_eRenderID = RENDER_OBJECT;
	
	m_bRender = true;

	m_fSpeed = 5.f;
	m_fMass = 1.f;

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"AcidProjectile");
}

int CBossAcidProjectile::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Move();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBossAcidProjectile::Late_Update()
{
	Move_Frame();
}

void CBossAcidProjectile::Render(HDC _hDC)
{
	if (m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));
	}
}

void CBossAcidProjectile::Release()
{
}

void CBossAcidProjectile::Set_TargetPos(float _fTargetX, float _fTargetY)
{
	m_fTargetX = _fTargetX; 
	m_fTargetY = _fTargetY;

	m_pWarningEffect = CAbstractFactory<CEffect>::Create(m_fTargetX, m_fTargetY);
	dynamic_cast<CEffect*>(m_pWarningEffect)->Set_Effect(EFFECT_WARNING);
	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, m_pWarningEffect);
}

void CBossAcidProjectile::Move()
{
	if (m_bFalling)
	{
		m_tInfo.fY += m_fSpeed;

		if (m_tInfo.fY > m_fTargetY)
		{
			m_bDead = true;

			m_pWarningEffect->Set_Dead();

			CSoundMgr::Get_Instance()->StopSound(SOUND_PROJECTILE);
			CSoundMgr::Get_Instance()->PlaySound(L"larvaHit.wav", SOUND_PROJECTILE, 0.5f);

			CObj* pObj = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY);
			dynamic_cast<CEffect*>(pObj)->Set_Effect(EFFECT_ACID_IMPACT);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

			const INFO& tPlayerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

			if (abs(tPlayerInfo.fX - m_fTargetX) <= float(TILECX) && abs(tPlayerInfo.fY - m_fTargetY) <= (float)TILECY)
			{
				if (abs(tPlayerInfo.fX - m_fTargetX) > abs(tPlayerInfo.fY - m_fTargetY))
				{
					if(tPlayerInfo.fX - m_fTargetX < 0.f)
						CObjMgr::Get_Instance()->Get_Player()->Hit(m_fAtkPower, m_fMass, m_fSpeed, DIR_LEFT);
					else
						CObjMgr::Get_Instance()->Get_Player()->Hit(m_fAtkPower, m_fMass, m_fSpeed, DIR_RIGHT);
				}
				else
				{
					if (tPlayerInfo.fY - m_fTargetY < 0.f)
						CObjMgr::Get_Instance()->Get_Player()->Hit(m_fAtkPower, m_fMass, m_fSpeed, DIR_UP);
					else
						CObjMgr::Get_Instance()->Get_Player()->Hit(m_fAtkPower, m_fMass, m_fSpeed, DIR_DOWN);
				}
			}
		}
	}
	else
	{
		m_tInfo.fY -= m_fSpeed;

		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();
		if (m_tRect.bottom < iCameraY)
		{
			m_bFalling = true;
			m_fSpeed = 3.f;
			m_tInfo.fX = m_fTargetX;
			m_tFrame.iMotion = 1;
		}
	}
}

void CBossAcidProjectile::Move_Frame()
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
