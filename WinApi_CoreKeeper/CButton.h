#pragma once
#include "CUi.h"
class CButton :
    public CUi
{
public:
    CButton();
    ~CButton();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    bool ButtonClick();
    void Set_Button(BUTTONID _eButtinID);
};

