#pragma once
#include "CCraftingSlot.h"

class CCraftingIronSword :
    public CCraftingSlot
{
public:
    CCraftingIronSword();
    ~CCraftingIronSword();
public:
    virtual void Initialize() override;
};

