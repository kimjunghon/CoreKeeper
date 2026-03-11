#pragma once
#include "CMelee.h"

class CWoodSword : public CMelee
{
public:
	CWoodSword();
	~CWoodSword();

public:
	void Initialize() override;
};

