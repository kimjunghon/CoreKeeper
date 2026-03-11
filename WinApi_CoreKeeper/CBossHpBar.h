#pragma once
#include "CUi.h"
class CBossHpBar :
    public CUi
{
public:
    CBossHpBar();
    ~CBossHpBar();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_HP(int _iHp, int _iMaxHp);

private:
    int m_iHp;
    int m_iMaxHp;
    float m_fHP_Percentage;

    float m_fPerentageBmpfCX;

    CUi* m_pBackground;

};

