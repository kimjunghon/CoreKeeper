#pragma once
#include "CUi.h"
class CCrafting :
    public CUi
{
public:
    CCrafting();
    virtual ~CCrafting();
public:
    virtual void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    vector<CUi*>* Get_vecCraftingSlot() { return &m_vecCraftingSlot; }
protected:
    vector<CUi*> m_vecCraftingSlot;
    
};

