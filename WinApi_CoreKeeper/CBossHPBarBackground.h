#pragma once
#include "CUi.h"
class CBossHPBarBackground :
    public CUi
{
public:
    CBossHPBarBackground();
    ~CBossHPBarBackground();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
};

