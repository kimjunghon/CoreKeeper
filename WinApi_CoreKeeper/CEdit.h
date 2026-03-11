#pragma once
#include "CScene.h"
class CEdit :
    public CScene
{
public:
    CEdit();
    ~CEdit();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    
private:
    void Key_Input();

    TILEID m_eTileID;
    WALLID m_eWallID;

};

