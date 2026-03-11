#include "pch.h"
#include "CCameraMgr.h"

CCameraMgr* CCameraMgr::m_pInstance = nullptr;

CCameraMgr::CCameraMgr():
	m_pTarget(nullptr),
	m_fCameraX(0.f),
	m_fCameraY(0.f),
	m_fDiffX(0.f),
	m_fDiffY(0.f),
	m_bShake(false),
	m_fShake(0.f)
{
}

CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::Set_RenderX(float _fX)
{
	if (m_fDiffX + _fX < 0.f)
		m_fDiffX = 0.f;
	else if (m_fDiffX + _fX > (TILECX * TILEX) - WINCX)
		m_fDiffX = (TILECX * TILEX) - WINCX;
	else
		m_fDiffX += _fX;
}

void CCameraMgr::Set_RenderY(float _fY)
{
	if (m_fDiffY + _fY < 0.f)
		m_fDiffY = 0.f;
	else if (m_fDiffY + _fY > (TILECY * TILEY) - WINCY)
		m_fDiffY = (TILECY * TILEY) - WINCY;
	else
		m_fDiffY += _fY;
}

void CCameraMgr::Update()
{
	if (m_pTarget)
	{
		if (m_pTarget->Is_Dead())
			m_pTarget = nullptr;
		else
		{
			m_fCameraX = m_pTarget->Get_Info().fX;
			m_fCameraY = m_pTarget->Get_Info().fY;
		}
	}

	CalDiff();

	if (m_bShake)
	{
		m_fShake *= -1.f;

		m_fDiffX += m_fShake;
		m_fDiffY -= m_fShake;
		
		if (m_dwShakeTime + m_dwShakeDelay < GetTickCount())
		{
			m_bShake = false;
		}
	}

}

void CCameraMgr::CameraShake(DWORD _dwDelay, float _fShake)
{
	m_bShake = true;
	m_dwShakeTime = GetTickCount();
	m_dwShakeDelay = _dwDelay;
	m_fShake = _fShake;
}

void CCameraMgr::CalDiff()
{
	if(m_fCameraX - WINCX * 0.5f <= 0.f)
		m_fDiffX = 0.f;
	else if (m_fCameraX - WINCX * 0.5f >= (TILECX * TILEX) - WINCX)
		m_fDiffY = (TILECX * TILEX) - WINCX;
	else
		m_fDiffX = m_fCameraX - WINCX * 0.5f;
	
	if (m_fCameraY - WINCY * 0.5f <= 0.f)
		m_fDiffY = 0.f;
	else if (m_fCameraY - WINCY * 0.5f >= (TILECY * TILEY) - WINCY)
		m_fDiffY = (TILECY * TILEY) - WINCY;
	else
		m_fDiffY = m_fCameraY - WINCY * 0.5f;
}

void CCameraMgr::CameraLock()
{

}
