#pragma once
#include "CCrafting.h"
class CIronCrafting :
    public CCrafting
{
public:
    CIronCrafting();
    ~CIronCrafting();

public:
    void Initialize() override;
};

