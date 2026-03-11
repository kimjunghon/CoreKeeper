#include "pch.h"
#include "CObj.h"
#include "CCameraMgr.h"

CObj::CObj() : 
	m_eDir(DIR_END), 
	m_eObjID(OBJ_END),
	m_eRenderID(RENDER_END),
	m_fAtkPower(0.f),
	m_fDefPower(0.f),
	m_fMiningPower(0.f),
	m_fAngle(0.f),
	m_fSpeed(0.f),
	m_bDeading(false),
	m_bDead(false),
	m_hObjDC(nullptr),
	m_bRender(true),
	m_bAttack(false),
	m_fDistanceX(0.f),
	m_fDistanceY(0.f),
	m_bHit(false),
	m_fMass(0.f)
{
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tCollisionInfo, sizeof(m_tCollisionInfo));
	ZeroMemory(&m_tBmpInfo, sizeof(m_tBmpInfo));
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}

CObj::~CObj()
{
}

void CObj::Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir)
{
}

void CObj::Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Update_CollisionRect()
{
	float fCX(0.f), fCY(0.f), fX(0.f), fY(0.f);
	int iIndex(0);
	
	if (m_tCollisionInfo.fCY > TILECY || m_tCollisionInfo.fCX > TILECX)
	{
		fCY = m_tCollisionInfo.fCY;
		int iCountY = (int)ceil(fCY / TILECY);

		for (int i = 0; i < iCountY; i++)
		{
			if (m_tCollisionInfo.fCX > TILECX)
			{
				fCX = m_tCollisionInfo.fCX;
				int iCountX = (int)ceil(fCX / TILECX);

				for (int j = 0; j < iCountX; j++)
				{
					iIndex = j + (i * iCountX);

					m_vecCollisionInfo[iIndex].fY = m_tRect.top + (TILECY* 0.5f) + m_fDistanceY + (i * TILECY);
					m_vecCollisionInfo[iIndex].fX = m_tRect.left + (TILECX * 0.5f) + m_fDistanceX + (j * TILECX);
				}
			}
			else
			{
				iIndex = i;
				m_vecCollisionInfo[iIndex].fY = m_tRect.top + (TILECY * 0.5f) + m_fDistanceY + (i * TILECY);
				m_vecCollisionInfo[iIndex].fX = m_tRect.left + (TILECX * 0.5f) + m_fDistanceX;

			}
		}
	}
	else
	{
		m_vecCollisionInfo[0].fY = m_tInfo.fY + m_fDistanceY;
		m_vecCollisionInfo[0].fX = m_tInfo.fX + m_fDistanceX;
	}
}

void CObj::Create_CollisionRect()
{
	float fCX(0.f), fCY(0.f), fX(0.f), fY(0.f);

	INFO tInfo = { 0.f, 0.f, TILECX, TILECY };

	if(m_tCollisionInfo.fCY > TILECY || m_tCollisionInfo.fCX > TILECX)
	{
		fCY = m_tCollisionInfo.fCY;
		int iCountY = (int)ceil(fCY / TILECY);

		for (int i = 0; i < iCountY; i++)
		{
			if (m_tCollisionInfo.fCX > TILECX)
			{
				fCX = m_tCollisionInfo.fCX;
				int iCountX = (int)ceil(fCX / (float)TILECX);

				for (int j = 0; j < iCountX; j++)
				{
					m_vecCollisionInfo.push_back(tInfo);
				}
			}
			else
			{
				m_vecCollisionInfo.push_back(tInfo);
			}
		}
	}
	else
	{
		tInfo.fCX = m_tCollisionInfo.fCX;
		tInfo.fCY = m_tCollisionInfo.fCY;

		m_vecCollisionInfo.push_back(tInfo);
	}
}

void CObj::Check_Render()
{
	int iCameraX = (int)CCameraMgr::Get_Instance()->Get_RenderX();
	int iCameraY = (int)CCameraMgr::Get_Instance()->Get_RenderY();

	//if (iCameraX < m_tRect.right || iCameraX + WINCX > m_tRect.left || iCameraY < m_tRect.bottom || iCameraY + WINCY > m_tRect.top)
	//{
	//	m_bRender = true;
	//}

	if (iCameraX < m_tRect.right && iCameraX + WINCX > m_tRect.left && iCameraY < m_tRect.bottom && iCameraY + WINCY > m_tRect.top)
	{
		m_bRender = true;
	}
	else
		m_bRender = false;
}
