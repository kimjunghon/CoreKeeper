#pragma once
#include "CObj.h"
class CHitDamage :
    public CObj
{
public:
    CHitDamage();
    ~CHitDamage();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_HitDamage(OBJID _eObjID, int _iDamage);
private:
    int m_iDamage;

    DWORD m_dwEffectTime;
    DWORD m_dwEffectDelay;

};

