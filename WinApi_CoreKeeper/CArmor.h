#pragma once
#include "CItem.h"
class CArmor :
    public CItem
{
public:
    CArmor();
    ~CArmor();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Use() override;
    void Move_Frame() override;

public:
    void Set_Armor(int _iItemCode);
};

