#pragma once
#include "CObj.h"
class CCookingPot :
    public CObj
{
public:
    CCookingPot();
    ~CCookingPot();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    bool Check_CanOpen();

private:
    bool m_bUiOpen;
};

