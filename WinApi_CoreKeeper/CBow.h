#pragma once
#include "CItem.h"
class CBow :
    public CItem
{
public:
    CBow();
    ~CBow();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    void Use() override;
    void Move_Frame() override;

    void Set_BowItem(int _iItemCode);
protected:
    void Set_Bmp();
protected:
    WEAPONGRADE m_eWeaponGrade;
    
    HDC m_hLeftDC;
    HDC m_hRightDC;

    bool m_bAttack;

};

