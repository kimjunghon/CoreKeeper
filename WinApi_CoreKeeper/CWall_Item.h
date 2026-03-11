#pragma once
#include "CItem.h"
class CWall_Item :
    public CItem
{
public:
    CWall_Item();
    ~CWall_Item();

public:
    virtual void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Use() override;
    void Move_Frame() override;

    void Set_WallItem(int _iItemCode);

protected:
    WALLID m_eWallID;
};

