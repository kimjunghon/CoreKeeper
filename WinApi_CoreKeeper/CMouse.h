#pragma once
#include "CUi.h"
#include "CObj.h"

class CMouse :
    public CUi
{
public:
    CMouse();
    ~CMouse();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_Item(CObj* _pObj) { m_pItem = _pObj; }
    
    void Set_InfoDC(HDC* _pInfoDC, int _iFrame = 0);

    CObj* Get_Item() { return m_pItem; }

private:
    CObj* m_pItem;

    int m_iFrame;
    HDC* m_hInfoDC;
};

