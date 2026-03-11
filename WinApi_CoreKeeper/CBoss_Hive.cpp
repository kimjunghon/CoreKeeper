#include "pch.h"
#include "CBoss_Hive.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CBossAcidProjectile.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CBossHpBar.h"
#include "CUiMgr.h"
#include "CEffect.h"
#include "CItem.h"
#include "CLarva_Egg.h"
#include "CHitDamage.h"
#include "CPlayer.h"
#include "CSceneMgr.h"

CBoss_Hive::CBoss_Hive() :
	m_bCanHit(true),
	m_iAttackCount(0),
	m_iMaxAttackCount(0),
	m_iDeathEffectCount(0),
	m_bScene(false),
	m_iSceneCount(0)
{
}

CBoss_Hive::~CBoss_Hive()
{
	Release();
}

void CBoss_Hive::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 200.f;

	m_tCollisionInfo.fCX = 160.f;
	m_tCollisionInfo.fCY = 120.f;
	m_fDistanceX = 20.f;
	m_fDistanceY = 80.f;

	m_tBmpInfo.fCX = 140.f;
	m_tBmpInfo.fCY = 140.f;

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 100;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iMotion = 0;

	m_eObjID = OBJ_MONSTER;
	m_eRenderID = RENDER_OBJECT;

	m_dwAttackTime = GetTickCount();
	m_dwAttackDelay = 1000;

	m_iMaxAttackCount = 4;

	m_fAtkPower = 40.f;
	m_fMass = 0.f;
	m_fSpeed = 0.f;

	m_pTarget = nullptr;
	m_fFindRange = 500.f;

	m_dwCanHitTime = GetTickCount();
	m_dwCanHitDelay = 360;

	m_iMaxHp = 2000;
	m_iHp = m_iMaxHp;

	m_eMonsterID = MON_BOSS_HIVE;

	m_dwDeathEffectTime = GetTickCount();
	m_dwDeathEffectDelay = 1000;

	m_dwSceneDelay = 2000;
	m_iSceneCount = 0;

	m_pHp_Bar = CUIFactory<CBossHpBar>::Create();
	CUiMgr::Get_Instance()->Add_Object(UI_STATE, m_pHp_Bar);

	m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"LarvaEgg");

	__super::Create_CollisionRect();
}

int CBoss_Hive::Update()
{
	if (!m_bScene)
	{
		if (Check_Scene())
		{
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_BossScene(true);
			Play_Scene();

			m_bRender = true;
			__super::Update_Rect();
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

	if (!m_bDeading)
	{
		IsFindTarget();

		if (CanAttack() && m_dwAttackTime + m_dwAttackDelay < GetTickCount() && !m_bHit)
		{
			m_eCurState = OS_ATTACK;
			m_bReadyToAttack = true;
		}

		if (m_dwCanHitTime + m_dwCanHitDelay < GetTickCount())
		{
			m_bCanHit = true;
		}

		if (m_pHp_Bar)
		{
			m_pHp_Bar->Set_Pos(m_tInfo.fX, (float)m_tRect.top);
			dynamic_cast<CBossHpBar*>(m_pHp_Bar)->Set_HP(m_iHp, m_iMaxHp);

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
				CObj* pEffect = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY - 100.f);
				dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_BOSS_HIVE_DEATH);
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

				CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
				CSoundMgr::Get_Instance()->PlaySound(L"hiveMotherShoot.wav", SOUND_MONSTER, g_fEffectVolume);

				CObj* pObj = CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY);
				dynamic_cast<CItem*>(pObj)->Set_Item(HIVE_CORE);
				CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pObj);

				m_bDead = true;
			}
			else
			{
				CObj* pEffect = CAbstractFactory<CEffect>::Create(m_tInfo.fX - 50.f + (m_iDeathEffectCount * 10.f), m_tInfo.fY);
				dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_DEATH);
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

			}
		}
	}

	__super::Update_Rect();
	__super::Update_CollisionRect();
	
	return OBJ_NOEVENT;
}

void CBoss_Hive::Late_Update()
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

void CBoss_Hive::Render(HDC _hDC)
{
	if (m_bRender)
	{
		int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
		int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

		if (!m_bScene)
		{
			GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
				m_hObjDC, 0, (int)50 * m_iSceneCount, 50,50, RGB(0, 255,0));
		}
		else
		{
			GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
				m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));
		
			HDC hName = CBmpMgr::Get_Instance()->Find_Image(L"Hive_Name");

			GdiTransparentBlt(_hDC, ((int)m_tInfo.fX - 55) - iCameraX, (int)m_tRect.top - iCameraY -50, 107, 33,
				hName, 0, 0, 107, 33, RGB(0, 0, 0));
		}
	}
}

void CBoss_Hive::Release()
{
}

