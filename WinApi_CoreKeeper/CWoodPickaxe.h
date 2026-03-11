#pragma once
#include "CMelee.h"
class CWoodPickaxe :
    public CMelee
{
public:
	CWoodPickaxe();
	~CWoodPickaxe();

public:
	void Initialize() override;
};

