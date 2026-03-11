#pragma once
#include "CMonster.h"

class CUi;

class CBoss_Slime :
    public CMonster
{
public:
    CBoss_Slime();
    ~CBoss_Slime();

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
    bool Check_Scene();
    void Play_Scene();
private:
    bool CanAttack();

private:
    bool m_bMove;

    DWORD m_dwHitTime;
    DWORD m_dwHitDelay;

    bool m_bCanHit;
    DWORD m_dwCanHitTime;
    DWORD m_dwCanHitDelay;

    CUi* m_pHp_Bar;

    int m_iDeathEffectCount;
    DWORD m_dwDeathEffectTime;
    DWORD m_dwDeathEffectDelay;

    bool m_bScene;
    int m_iSceneCount;
    DWORD m_dwSceneTime;
    DWORD m_dwSceneDelay;

    float m_fSceneY;
};

