#include "pch.h"
#include "CPlayer.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CCollisionMgr.h"
#include "CInventory.h"
#include "CItem.h"
#include "CPlayer_HpBar.h"
#include "CSoundMgr.h"
#include "CEquip.h"
#include "CEffect.h"
#include "CSlot.h"
#include "CHitDamage.h"

float g_fEffectVolume(1.f);

CPlayer::CPlayer() :
	m_eCurState(OS_IDLE),
	m_ePreState(OS_END),
	m_ePreDir(DIR_END),
	m_iFrameStart(0),
	m_iAttackFrameStart(0),
	m_iAttackFrameEnd(0),
	m_bUiOpen(false),
	m_bEdit(false),
	m_pCursor(nullptr),
	m_pEquipItem(nullptr),
	m_dwAttackTime(GetTickCount()),
	m_dwAttackDelay(0),
	m_hLeftDC(nullptr),
	m_hRightDC(nullptr),
	m_bAttacking(false),
	m_bCanHit(true),
	m_dwHitDelay(0),
	m_dwHitTime(0),
	m_dwCanHitTime(0),
	m_dwCanHitDelay(0),
	m_eHitDir(DIR_END),
	m_pHp_Bar(nullptr),
	m_bCanMining(true),
	m_iEquipSlot(0),
	m_bCraftingOpen(false),
	m_pEquipHelm(nullptr),
	m_pEquipChest(nullptr),
	m_pEquipPants(nullptr),
	m_fBasicAtkPower(0.f),
	m_fBasicDefPower(0.f),
	m_fBasicMiningPower(0.f),
	m_iBasicMaxHp(0),
	m_bBossScene(false)
{
	ZeroMemory(&m_tCollisionInfo, sizeof(m_tCollisionInfo));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tBmpInfo.fCX = 26.f;
	m_tBmpInfo.fCY = 26.f;

	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;

	m_tCollisionInfo.fCX = 40.f;
	m_tCollisionInfo.fCY = 40.f;
	m_fDistanceX = 0.f;
	m_fDistanceY = 16.f;

	m_eDir = DIR_DOWN;
	m_eObjID = OBJ_PLAYER;
	m_eRenderID = RENDER_OBJECT;

	m_dwAttackDelay = 100;

	m_dwHitDelay = 200;
	m_dwCanHitDelay = 600;

	m_dwRecoveryTime = GetTickCount();
	m_dwRecoveryDelay = 5000;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iMotion = 0;
	
	m_iMaxHp = PLAYER_MAXHP;
	m_iBasicMaxHp = PLAYER_MAXHP;
	m_iHp = m_iMaxHp;
	
	m_fSpeed = PLAYER_SPEED;
	m_fAtkPower = 0.f;
	m_fDefPower = 0.f;
	m_fBasicAtkPower = PLAYER_ATKPOWER;
	m_fBasicDefPower = PLAYER_DEFPOWER;
	m_fBasicMiningPower = PLAYER_MININGPOWER;
	m_fMass = 3.f;

	m_dwDeadingDelay = 2000;

	m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"NoEquip_Miner_L");
	m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"NoEquip_Miner_R");

	m_pCursor = CUiMgr::Get_Instance()->Get_Cursor();
	__super::Create_CollisionRect();
}

