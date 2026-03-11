#pragma once
#include "CUi.h"
class CMinimap :
    public CUi
{
public:
    CMinimap();
    ~CMinimap();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    vector<MINIMAPINFO> m_vecMinimapTile;

    HDC m_hMinimapTile[4];
    HDC m_hMinimapPlayer;

    INFO m_tMinimapTileInfo;

    bool m_bFull;
};

