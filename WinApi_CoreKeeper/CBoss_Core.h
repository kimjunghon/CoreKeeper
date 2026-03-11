#pragma once
#include "CObj.h"

class CUi;

class CBoss_Core :
    public CObj
{
public:
    CBoss_Core();
    ~CBoss_Core();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_Boss_Core(int _iItemCode);

    bool IsEquipCore() { return m_bEquipCore; }
private:
    void Move_Frame();
    bool Check_CanOpen();

private:
    CUi* m_pCoreUi;

    bool m_bMoveFrame;
    bool m_bEquipCore;
    int m_iEquipItemCode;

    bool m_bUiOpen;
};

