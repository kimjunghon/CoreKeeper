#pragma once
#include "Define.h"
#include "CObj.h"

class CCameraMgr
{
private:
	CCameraMgr();
	CCameraMgr(const CCameraMgr& rhs) = delete;
	CCameraMgr& operator=(CCameraMgr& rCameraMgr) = delete;
	~CCameraMgr();

public:
	static CCameraMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CCameraMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CCameraMgr* m_pInstance;

public:
	void	Set_Pos(float _fX, float _fY) { m_fCameraX = _fX, m_fCameraY = _fY; }
	void	Set_Target(CObj* _pObj) { m_pTarget = _pObj; }
	

	float	Get_RenderX() { return m_fDiffX; }
	void	Set_RenderX(float _fX);
	float	Get_RenderY() { return m_fDiffY; }
	void	Set_RenderY(float _fY);

	void	Update();

	void CameraShake(DWORD _dwDelay, float _fShake);

private:
	void CalDiff();
	void CameraLock();
private:
	float m_fCameraX;
	float m_fCameraY;

	float m_fDiffX;
	float m_fDiffY;

	CObj* m_pTarget;

	DWORD m_dwShakeTime;
	DWORD m_dwShakeDelay;

	bool m_bShake;
	float m_fShake;
};