int CPlayer::Update()
{
	if (m_bBossScene)
		return OBJ_NOEVENT;

	if (m_bEdit)
	{
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}


	if (m_bDeading)
	{
		Check_Equip();
		__super::Update_Rect();

		if (m_pHp_Bar)
		{
			dynamic_cast<CPlayer_HpBar*>(m_pHp_Bar)->Set_HP(m_iHp, m_iMaxHp);
		}

		if (m_dwDeadingTime + m_dwDeadingDelay < GetTickCount())
		{
			m_bDeading = false;
			m_tInfo.fX = 1200.f;
			m_tInfo.fY = 1000.f;
			m_iHp = m_iMaxHp;
		}

		return OBJ_NOEVENT;
	}

	Check_Equip();

	if (m_pHp_Bar)
	{
		dynamic_cast<CPlayer_HpBar*>(m_pHp_Bar)->Set_HP(m_iHp, m_iMaxHp);
	}
	
	

	Key_Input();
	

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
			m_tInfo.fY += m_fHitSpeed;
			break;
		case DIR_UP:
			m_tInfo.fY -= m_fHitSpeed;
			break;
		}

		if (m_dwHitTime + m_dwHitDelay < GetTickCount())
		{
			m_bHit = false;
			m_dwCanHitTime = GetTickCount();
		}
	}

	if (!m_bHit && m_dwCanHitTime + m_dwCanHitDelay < GetTickCount())
	{
		m_bCanHit = true;
	}

	if (m_dwRecoveryTime + m_dwRecoveryDelay < GetTickCount())
	{
		m_iHp += 5;
		if (m_iHp >= m_iMaxHp)
			m_iHp = m_iMaxHp;

		m_dwRecoveryTime = GetTickCount();
	}

	__super::Update_Rect();
	__super::Update_CollisionRect();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	if (m_bBossScene)
		return;

	if (m_bEdit)
	{
		Set_Bmp();
		__super::Check_Render();
		return;
	}
	
	if (m_bDeading)
	{
		Set_Bmp();
		__super::Check_Render();
		return;
	}

	Check_Dir();
	Change_Dir();
	Change_State();
	Set_Bmp();
	Set_CurItemInfo();
	Move_Frame();

	if (m_eCurState == OS_WALK)
	{
		//CSoundMgr::Get_Instance()->StopSound(SOUND_FOOTSTEP);
		CSoundMgr::Get_Instance()->PlaySound(L"Footstep_Dirt.wav", SOUND_FOOTSTEP, 0.5f);
	}

	__super::Check_Render();
}

void CPlayer::Render(HDC _hDC)
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();
	GdiTransparentBlt(_hDC, (int)m_tRect.left - iCameraX, (int)m_tRect.top - iCameraY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		m_hObjDC, (int)m_tBmpInfo.fCX * m_tFrame.iFrameStart, (int)m_tBmpInfo.fCY * m_tFrame.iMotion, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(255, 255, 255));


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


void CPlayer::Set_HpBar(CUi* _pUi)
{
	if (!_pUi)
		return;

	m_pHp_Bar = _pUi;
}

void CPlayer::EatFood(int _iHp)
{
	m_iHp += _iHp;

	if (m_iHp >= m_iMaxHp)
		m_iHp = m_iMaxHp;
}

void CPlayer::PowerUp(float _fAtkPower, float _fMiningPower, float _fDefPower, int _iMaxHp)
{
	m_fBasicAtkPower += _fAtkPower;
	m_fBasicMiningPower += _fMiningPower;
	m_fBasicDefPower += _fDefPower;
	m_iBasicMaxHp += _iMaxHp;
}

