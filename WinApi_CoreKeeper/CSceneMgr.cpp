#include "pch.h"
#include "CSceneMgr.h"
#include "CStart.h"
#include "CTitle.h"
#include "CPlay.h"
#include "CEdit.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CUiMgr.h"
#include "CCameraMgr.h"
#include "CClear.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;


CSceneMgr::CSceneMgr() : m_pScene(nullptr)
, m_eCurScene(SC_START), m_ePreScene(SC_END),
m_bBossScene(false),
m_iBossSceneStart(0),
m_bEnding(false)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Change_Scene(SCENE _eScene)
{
	m_eCurScene = _eScene;

	if (m_eCurScene != m_ePreScene)
	{
		Safe_Delete<CScene*>(m_pScene);

		switch (m_eCurScene)
		{
		case SC_START:
			m_pScene = new CStart;
			break;
		case SC_TITLE:
			m_pScene = new CTitle;
			break;
		case SC_PLAY:
			m_pScene = new CPlay;
			break;
		case SC_CLEAR:
			m_pScene = new CClear;
			break;
		case SC_EDIT:
			m_pScene = new CEdit;
			break;
		}

		m_pScene->Initialize();
		m_pScene->Update();
		m_pScene->Late_Update();

		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	if (m_bEnding)
		return;

	m_pScene->Update();

	if (!m_bBossScene)
	{
		m_dwBossSceneTime = GetTickCount();
		m_dwBossSceneDelay = 2000;
	}
}

void CSceneMgr::Late_Update()
{
	if (m_bEnding)
	{
		m_bfEnding.SourceConstantAlpha++;

		if (m_bfEnding.SourceConstantAlpha >= 255)
		{
			Change_Scene(SC_CLEAR);
			m_bEnding = false;
		}
	}

	m_pScene->Late_Update();

	if (m_bBossScene)
	{
		if(m_iBossSceneStart < 4)
		{
			if (m_dwBossSceneTime + m_dwBossSceneDelay < GetTickCount())
			{
				++m_iBossSceneStart;
				m_dwBossSceneTime = GetTickCount();

			}
		}
		else
		{
			Stop_BossScene();
		}
	}


}

void CSceneMgr::Render(HDC _hDC)
{
	m_pScene->Render(_hDC);

	if (m_bBossScene)
	{
		GdiTransparentBlt(_hDC, 0, 0, 800, 600,
			m_hScene, 800 * m_iBossSceneStart, 0, 800, 600, RGB(255, 255, 255));
	}
	else if (m_bEnding)
	{
		GdiAlphaBlend(_hDC, 0, 0, 800, 600,
			m_hScene,0,0,800,600, m_bfEnding);
	}
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}

void CSceneMgr::Play_BossScene(const TCHAR* pKey)
{
	m_bBossScene = true;

	m_hScene = CBmpMgr::Get_Instance()->Find_Image(pKey);

	CUiMgr::Get_Instance()->Set_Render(false);
}

void CSceneMgr::Stop_BossScene()
{
	if (m_iBossSceneStart > 3)
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_BossScene(false);

		m_bBossScene = false;
		m_iBossSceneStart = 0;

		CUiMgr::Get_Instance()->Set_Render(true);
	}
}

void CSceneMgr::Play_EndingScene()
{
	if (!m_bEnding)
	{
		m_bfEnding = { 0,0,100,0 };
	}

	m_bEnding = true;
	m_iBossSceneStart = 0;
	m_hScene = CBmpMgr::Get_Instance()->Find_Image(L"Ending_Scene");
	
}
