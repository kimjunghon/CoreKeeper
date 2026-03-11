#pragma once
#include "CObj.h"

class CUi;

class CForge :
    public CObj
{
public:
    CForge();
    ~CForge();

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

    int m_iFrame;
    CUi* m_pSmelting;
};

