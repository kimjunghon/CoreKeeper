#pragma once
#include "CObj.h"

class CItem :
    public CObj
{
public:
    CItem();
    virtual ~CItem();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;

    virtual void Use();
protected:
    virtual void Move_Frame();
    
public:
    void    Set_Equip(bool _bEquip) { m_bEquip = _bEquip; }
    void    Set_Inventory(bool _bInventory) { m_bInventory = _bInventory; }
    bool    IsEquip() { return m_bEquip; }
    bool    IsInventory() { return m_bInventory; }

    int     Get_ItemCode() { return m_iItemCode; }
    int     Get_ItemCount() { return m_iCount; }
    bool    IsItemFullCount() { return m_iCount >= m_iMaxCount; }
    
    void    Set_Item(int _iItemCode);
    void    Set_ItemCount(int _iItemCount) { m_iCount = _iItemCount; }
    void    IncreaseCount(int _iItemCount);
    void    DecreaseCount(int _iItemCount)
    {
        m_iCount -= _iItemCount;
        if (m_iCount <= 0)
            m_bDead = true;
    }
    ITEMCLASS Get_ItemClass() { return m_eItemClass; }

    HDC* Get_InfoDC() { return &m_hInfoDC; }

protected:
    ITEMCLASS m_eItemClass;
    bool      m_bEquip;
    bool      m_bInventory;

    int m_iCount;
    int m_iMaxCount;

    int m_iItemCode;

    HDC m_hInfoDC;

    DWORD m_dwDropTime;
    DWORD m_dwDropDelay;
};

