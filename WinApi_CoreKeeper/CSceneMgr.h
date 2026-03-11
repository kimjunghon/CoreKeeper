#pragma once
#include "CScene.h"

class CSceneMgr
{
public:

private:
	CSceneMgr();
	CSceneMgr(const CSceneMgr& rhs) = delete;
	CSceneMgr& operator=(CSceneMgr& rSceneMgr) = delete;
	~CSceneMgr();


public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSceneMgr;
		}

		return m_pInstance;
	}
	static void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CSceneMgr* m_pInstance;

public:
	SCENE		Get_Scene() { return m_eCurScene; }
	
public:
	void Change_Scene(SCENE _eScene);
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

	void Play_BossScene(const TCHAR* pKey);
	void Stop_BossScene();
	void Play_EndingScene();

public:
	CScene* m_pScene;

	SCENE					m_eCurScene;
	SCENE					m_ePreScene;

	bool m_bBossScene;

	int m_iBossSceneStart;
	DWORD m_dwBossSceneTime;
	DWORD m_dwBossSceneDelay;

	HDC m_hScene;

	bool m_bEnding;

	BLENDFUNCTION m_bfEnding;

};

