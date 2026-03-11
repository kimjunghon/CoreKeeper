#pragma once
#include "CUi.h"
class CPlayer_Stat :
    public CUi
{
public:
    CPlayer_Stat();
    ~CPlayer_Stat();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
};

