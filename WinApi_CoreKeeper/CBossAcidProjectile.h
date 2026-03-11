#pragma once
#include "CObj.h"
class CBossAcidProjectile :
    public CObj
{
public:
    CBossAcidProjectile();
    ~CBossAcidProjectile();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_TargetPos(float _fTargetX, float _fTargetY);
private:
    void Move();
    void Move_Frame();

private:
    bool m_bFalling;
    float m_fTargetX;
    float m_fTargetY;

    CObj* m_pWarningEffect;
};

