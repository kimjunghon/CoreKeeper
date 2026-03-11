#pragma once
#include "CObj.h"
class CWall :
    public CObj
{
public:
    CWall();
    ~CWall();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;  
    void Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir) override;

   
public:
    void Set_BmpPos(float _fX, float _fY) { m_fBmpPosX = _fX, m_fBmpPosY = _fY; }
    const WALLID& Get_WallID() { return m_eWallID; }
    void Set_WallID(WALLID _eWallID);

private:
    float m_fBmpPosX;
    float m_fBmpPosY;
 
    WALLID m_eWallID;

    DWORD m_dwTime;

    DWORD m_dwHitTime;
    DWORD m_dwHitDelay;
};