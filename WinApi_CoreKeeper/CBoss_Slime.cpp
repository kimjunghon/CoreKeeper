#include "pch.h"
#include "CBoss_Slime.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CBossHpBar.h"
#include "CUiMgr.h"
#include "CObjMgr.h"
#include "CIron.h"
#include "CEffect.h"
#include "CHitDamage.h"
#include "CPlayer.h"
#include "CSceneMgr.h"

CBoss_Slime::CBoss_Slime() :
	m_bMove(false),
	m_bCanHit(true),
	m_pHp_Bar(nullptr),
	m_iDeathEffectCount(0),
	m_bScene(false),
	m_iSceneCount(0)
{
}

CBoss_Slime::~CBoss_Slime()
{
	Release();
}

void CBoss_Slime::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 200.f;

	m_tCollisionInfo.fCX = 160.f;
	m_tCollisionInfo.fCY = 120.f;
	m_fDistanceX = 20.f;
	m_fDistanceY = 80.f;

	m_tBmpInfo.fCX = 120.f;
	m_tBmpInfo.fCY = 120.f;

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 100;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;

	m_eObjID = OBJ_MONSTER;
	m_eRenderID = RENDER_OBJECT;

	m_dwAttackTime = GetTickCount();
	m_dwAttackDelay = 300;

	m_fAtkPower = 20.f;
	m_fMass = 3.f;
	m_fSpeed = 10.f;

	//m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_pTarget = nullptr;
	m_fFindRange = 500.f;

	m_dwCanHitDelay = 320;

	m_iMaxHp = 1000;
	m_iHp = m_iMaxHp;

	m_eMonsterID = MON_BOSS_SLIME;

	m_dwDeathEffectTime = GetTickCount();
	m_dwDeathEffectDelay = 1000;

	m_dwSceneDelay = 2000;
	m_iSceneCount = 0;
	m_fSceneY = -200.f;

	m_pHp_Bar = CUIFactory<CBossHpBar>::Create();
	CUiMgr::Get_Instance()->Add_Object(UI_STATE, m_pHp_Bar);

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Boss_Slime");

	__super::Create_CollisionRect();
}

int CBoss_Slime::Update()
{
	if (!m_bScene)
	{
		if (Check_Scene())
		{
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_BossScene(true);
			Play_Scene();
		}
		else
		{
			m_dwSceneTime = GetTickCount();
			m_bRender = false;
		}

		return OBJ_NOEVENT;
	}

	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlayBGM(L"Base_BGM.wav", g_fEffectVolume);

		return OBJ_DEAD;
	}
	if (m_bEdit)
	{
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}

	IsFindTarget();

	if(!m_bDeading)
	{
		if (CanAttack() && m_dwAttackTime + m_dwAttackDelay < GetTickCount())
		{
			m_eCurState = OS_ATTACK;
			m_bReadyToAttack = true;
		}

		if (m_dwCanHitTime + m_dwCanHitDelay < GetTickCount())
		{
			m_bCanHit = true;
		}

		Move();
		
		if (m_pHp_Bar)
		{
			m_pHp_Bar->Set_Pos(m_tInfo.fX, (float)m_tRect.top + 50);
			dynamic_cast<CBossHpBar*>(m_pHp_Bar)->Set_HP(m_iHp, m_iMaxHp);
			m_pHp_Bar->Update();

			if (m_bRender)
				m_pHp_Bar->Set_Render(true);
			else
				m_pHp_Bar->Set_Render(false);
		}
	}
	else
	{
		if (m_dwDeathEffectTime + m_dwDeathEffectDelay < GetTickCount())
		{
			m_dwDeathEffectTime = GetTickCount();

			++m_iDeathEffectCount;

			if (m_iDeathEffectCount > 5)
			{
				CObj* pEffect = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY + 60.f);
				dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_BOSS_SLIME_DEATH);
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

				CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
				CSoundMgr::Get_Instance()->PlaySound(L"hiveMotherShoot.wav", SOUND_MONSTER, g_fEffectVolume);

				CObj* pObj = CAbstractFactory<CItem>::Create(m_tInfo.fX - 20.f, m_tInfo.fY);
				dynamic_cast<CItem*>(pObj)->Set_Item(SLIME_CORE);
				CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pObj);

				m_bDead = true;
			}
			else
			{
				CObj* pEffect = CAbstractFactory<CEffect>::Create(m_tInfo.fX - 50.f + (m_iDeathEffectCount * 10.f), m_tInfo.fY + (m_iDeathEffectCount * 10.f));
				dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_DEATH);
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);


			}
		}

	}
	
	__super::Update_Rect();

	if(m_bScene)
		__super::Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CBoss_Slime::Late_Update()
{
	if (m_bScene)
	{
		if (m_bEdit)
		{
			__super::Check_Render();
			return;
		}

		if (!m_bDeading)
		{
			Change_State();
			Move_Frame();
		}

		__super::Check_Render();
	}
}

