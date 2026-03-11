#pragma once
#include "CMonster.h"
class CSlime :
    public CMonster
{
public:
    CSlime();
    ~CSlime();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir) override;

public:
    void Move() override;
    void Move_Frame() override;
    void IsFindTarget() override;
    void Change_State() override;
private:
    bool CanAttack();

private:
    //Hit Ã³¸®¿ë
    float m_fHitSpeed;
    DIRECTION m_eHitDir;
    DWORD m_dwHitTime;
    DWORD m_dwHitDelay;

    bool m_bMove;

    bool m_bCanHit;
    DWORD m_dwCanHitTime;
    DWORD m_dwCanHitDelay;

};

