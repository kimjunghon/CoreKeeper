#pragma once
#include "CCraftingSlot.h"
class CCraftingWoodBow :
    public CCraftingSlot
{
public:
    CCraftingWoodBow();
    ~CCraftingWoodBow();
public:
    virtual void Initialize() override;
};

