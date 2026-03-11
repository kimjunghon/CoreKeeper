#pragma once
#include "CUi.h"

class CPlayer_HpBar_Background :
    public CUi
{
public:
    CPlayer_HpBar_Background();
    ~CPlayer_HpBar_Background();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
};

