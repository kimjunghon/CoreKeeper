#pragma once
#include "CItem.h"
class CMelee :
    public CItem
{
public:
    CMelee();
    virtual ~CMelee();

public:
    virtual void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    void Use() override;
    void Move_Frame() override;

    void Set_MeleeItem(int _iItemCode);
private:
    void Set_Bmp();

protected:
    HDC m_hLeftDC;
    HDC m_hRightDC;
    HDC m_hUpDC;
    HDC m_hDownDC;

    bool m_bAttack;
};