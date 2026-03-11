#include "pch.h"
#include "CSlime.h"

#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CEffect.h"
#include "CItem.h"
#include "CHitDamage.h"

CSlime::CSlime() :
	m_bCanHit(true),
	m_eHitDir(DIR_END),
	m_bMove(false)
{
}

CSlime::~CSlime()
{
	Release();
}

void CSlime::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_tCollisionInfo.fCX = 30.f;
	m_tCollisionInfo.fCY = 30.f;
	m_fDistanceX = 0.f;
	m_fDistanceY = 5.f;

	m_tBmpInfo.fCX = 64.f;
	m_tBmpInfo.fCY = 64.f;

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 100;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iMotion = 0;

	m_dwMovePointTime = GetTickCount();
	m_dwMovePointDelay = 3000;

	m_eObjID = OBJ_MONSTER;
	m_eRenderID = RENDER_OBJECT;

	m_dwAttackTime = GetTickCount();
	m_dwAttackDelay = 500;

	m_fAtkPower = 10.f;
	m_fMass = 3.f;
	m_fSpeed = 1.f;

	m_dwHitDelay = 100;
	m_dwCanHitDelay = 320;

	m_iMaxHp = 100;
	m_iHp = m_iMaxHp;

	m_eMonsterID = MON_SLIME;

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();

	m_fFindRange = 120.f;

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Slime");

	__super::Create_CollisionRect();
}

int CSlime::Update()
{
	if (m_bDead)
		return OBJ_DEAD;	

	if (m_bEdit)
	{
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}
	
	if(!m_bDeading)
	{
		IsFindTarget();

		if (CanAttack() && m_dwAttackTime + m_dwAttackDelay < GetTickCount() && !m_bHit)
		{
			m_eCurState = OS_ATTACK;
			m_bReadyToAttack = true;
			m_fSpeed = 2.f;
		}

		if (m_dwCanHitTime + m_dwCanHitDelay < GetTickCount())
		{
			m_bCanHit = true;
		}

		Move();
	}

	__super::Update_Rect();
	__super::Update_CollisionRect();
	return OBJ_NOEVENT;
}

void CSlime::Late_Update()
{
	if (m_bEdit)
	{
		__super::Check_Render();
		return;
	}

	if (!m_bDeading)
	{
		Change_State();
	}
	Move_Frame();

	__super::Check_Render();
}

void CSlime::Render(HDC _hDC)
{
	if(m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CSlime::Release()
{
}

void CSlime::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{

	if (m_bCanHit && !m_bDeading)
	{
		m_iHp -= (int)_fAtkDmg;

		INFO tInfo = m_tInfo;
		switch (_eDir)
		{
		case DIR_LEFT:
			tInfo.fX -= 10.f;
			break;
		case DIR_RIGHT:
			tInfo.fX += 10.f;
			break;
		case DIR_UP:
			tInfo.fY -= 10.f;
			break;
		case DIR_DOWN:
			tInfo.fY += 10.f;
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
			m_bDeading = true;
			m_bAttack = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 4;

			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"slimeDeath.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);

			CObj* pObj = CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY);
			dynamic_cast<CItem*>(pObj)->Set_Item(SLIME_MEAT);
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pObj);
			return;
		}

		m_fHitSpeed = _fMass * (_fSpeed / 2.f);
		m_eHitDir = _eDir;
		m_bHit = true;
		m_bCanHit = false;
		m_dwHitTime = GetTickCount();
		m_dwCanHitTime = GetTickCount();

		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"slimehurt.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);
	}

}

