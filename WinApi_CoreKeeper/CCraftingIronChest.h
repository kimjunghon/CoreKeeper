#pragma once
#include "CCraftingSlot.h"
class CCraftingIronChest :
    public CCraftingSlot
{
public:
    CCraftingIronChest();
    ~CCraftingIronChest();
public:
    virtual void Initialize() override;
};

