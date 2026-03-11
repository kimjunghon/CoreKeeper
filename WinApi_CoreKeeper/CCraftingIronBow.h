#pragma once
#include "CCraftingSlot.h"
class CCraftingIronBow :
    public CCraftingSlot
{
public:
    CCraftingIronBow();
    ~CCraftingIronBow();
public:
    virtual void Initialize() override;
};

