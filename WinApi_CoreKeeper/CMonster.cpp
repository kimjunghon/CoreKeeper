#include "pch.h"
#include "CMonster.h"


CMonster::CMonster() :
	m_pTarget(nullptr),
	m_dwMovePointTime(0),
	m_dwMovePointDelay(0),
	m_fFindRange(0.f),
	m_dwAttackTime(0),
	m_dwAttackDelay(0),
	m_eCurState(OS_IDLE),
	m_ePreState(OS_END),
	m_bReadyToAttack(false),
	m_bEdit(false),
	m_eMonsterID(MON_END)
{
	ZeroMemory(&m_tCollisionInfo, sizeof(m_tCollisionInfo));
	ZeroMemory(&m_ptMovePoint, sizeof(m_ptMovePoint));
}

CMonster::~CMonster()
{
}

