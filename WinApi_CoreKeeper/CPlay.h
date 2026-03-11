#pragma once
#include "CScene.h"
#include "CShadow.h"


class CPlay :
    public CScene
{
public:
    CPlay();
    ~CPlay();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:

    CShadow     Shadw;
};

