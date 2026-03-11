#pragma once
#include "CCraftingSlot.h"
class CCraftingIronHelm :
    public CCraftingSlot
{
public:
    CCraftingIronHelm();
    ~CCraftingIronHelm();
public:
    virtual void Initialize() override;
};

