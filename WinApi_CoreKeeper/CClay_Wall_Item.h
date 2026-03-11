#pragma once
#include "CWall_Item.h"

class CClay_Wall_Item :
	public CWall_Item
{
public:
    CClay_Wall_Item();
    ~CClay_Wall_Item();

public:
    void Initialize() override;
};

