#pragma once
#include "CCraftingSlot.h"
class CCraftingIronPants :
    public CCraftingSlot
{
public:
    CCraftingIronPants();
    ~CCraftingIronPants();
public:
    virtual void Initialize() override;
};

