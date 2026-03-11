#pragma once
#include "CCraftingSlot.h"
class CCraftingWoodSword :
    public CCraftingSlot
{
public:
    CCraftingWoodSword();
    ~CCraftingWoodSword();
public:
    virtual void Initialize() override;
};

