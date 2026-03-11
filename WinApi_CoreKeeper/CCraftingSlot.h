#pragma once
#include "CSlot.h"
#include "CObj.h"

class CCraftingSlot :
    public CSlot
{
public:
    CCraftingSlot();
    ~CCraftingSlot();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    bool IsCrafting();
    CObj* Crafting();

    void Set_CraftingItem(int _iCraftingItemCode);
    int Get_CraftingItemCode() { return m_iCraftingItemCode; }

    HDC* Get_InfoDC() { return &m_hInfoDC; }
protected:
    bool m_bCrafting;

    int m_iCraftingItemCode;
    int m_iNeedItemCode;
    int m_iNeedItemCount;

    HDC m_hInfoDC;
};

