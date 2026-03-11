#pragma once
#include "CScene.h"
class CClear :
    public CScene
{
public:
    CClear();
    ~CClear();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
};

