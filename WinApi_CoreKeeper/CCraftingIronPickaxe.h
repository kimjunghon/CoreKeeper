#pragma once
#include "CCraftingSlot.h"
class CCraftingIronPickaxe :
    public CCraftingSlot
{
public:
    CCraftingIronPickaxe();
    ~CCraftingIronPickaxe();
public:
    virtual void Initialize() override;
};