#include "pch.h"
#include "CLarva.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CEffect.h"
#include "CItem.h"
#include "CHitDamage.h"

CLarva::CLarva() :
	m_bMove(false),
	m_bCanHit(true)
{
}

CLarva::~CLarva()
{
	Release();
}

void CLarva::Initialize()
{
	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;

	m_tCollisionInfo.fCX = 36.f;
	m_tCollisionInfo.fCY = 36.f;
	m_fDistanceX = 0.f;
	m_fDistanceY = 16.f;

	m_tBmpInfo.fCX = 32.f;
	m_tBmpInfo.fCY = 32.f;

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 150;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;

	m_eCurState = OS_IDLE;
	m_eDir = DIR_DOWN;

	m_fAtkPower = 30.f;
	m_fMass = 5.f;
	m_fSpeed = 1.f;

	m_dwAttackTime = GetTickCount();
	m_dwAttackDelay = 500;

	m_dwCanHitDelay = 320;

	m_iMaxHp = 150;
	m_iHp = m_iMaxHp;

	m_eObjID = OBJ_MONSTER;
	m_eRenderID = RENDER_OBJECT;

	m_eMonsterID = MON_LARVA;

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();

	m_fFindRange = 160.f;

	m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Larva_L");;
	m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Larva_R");;
	m_hUpDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Larva_UP");;
	m_hDownDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Larva_DOWN");;

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Larva_DOWN");

	__super::Create_CollisionRect();
}

int CLarva::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bEdit)
	{
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}
	IsFindTarget();

	Move();

	if (CanAttack() && m_dwAttackTime + m_dwAttackDelay < GetTickCount())
	{
		m_eCurState = OS_ATTACK;
		m_bReadyToAttack = true;
		m_bMove = false;
	}

	if (m_dwCanHitTime + m_dwCanHitDelay < GetTickCount())
	{
		m_bCanHit = true;
	}


	__super::Update_Rect();
	__super::Update_CollisionRect();
	return OBJ_NOEVENT;
}

void CLarva::Late_Update()
{
	if (m_bEdit)
	{
		__super::Check_Render();
		return;
	}
	Set_Bmp();	
	Change_State();
	Move_Frame();

	__super::Check_Render();
}

void CLarva::Render(HDC _hDC)
{
	if (m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
	}
}

void CLarva::Release()
{
}

void CLarva::Move()
{
	if (!m_bMove)
		return;

	float       fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

	fWidth = (float)m_ptMovePoint.x - m_tInfo.fX;
	fHeight = (float)m_ptMovePoint.y - (m_tInfo.fY - m_fDistanceX);
	if (abs(fWidth) <= 10.f && abs(fHeight) <= 10.f)
		return;


	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acosf(fWidth / fDiagonal);

	if (m_ptMovePoint.y > (m_tInfo.fY - m_fDistanceX))
		fRadian = (2.f * PI) - fRadian;

	if(abs(m_fSpeed * cosf(fRadian)) < abs(m_fSpeed * sinf(fRadian)))
	{
		if (m_fSpeed * sinf(fRadian) > 0.f)
			m_eDir = DIR_UP;
		else
			m_eDir = DIR_DOWN;
	}
	else
	{
		if (m_fSpeed * cosf(fRadian) > 0.f)
			m_eDir = DIR_RIGHT;
		else
			m_eDir = DIR_LEFT;
	}

	m_tInfo.fX += m_fSpeed * cosf(fRadian);
	m_tInfo.fY -= m_fSpeed * sinf(fRadian);
}

void CLarva::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_eCurState == OS_ATTACK)
		{
			if (m_tFrame.iFrameStart == 3)
			{
				m_bAttack = true;
				CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
				CSoundMgr::Get_Instance()->PlaySound(L"larvaAttack.wav", SOUND_MONSTER, g_fEffectVolume);
			}

		}

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
			
			if (m_eCurState == OS_ATTACK)
			{
				m_dwAttackTime = GetTickCount();
				m_bAttack = false;
				m_bReadyToAttack = false;
				m_bMove = true;
				m_eCurState = OS_IDLE;
			}
		}
		m_tFrame.dwTime = GetTickCount();

	}
}

void CLarva::IsFindTarget()
{
	if (!m_pTarget)
		return;

	const INFO& tInfo = m_pTarget->Get_Info();

	if(!m_bReadyToAttack)
	{
		if (tInfo.fX >= m_tInfo.fX - m_fFindRange && tInfo.fX < m_tInfo.fX + m_fFindRange
			&& tInfo.fY >= m_tInfo.fY - m_fFindRange && tInfo.fY < m_tInfo.fY + m_fFindRange)
		{
			m_ptMovePoint.x = (LONG)tInfo.fX;
			m_ptMovePoint.y = (LONG)tInfo.fY;

			m_bMove = true;
			m_eCurState = OS_WALK;
		}
		else
		{
			m_bMove = false;
			m_eCurState = OS_IDLE;
		}
	}
}

void CLarva::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OS_IDLE:
			m_tFrame.iMotion = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.dwSpeed = 200;
			break;
		case OS_WALK:
			m_tFrame.iMotion = 1;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.dwSpeed = 100;
			break;
		case OS_ATTACK:
			m_tFrame.iMotion = 2;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.dwSpeed = 100;
			break;
		}
		m_tFrame.iFrameStart = 0;

		m_ePreState = m_eCurState;
	}
}

void CLarva::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
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
		dynamic_cast<CHitDamage*>(pObj)->Set_HitDamage(OBJ_MONSTER, (int)_fAtkDmg);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

		if (m_iHp <= 0)
		{
			m_bDead = true;
			CObj* pEffect = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY);
			dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_LARVA_DEATH);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
			m_bAttack = false;

			CObj* pObj = CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY);
			dynamic_cast<CItem*>(pObj)->Set_Item(LARVA_MEAT);
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pObj);
			return;
		}

		m_fHitSpeed = _fMass * (_fSpeed / 2.f);
		m_eHitDir = _eDir;
		m_bHit = true;
		m_bCanHit = false;
		m_dwHitTime = GetTickCount();
		m_dwCanHitTime = GetTickCount();
	}
}

void CLarva::Set_Bmp()
{
	switch (m_eDir)
	{
	case DIR_LEFT:
		m_hObjDC = m_hLeftDC;
		break;
	case DIR_UP:
		m_hObjDC = m_hUpDC;
		break;
	case DIR_DOWN:
		m_hObjDC = m_hDownDC;
		break;
	case DIR_RIGHT:
		m_hObjDC = m_hRightDC;
		break;

	default:
		break;
	}
}

bool CLarva::CanAttack()
{
	const INFO& tInfo = m_pTarget->Get_Info();
	float fWidth = tInfo.fX - m_tInfo.fX;
	float fHeight = tInfo.fY - (m_tInfo.fY - m_fDistanceX);

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	if (fDiagonal <= 40.f)
	{
		return true;
	}
	else
		return false;
}
