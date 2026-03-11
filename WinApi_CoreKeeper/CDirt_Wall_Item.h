#pragma once
#include "CWall_Item.h"
class CDirt_Wall_Item :
    public CWall_Item
{
public:
    CDirt_Wall_Item();
    ~CDirt_Wall_Item();

public:
    void Initialize() override;
};