void CPlayer::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_TAB))
	{
		if(!m_bUiOpen)
		{
			CUiMgr::Get_Instance()->InventoryOpen();
			m_bUiOpen = true;
		}
		else
		{
			CUiMgr::Get_Instance()->InventoryClose();
			m_bUiOpen = false;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('R'))
		m_iHp--;

	if(!m_bAttacking)
	{

		if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
			{
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
				//m_tInfo.fX += m_fSpeed;
				//m_tInfo.fY -= m_fSpeed;
				m_eCurState = OS_WALK;
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			{
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
				//m_tInfo.fX += m_fSpeed;
				//m_tInfo.fY += m_fSpeed;
				m_eCurState = OS_WALK;
			}
			else
			{
				m_tInfo.fX += m_fSpeed;
				m_eCurState = OS_WALK;
			}
		}

		else if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
			{
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
				//m_tInfo.fX -= m_fSpeed;
				//m_tInfo.fY -= m_fSpeed;
				m_eCurState = OS_WALK;
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			{
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
				//m_tInfo.fX -= m_fSpeed ;
				//m_tInfo.fY += m_fSpeed ;
				m_eCurState = OS_WALK;
			}
		else
			{
				m_tInfo.fX -= m_fSpeed;
				m_eCurState = OS_WALK;
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		{
			m_tInfo.fY -= m_fSpeed;
			m_eCurState = OS_WALK;
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		{
			m_tInfo.fY += m_fSpeed;
			m_eCurState = OS_WALK;
		}
		else
			m_eCurState = OS_IDLE;

		if (CKeyMgr::Get_Instance()->Key_Down('1'))
		{
			m_iEquipSlot = 0;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('2'))
		{
			m_iEquipSlot = 1;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('3'))
		{
			m_iEquipSlot = 2;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('4'))
		{
			m_iEquipSlot = 3;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('5'))
		{
			m_iEquipSlot = 4;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('6'))
		{
			m_iEquipSlot = 5;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('7'))
		{
			m_iEquipSlot = 6;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('8'))
		{
			m_iEquipSlot = 7;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('9'))
		{
			m_iEquipSlot = 8;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('0'))
		{
			m_iEquipSlot = 9;
		}
		
		if(!m_bUiOpen && !m_bCraftingOpen)
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
			{

				if (!m_bAttacking && m_dwAttackTime + m_dwAttackDelay < GetTickCount())
				{
					m_eCurState = OS_ATTACK;

					if (m_pEquipItem != nullptr)
					{
						dynamic_cast<CItem*>(m_pEquipItem)->Use();
						if (m_pEquipItem->Is_Dead())
							m_pEquipItem = nullptr;
					}
					else
					{
						CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
						CSoundMgr::Get_Instance()->PlaySound(L"Player_Attack_NoEqiup.wav", SOUND_EFFECT, 0.8f);
					}

					m_bAttack = true;
					m_bAttacking = true;

				}
			}
		}
	}
}

void CPlayer::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OS_IDLE:
			switch (m_eDir)
			{
			case DIR_DOWN:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			case DIR_UP:
				m_tFrame.iFrameStart = 2;
				m_tFrame.iFrameEnd = 2;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			case DIR_LEFT:
				m_tFrame.iFrameStart = 7;
				m_tFrame.iFrameEnd = 7;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			case DIR_RIGHT:
				m_tFrame.iFrameStart = 1;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			}
			break;
		case OS_WALK:
			switch (m_eDir)
			{
			case DIR_DOWN:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 1;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			case DIR_UP:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 3;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			case DIR_LEFT:
				m_tFrame.iFrameStart = 8;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 2;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			case DIR_RIGHT:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 2;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;
				break;
			}
			break;

		case OS_ATTACK:
			m_tFrame.iMotion = 4;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;

			m_iAttackFrameStart = 0;
			m_iAttackFrameEnd = 2;
			break;
		}

		m_iFrameStart = m_tFrame.iFrameStart;
		m_ePreState = m_eCurState;
	}

}


void CPlayer::Move_Frame()
{
	if (m_eCurState == OS_ATTACK)
	{
		switch (m_iAttackFrameStart)
		{
		case 0:
			switch(m_eDir)
			{
			case DIR_DOWN:
				m_tFrame.iFrameStart = 6;
				break;
			case DIR_UP:
				m_tFrame.iFrameStart = 8;
				break;
			case DIR_LEFT:
				m_tFrame.iFrameStart = 1;
				break;
			case DIR_RIGHT:
				m_tFrame.iFrameStart = 7;
				break;
			}
			break;
		case 1:
			switch (m_eDir)
			{
			case DIR_DOWN:
				m_tFrame.iFrameStart = 1;
				break;
			case DIR_UP:
				m_tFrame.iFrameStart = 4;
				break;
			case DIR_LEFT:
				m_tFrame.iFrameStart = 6;
				break;
			case DIR_RIGHT:
				m_tFrame.iFrameStart = 2;
				break;
			}
			break;
		case 2:
			switch (m_eDir)
			{
			case DIR_DOWN:
				m_tFrame.iFrameStart = 1;
				break;
			case DIR_UP:
				m_tFrame.iFrameStart = 5;
				break;
			case DIR_LEFT:
				m_tFrame.iFrameStart = 5;
				break;
			case DIR_RIGHT:
				m_tFrame.iFrameStart = 3;
				break;
			}
			break;
		}

		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_iAttackFrameStart;

			if (m_iAttackFrameStart > m_iAttackFrameEnd)
			{
				m_dwAttackTime = GetTickCount();
				m_iAttackFrameStart = 0;
				m_bAttacking = false;
				m_bAttack = false;
				m_bCanMining = true;
			}

			m_tFrame.dwTime = GetTickCount();
		}
	}
	else
	{
		if (m_eDir == DIR_LEFT)
		{
			if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
			{
				--m_tFrame.iFrameStart;

				if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
				{
					m_tFrame.iFrameStart = m_iFrameStart;
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
					m_tFrame.iFrameStart = m_iFrameStart;
				}

				m_tFrame.dwTime = GetTickCount();

			}
		}
	}
}

