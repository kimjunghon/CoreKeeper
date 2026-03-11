#pragma once
#include "CObj.h"
class CProjectile :
    public CObj
{
public:
    CProjectile();
    virtual ~CProjectile();

public:
    virtual void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_Projectile(int _iItemCode);
private:
    void Move();
    void Move_Frame();
    void Set_Bmp();

protected:
    HDC m_hLeft;
    HDC m_hRight;

};

