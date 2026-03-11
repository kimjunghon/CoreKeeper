#pragma once
#include "CCrafting.h"
class CCooking :
    public CCrafting
{
public:
    CCooking();
    ~CCooking();
public:
    virtual void Initialize() override;
};

