#pragma once
#include "CUi.h"

class CEquip :
    public CUi
{
public:
    CEquip();
    ~CEquip();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    CUi* Get_HelmSlot() { return m_pSlot[0]; }
    CUi* Get_ChestSlot() { return m_pSlot[1]; }
    CUi* Get_PantsSlot() { return m_pSlot[2]; }
private:
    CUi* m_pSlot[3];

    HDC m_hPlayerDC;
};

