#pragma once
#include "CObj.h"
class CMain_Core :
    public CObj
{
public:
    CMain_Core();
    ~CMain_Core();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_SlimeCore(CObj* _pSlimeCore) { m_pSlimeCore = _pSlimeCore;}
    void Set_HiveCore(CObj* _pHiveCore) { m_pHiveCore = _pHiveCore; }
private:
    void Move_Frame(int _iCore);
private:
    CObj* m_pSlimeCore;
    CObj* m_pHiveCore;

    bool m_bSlimeCore;
    bool m_bHiveCore;
};

