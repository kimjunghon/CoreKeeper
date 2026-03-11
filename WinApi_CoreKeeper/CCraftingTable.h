#pragma once
#include "CObj.h"
class CCraftingTable :
    public CObj
{
public:
    CCraftingTable();
    ~CCraftingTable();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    void Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir) override;

private:
    bool Check_CanOpen();

private:
    bool m_bUiOpen;
};

