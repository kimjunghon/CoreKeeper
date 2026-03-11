#pragma once
#include "CItem.h"
class CIron :
    public CItem
{
public:
    CIron();
    ~CIron();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Use() override;
    void Move_Frame() override;
};