void CBoss_Slime::Render(HDC _hDC)
{
	if (m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (!m_bScene)
		{
			GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)(m_fSceneY - m_tInfo.fCY*0.5f) - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
				m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));
		}
		else
		{
			GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
				m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));

			HDC hName = CBmpMgr::Get_Instance()->Find_Image(L"Slime_Name");

			GdiTransparentBlt(_hDC, ((int)m_tInfo.fX - 55) - iCameraX, (int)m_tRect.top - iCameraY, 107, 33,
				hName, 0,0, 107, 33, RGB(0, 0, 0));

		}
	}
	
}

void CBoss_Slime::Release()
{
}

void CBoss_Slime::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{

	if (m_bCanHit && !m_bDeading)
	{
		if(!m_pTarget)
		{
			m_pTarget = CObjMgr::Get_Instance()->Get_Player();

			CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
			CSoundMgr::Get_Instance()->PlayBGM(L"Boss_Theme_full.wav", g_fEffectVolume);
		}

		m_iHp -= (int)_fAtkDmg;

		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"HitMonster.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);

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
			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Slime_Death.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);

			m_pHp_Bar->Set_Render(false);
			m_bDeading = true;
			m_bAttack = false;

			return;
		}

		m_bHit = true;
		m_bCanHit = false;
		m_dwHitTime = GetTickCount();
		m_dwCanHitTime = GetTickCount();
	}


}

void CBoss_Slime::Move()
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

void CBoss_Slime::Move_Frame()
{
	if (m_bReadyToAttack)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iMotion == 3 && m_tFrame.iFrameStart == 2)
				m_bMove = true;

			if(m_tFrame.iMotion == 4 && m_tFrame.iFrameStart == 1)
				m_bMove = false;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 0;

				++m_tFrame.iMotion;


				if (m_tFrame.iMotion == 4)
				{
					m_bAttack = true;

					CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
					CSoundMgr::Get_Instance()->PlaySound(L"Boss_Slime_Attack.wav", SOUND_MONSTER, g_fEffectVolume);
				}
				else if(m_tFrame.iMotion > 4)
				{
					m_dwAttackTime = GetTickCount();
					m_tFrame.iMotion = 0;
					m_bAttack = false;
					m_bReadyToAttack = false;
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

				if (m_tFrame.iMotion > 1)
				{
					m_tFrame.iMotion = 0;
				}
			}
			m_tFrame.dwTime = GetTickCount();
		}
	}
}

void CBoss_Slime::IsFindTarget()
{
	if (!m_pTarget)
		return;

	if(!m_bReadyToAttack)
	{
		const INFO& tInfo = m_pTarget->Get_Info();

		if (tInfo.fX >= m_tInfo.fX - m_fFindRange && tInfo.fX < m_tInfo.fX + m_fFindRange
			&& tInfo.fY >= m_tInfo.fY - m_fFindRange && tInfo.fY < m_tInfo.fY + m_fFindRange)
		{
			m_ptMovePoint.x = (LONG)tInfo.fX;
			m_ptMovePoint.y = (LONG)tInfo.fY;
		}
	}
}

void CBoss_Slime::Change_State()
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
		m_tFrame.iFrameEnd = 3;

		m_ePreState = m_eCurState;
	}
}

bool CBoss_Slime::Check_Scene()
{
	const INFO& tInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	float fWidth = tInfo.fX - m_tInfo.fX;
	float fHeight = tInfo.fY - m_tInfo.fY;

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	if (fDiagonal <= 300.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CBoss_Slime::Play_Scene()
{

	CSceneMgr::Get_Instance()->Play_BossScene(L"Slime_Scene");

	if(m_iSceneCount < 3)
	{
		if (m_dwSceneTime + m_dwSceneDelay < GetTickCount())
		{
			++m_iSceneCount;
			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Slime_Attack.wav", SOUND_MONSTER, g_fEffectVolume);
			CCameraMgr::Get_Instance()->CameraShake(500, 2.f);
			m_dwSceneTime = GetTickCount();
		}
	}
	else
	{
		m_bRender = true;

		m_fSceneY += 10.f;

		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY - 40 < m_fSceneY)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Slime_Attack.wav", SOUND_MONSTER, g_fEffectVolume);
			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Slime_Death.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);

			CCameraMgr::Get_Instance()->CameraShake(1000, 3.f);

			m_bScene = true;
			m_tInfo.fY = m_fSceneY;
		}
		__super::Update_Rect();

	}
}

bool CBoss_Slime::CanAttack()
{
		if (m_pTarget)
		{
			const INFO& tInfo = m_pTarget->Get_Info();

			float fWidth = tInfo.fX - m_tInfo.fX;
			float fHeight = tInfo.fY - m_tInfo.fY;

			float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

			if (fDiagonal <= 500.f)
			{
				return true;
			}
			else
			{
				m_pTarget = nullptr;
				m_iHp = m_iMaxHp;
				CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
				CSoundMgr::Get_Instance()->PlayBGM(L"Base_BGM.wav", g_fEffectVolume);
				return false;
			}
		}
		else
			return false;
}
