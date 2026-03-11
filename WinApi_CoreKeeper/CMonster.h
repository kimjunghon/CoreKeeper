#pragma once
#include "CObj.h"

class CMonster :
    public CObj
{
public:
    CMonster();
    virtual ~CMonster();

public:
    virtual void Move() PURE;
    virtual void Move_Frame() PURE;
    virtual void IsFindTarget() PURE;
    virtual void Change_State() PURE;

    void Set_Edit(bool _eEdit) { m_bEdit = _eEdit; }
    MONSTERID Get_MonsterID() { return m_eMonsterID; }
protected:
    CObj* m_pTarget;

    float m_fFindRange;

    DWORD m_dwMovePointTime;
    DWORD m_dwMovePointDelay;

    POINT m_ptMovePoint;

    DWORD m_dwAttackTime;
    DWORD m_dwAttackDelay;

    OBJSTATE m_eCurState;
    OBJSTATE m_ePreState;

    MONSTERID m_eMonsterID;

    bool m_bReadyToAttack;

    bool m_bEdit;
};

