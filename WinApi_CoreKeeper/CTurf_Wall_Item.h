#pragma once
#include "CWall_Item.h"
class CTurf_Wall_Item :
    public CWall_Item
{
public:
    CTurf_Wall_Item();
    ~CTurf_Wall_Item();

public:
    void Initialize() override;
};

