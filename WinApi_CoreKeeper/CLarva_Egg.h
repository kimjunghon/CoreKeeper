#pragma once
#include "CMonster.h"
class CLarva_Egg :
    public CMonster
{
public:
    CLarva_Egg();
    ~CLarva_Egg();

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
    bool m_bCanHit;
    DWORD m_dwCanHitTime;
    DWORD m_dwCanHitDelay;
};

