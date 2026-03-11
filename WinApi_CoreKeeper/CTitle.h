#pragma once
#include "CScene.h"
class CTitle :
    public CScene
{
public:
    CTitle();
    ~CTitle();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Move_Frame();

private:
    FRAME   m_tBackgroundFrame;
    INFO    m_tTextInfo;

};

