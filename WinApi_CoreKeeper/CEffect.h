#pragma once
#include "CObj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    ~CEffect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_Effect(int _iEffectNumber);

private:
    void Move_Frame();

    int m_iEffectNumber;
};