void CSlime::Move()
{
	if (m_bHit)
	{
		switch (m_eHitDir)
		{
		case DIR_LEFT:
			m_tInfo.fX += m_fHitSpeed;
			break;
		case DIR_RIGHT:
			m_tInfo.fX -= m_fHitSpeed;
			break;
		case DIR_DOWN:
			m_tInfo.fY -= m_fHitSpeed;
			break;
		case DIR_UP:
			m_tInfo.fY += m_fHitSpeed;
			break;
		}

		if (m_dwHitTime + m_dwHitDelay < GetTickCount())
		{
			m_bHit = false;
		}
	}
	else
	{
		if (!m_bMove)
			return;

		float       fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

		fWidth = (float)m_ptMovePoint.x - m_tInfo.fX;
		fHeight = (float)m_ptMovePoint.y - m_tInfo.fY;
		if (fWidth == 0.f && fHeight == 0.f)
			return;


		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);


		fRadian = acosf(fWidth / fDiagonal);

		if (m_ptMovePoint.y > m_tInfo.fY)
			fRadian = (2.f * PI) - fRadian;


		m_tInfo.fX += m_fSpeed * cosf(fRadian);
		m_tInfo.fY -= m_fSpeed * sinf(fRadian);
	}

}

void CSlime::Move_Frame()
{
	if (m_bDeading)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 0;

				++m_tFrame.iMotion;
				if(m_tFrame.iMotion == 1)
				{
					m_tFrame.iFrameEnd = 1;
				}
				else if (m_tFrame.iMotion > 1)
				{
					m_bDead = true;
				}
			}
			m_tFrame.dwTime = GetTickCount();
		}
	}
	else
	{
		if (m_bReadyToAttack)
		{
			if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
			{
				++m_tFrame.iFrameStart;

				if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				{
					m_tFrame.iFrameStart = 0;

					++m_tFrame.iMotion;


					if (m_tFrame.iMotion == 3)
					{
						if (m_tFrame.iFrameStart == 0)
						{
							m_bAttack = true;
							m_bMove = true;

							CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
							CSoundMgr::Get_Instance()->PlaySound(L"slimeAttack.wav", SOUND_MONSTER, g_fEffectVolume);
						}

						m_tFrame.iFrameEnd = 4;

					}
					else if (m_tFrame.iMotion > 3)
					{
						m_tFrame.iMotion = 0;
						m_dwAttackTime = GetTickCount();
						m_bAttack = false;
						m_bReadyToAttack = false;
						m_bMove = false;
						m_fSpeed = 1.f;
						m_eCurState = OS_IDLE;
					}
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
					m_tFrame.iFrameStart = 0;

					++m_tFrame.iMotion;

					if (m_tFrame.iMotion == 1)
						m_tFrame.iFrameEnd = 2;
					else if (m_tFrame.iMotion > 2)
					{
						m_tFrame.iMotion = 0;
					}
				}
				m_tFrame.dwTime = GetTickCount();
			}
		}
	}
}

void CSlime::IsFindTarget()
{
	if (!m_pTarget)
		return;

	const INFO& tInfo = m_pTarget->Get_Info();

	if (!m_bReadyToAttack)
	{
		const INFO& tInfo = m_pTarget->Get_Info();

		if (tInfo.fX >= m_tInfo.fX - m_fFindRange && tInfo.fX < m_tInfo.fX + m_fFindRange
			&& tInfo.fY >= m_tInfo.fY - m_fFindRange && tInfo.fY < m_tInfo.fY + m_fFindRange)
		{
			m_ptMovePoint.x = (LONG)tInfo.fX;
			m_ptMovePoint.y = (LONG)tInfo.fY;

			m_bMove = true;
		}
	}
	else if(!m_bAttack)
	{
		m_bMove = false;
	}
}

void CSlime::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OS_IDLE:
			m_tFrame.iMotion = 0;
			break;
		case OS_ATTACK:
			m_tFrame.iMotion = 2;
			break;
		}
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 5;

		m_ePreState = m_eCurState;
	}
}

bool CSlime::CanAttack()
{
	const INFO& tInfo = m_pTarget->Get_Info();
	float fWidth = tInfo.fX - m_tInfo.fX;
	float fHeight = tInfo.fY - m_tInfo.fY;

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	if (fDiagonal <= 80.f)
	{
		return true;
	}
	else
		return false;
}
