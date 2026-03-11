#pragma once
#include "CUi.h"

class CCoreEquip :
    public CUi
{
public:
    CCoreEquip();
    ~CCoreEquip();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    bool IsCoreEquip();
    void Set_CoreEquip(int _iItemCode);
private:
    CUi* m_pSlot;
};