void CPlayer::Change_Dir()
{
	if (m_eDir != m_ePreDir)
	{
		m_ePreDir = m_eDir;
		m_ePreState = OS_END;
	}
}

void CPlayer::Check_Dir()
{
	if (!m_pCursor || m_bAttacking)
		return;

	float       fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

	fWidth = m_pCursor->Get_Info().fX - m_tInfo.fX;
	fHeight = m_pCursor->Get_Info().fY - m_tInfo.fY;

	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acosf(fWidth / fDiagonal);

	m_fAngle = fRadian * (180.f / PI);

	if (m_fAngle <= 45.f)
		m_eDir = DIR_RIGHT;
	else if (m_fAngle >= 135.f)	
		m_eDir = DIR_LEFT;
	else if (m_pCursor->Get_Info().fY > m_tInfo.fY)
		m_eDir = DIR_DOWN;
	else
		m_eDir = DIR_UP;

	if (m_pCursor->Get_Info().fY > m_tInfo.fY)
	{
		m_fAngle *= -1.f;
	}
}

void CPlayer::Set_CurItemInfo()
{
	if (!m_pEquipItem)
		return;

	float fX(0.f);
	float fY(0.f);

	if (m_bAttacking)
	{
		switch (m_eDir)
		{
		case DIR_UP:
			fX = m_tInfo.fX + 10.f;
			fY = m_tInfo.fY - 15.f;
			break;
		case DIR_DOWN:
			fX = m_tInfo.fX - 10.f;
			fY = m_tInfo.fY + 25.f;
			break;
		case DIR_LEFT:
			fX = m_tInfo.fX - 15.f;
			fY = m_tInfo.fY + 4.f;
			break;
		case DIR_RIGHT:
			fX = m_tInfo.fX + 15.f;
			fY = m_tInfo.fY + 4.f;
			break;
		}
	}
	else
	{
		switch (m_eDir)
		{
		case DIR_UP:
			fX = m_tInfo.fX + 15.f;
			fY = m_tInfo.fY - 1.f;
			break;
		case DIR_DOWN:
			fX = m_tInfo.fX - 10.f;
			fY = m_tInfo.fY + 8.f;
			break;
		case DIR_LEFT:
			fX = m_tInfo.fX + 10.f;
			fY = m_tInfo.fY + 6.f;
			break;
		case DIR_RIGHT:
			fX = m_tInfo.fX - 10.f;
			fY = m_tInfo.fY + 6.f;
			break;
		}
	}

	m_pEquipItem->Set_Pos(fX,fY);
	m_pEquipItem->Set_Dir(m_eDir);
	m_pEquipItem->Set_Angle(m_fAngle);
	m_pEquipItem->Update_Rect();
}

void CPlayer::Set_Bmp()
{
	if (m_eDir == DIR_LEFT)
	{
		m_hObjDC = m_hLeftDC;
	}
	else
	{
		m_hObjDC = m_hRightDC;
	}
}

