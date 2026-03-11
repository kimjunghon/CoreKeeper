#pragma once
#include "CScene.h"
class CStart :
    public CScene
{
public:
    CStart();
    ~CStart();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
};

