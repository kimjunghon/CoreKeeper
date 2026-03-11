#pragma once
#include "CBow.h"
class CWoodBow :
    public CBow
{
public:
    CWoodBow();
    ~CWoodBow();

public:
    void Initialize() override;
};