void CBoss_Hive::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{
	if (m_bCanHit && !m_bDeading)
	{
		if (!m_pTarget)
		{
			m_pTarget = CObjMgr::Get_Instance()->Get_Player();

			CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
			CSoundMgr::Get_Instance()->PlayBGM(L"Boss_Theme_full.wav", g_fEffectVolume);
		}

		m_iHp -= (int)_fAtkDmg;

		m_bCanHit = false;
		m_dwCanHitTime = GetTickCount();

		INFO tInfo = m_tInfo;
		switch (_eDir)
		{
		case DIR_LEFT:
			tInfo.fX = m_tRect.left + 20.f;
			break;
		case DIR_RIGHT:
			tInfo.fX = m_tRect.right - 20.f;
			break;
		case DIR_UP:
			tInfo.fY = m_tRect.top + 20.f;
			break;
		case DIR_DOWN:
			tInfo.fY = m_tRect.bottom - 20.f;
			break;
		}
		CObj* pObj = CAbstractFactory<CEffect>::Create(tInfo.fX, tInfo.fY);
		dynamic_cast<CEffect*>(pObj)->Set_Effect(EFFECT_HIT);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

		pObj = CAbstractFactory<CHitDamage>::Create(m_tInfo.fX, m_tRect.top + 20.f);
		dynamic_cast<CHitDamage*>(pObj)->Set_HitDamage(OBJ_MONSTER, (int)_fAtkDmg);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"HitMonster.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);

		if (m_iHp <= 0)
		{

			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Hive_Mother_Death.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);
			m_pHp_Bar->Set_Render(false);
			m_bDeading = true;
			m_bAttack = false;

			return;
		}
	}

}

void CBoss_Hive::Move()
{
}


void CBoss_Hive::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_eCurState == OS_ATTACK)
		{
			if (m_tFrame.iFrameStart == 2)
			{
				Attack();

				CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
				CSoundMgr::Get_Instance()->PlaySound(L"hiveMotherShoot.wav", SOUND_MONSTER, g_fEffectVolume);
			}

		}

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;

			if (m_eCurState == OS_ATTACK)
			{
				m_dwAttackTime = GetTickCount();
				m_bReadyToAttack = false;
				m_eCurState = OS_IDLE;
			}
		}
		m_tFrame.dwTime = GetTickCount();

	}
}

void CBoss_Hive::IsFindTarget()
{
	if (!m_pTarget)
		return;

	if (!m_bReadyToAttack)
	{
		const INFO& tInfo = m_pTarget->Get_Info();

		m_ptMovePoint.x = (LONG)tInfo.fX;
		m_ptMovePoint.y = (LONG)tInfo.fY;
	}
}

void CBoss_Hive::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OS_IDLE:
			m_tFrame.iMotion = 0;
			m_tFrame.iFrameEnd = 7;
			break;
		case OS_ATTACK:
			m_tFrame.iMotion = 1;
			m_tFrame.iFrameEnd = 6;
			break;
		}

		m_tFrame.iFrameStart = 0;

		m_ePreState = m_eCurState;
	}
}

bool CBoss_Hive::Check_Scene()
{
	const INFO& tInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	float fWidth = tInfo.fX - m_tInfo.fX;
	float fHeight = tInfo.fY - m_tInfo.fY;

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	if (fDiagonal <= 150.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CBoss_Hive::Play_Scene()
{
	CSceneMgr::Get_Instance()->Play_BossScene(L"Hive_Scene");

	if (m_iSceneCount < 3)
	{
		if (m_dwSceneTime + m_dwSceneDelay < GetTickCount())
		{
			++m_iSceneCount;
			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
			CSoundMgr::Get_Instance()->PlaySound(L"hiveMotherShoot.wav", SOUND_MONSTER, g_fEffectVolume);
			CCameraMgr::Get_Instance()->CameraShake(500, 2.f);
			m_dwSceneTime = GetTickCount();
		}
	}
	else
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER);
		CSoundMgr::Get_Instance()->PlaySound(L"hiveMotherShoot.wav", SOUND_MONSTER, g_fEffectVolume);
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"Hive_Mother_Death.wav", SOUND_MONSTER_EFFECT, g_fEffectVolume);
		
		CCameraMgr::Get_Instance()->CameraShake(1000, 3.f);

		m_hObjDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster_Boss_Hive");

		CObj* pObj = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY + 40.f);
		dynamic_cast<CEffect*>(pObj)->Set_Effect(HIVE_SCENE_EFFECT);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

		m_bScene = true;
	}
}

bool CBoss_Hive::CanAttack()
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

void CBoss_Hive::Attack()
{
	if (m_iAttackCount > m_iMaxAttackCount)
	{
		CObj* pLarvaEgg = CAbstractFactory<CLarva_Egg>::Create((float)m_ptMovePoint.x, (float)m_ptMovePoint.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pLarvaEgg);

		m_iAttackCount = 0;
	}
	else
	{
		++m_iAttackCount;
		CObj* pProjectile = CAbstractFactory<CBossAcidProjectile>::Create(m_tInfo.fX, m_tInfo.fY);
		pProjectile->Set_AtkPower(m_fAtkPower);
		pProjectile->Update_Rect();
		dynamic_cast<CBossAcidProjectile*>(pProjectile)->Set_TargetPos((float)m_ptMovePoint.x, (float)m_ptMovePoint.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ_PROJECTILE, pProjectile);
	}
}
