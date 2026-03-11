#pragma once
#include "CMelee.h"
class CIronSword :
    public CMelee
{
public:
	CIronSword();
	~CIronSword();

public:
	void Initialize() override;
};

