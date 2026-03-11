#pragma once
#include "CMonster.h"
class CLarva :
    public CMonster
{
public:
    CLarva();
    ~CLarva();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Move() override;
    void Move_Frame() override;
    void IsFindTarget() override;
    void Change_State() override;

    void Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir) override;

private:
    void Set_Bmp();
    bool CanAttack();
private:
    HDC m_hLeftDC;
    HDC m_hRightDC;
    HDC m_hUpDC;
    HDC m_hDownDC;

    float m_fHitSpeed;
    DIRECTION m_eHitDir;
    DWORD m_dwHitTime;
    DWORD m_dwHitDelay;
    
    bool m_bMove;

    bool m_bCanHit;
    DWORD m_dwCanHitTime;
    DWORD m_dwCanHitDelay;

};

