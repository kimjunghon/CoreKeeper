#include "pch.h"
#include "CWoodPickaxe.h"
#include "CBmpMgr.h"

CWoodPickaxe::CWoodPickaxe()
{
}

CWoodPickaxe::~CWoodPickaxe()
{
}

void CWoodPickaxe::Initialize()
{
	m_tBmpInfo.fCX = 40.f;
	m_tBmpInfo.fCY = 40.f;

	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 50;

	m_eDir = DIR_END;



	m_iItemCode = WOOD_PICKAXE;

}