void CPlayer::Check_Equip()
{
	CInventory* pUi = dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory());

	if (pUi)
		EquipItem(pUi->Equip_Item(m_iEquipSlot));

	if (m_pEquipItem)
	{
		m_fAtkPower = m_pEquipItem->Get_AtkPower() + m_fBasicAtkPower;
		m_fMass = m_pEquipItem->Get_Mass();
		m_fMiningPower = m_pEquipItem->Get_MiningPower() + m_fBasicMiningPower;
	}
	else
	{
		m_fAtkPower = m_fBasicAtkPower;
		m_fMass = 3.f;
		m_fMiningPower = m_fBasicMiningPower;
	}

	for (auto& InventoryList : *(CUiMgr::Get_Instance()->Get_Inventory()))
	{
		if (InventoryList->Get_UiID() == UI_EQUIP)
		{
			CEquip* pEquip = dynamic_cast<CEquip*>(InventoryList);

			CSlot* pHelmSlot = dynamic_cast<CSlot*>(pEquip->Get_HelmSlot());
			m_pEquipHelm = (pHelmSlot->Get_Item());

			CSlot* pChestSlot = dynamic_cast<CSlot*>(pEquip->Get_ChestSlot());
			m_pEquipChest = (pChestSlot->Get_Item());

			CSlot* pPantsslot = dynamic_cast<CSlot*>(pEquip->Get_PantsSlot());
			m_pEquipPants = (pPantsslot->Get_Item());
		}
	}

	//DEF, HP
	float fDef(0.f);
	int   iMaxHp(0);
	
	if (m_pEquipHelm)
	{
		fDef += m_pEquipHelm->Get_DefPower();
		iMaxHp += m_pEquipHelm->Get_MaxHp();
	}

	if (m_pEquipChest)
	{
		fDef += m_pEquipChest->Get_DefPower();
		iMaxHp += m_pEquipChest->Get_MaxHp();
	}

	if (m_pEquipPants)
	{
		fDef += m_pEquipPants->Get_DefPower();
		iMaxHp += m_pEquipPants->Get_MaxHp();
	}

	m_fDefPower = fDef + m_fBasicDefPower;
	m_iMaxHp = iMaxHp + m_iBasicMaxHp;
	
	//BMP
	if (!m_pEquipHelm && !m_pEquipChest && !m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"NoEquip_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"NoEquip_Miner_R");
	}
	else if (m_pEquipHelm && !m_pEquipChest && !m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Helm_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Helm_Miner_R");
	}
	else if (!m_pEquipHelm && m_pEquipChest && !m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Chest_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Chest_Miner_R");
	}
	else if (!m_pEquipHelm && !m_pEquipChest && m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Pants_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Pants_Miner_R");
	}
	else if (m_pEquipHelm && m_pEquipChest && !m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_HelmChest_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_HelmChest_Miner_R");
	}
	else if (m_pEquipHelm && !m_pEquipChest && m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_HelmPants_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_HelmPants_Miner_R");
	}
	else if (!m_pEquipHelm && m_pEquipChest && m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_ChestPants_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_ChestPants_Miner_R");
	}
	else if (m_pEquipHelm && m_pEquipChest && m_pEquipPants)
	{
		m_hLeftDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Full_Miner_L");
		m_hRightDC = CBmpMgr::Get_Instance()->Find_Image(L"Iron_Full_Miner_R");
	}
	m_hSlotDC = m_hRightDC;
}



void CPlayer::EquipItem(CObj* _pObj)
{
	if (m_pEquipItem != _pObj)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
		CSoundMgr::Get_Instance()->PlaySound(L"QuickSlot.wav", SOUND_UI, 0.8f);
	}

	if(m_pEquipItem)
	{
		dynamic_cast<CItem*>(m_pEquipItem)->Set_Equip(false);
		m_pEquipItem->Set_Render(false);
	}

	m_pEquipItem = _pObj;

	if(m_pEquipItem)
	{
		dynamic_cast<CItem*>(m_pEquipItem)->Set_Equip(true);
		m_pEquipItem->Set_Render(true);
	}
}

bool CPlayer::IsEquipWeapon()
{
	if (m_pEquipItem)
		return true;
	else
		return false;
}


void CPlayer::Release()
{
	m_hLeftDC = nullptr;
	m_hRightDC = nullptr;
}

void CPlayer::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{

	if(m_bCanHit && !m_bDeading)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		m_dwRecoveryTime = GetTickCount();

		float AtkDmg = _fAtkDmg - m_fDefPower;

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
		dynamic_cast<CHitDamage*>(pObj)->Set_HitDamage(OBJ_PLAYER, (int)AtkDmg);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);

		if (AtkDmg <= 0.f)
		{
			m_bCanHit = false;
			m_dwCanHitTime = GetTickCount();
		}
		else
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"HitPlayer.wav", SOUND_EFFECT, g_fEffectVolume);
			
			m_iHp -= (int)AtkDmg;

			if (m_iHp < 0)
			{
				CObj* pEffect = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY);
				dynamic_cast<CEffect*>(pEffect)->Set_Effect(EFFECT_POTAL);
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
				m_bDeading = true;
				m_dwDeadingTime = GetTickCount();
				m_hObjDC = m_hRightDC;
				m_tFrame.iFrameStart = 6;
				m_tFrame.iMotion = 5;
				return;
			}

			m_fHitSpeed = _fMass * (_fSpeed / 2.f);
			m_eHitDir = _eDir;
			m_bHit = true;
			m_bCanHit = false;
			m_dwHitTime = GetTickCount();
		}
	}

}
