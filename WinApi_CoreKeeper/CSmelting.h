#pragma once
#include "CUi.h"

class CSmelting :
    public CUi
{
public:
    CSmelting();
    ~CSmelting();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    bool IsSmelting() { return m_bSmelting; }
private:
    CUi* m_pSmeltingSlot;
    CUi* m_pResultSlot;

    DWORD m_dwSmeltingTime;
    DWORD m_dwSmeltingDelay;

    int m_iSmeltingCount;
    int m_iSmeltingCountMax;

    bool m_bSmelting;

    HDC m_hFireDC;
};

