#pragma once
#include "CObj.h"
#include "CUi.h"

class CPlayer :
    public CObj
{
public:
    CPlayer();
    ~CPlayer();

public:
    void Initialize() override;
    int  Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    void Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir) override;

public:
    void EquipItem(CObj* _pObj);
    
    bool IsEquipWeapon();
    CObj* Get_EquipItem() { return m_pEquipItem; }
    void Set_Edit(bool _eEdit) { m_bEdit = _eEdit; }
    const INFO& Get_CollisionInfo() { return m_tCollisionInfo; }

    void Set_HpBar(CUi* _pUi);
    DIRECTION Get_Dir() { return m_eDir; }

    bool Get_CanMining() { return m_bCanMining; }
    void Set_CanMining(bool _bCanMing) { m_bCanMining = _bCanMing; }
    void Set_UiOpen(bool _bOpen) { m_bUiOpen = _bOpen; }
    void Set_CraftingOpen(bool _bCrafting) { m_bCraftingOpen = _bCrafting; }
    void Set_BossScene(bool _bScene) { m_bBossScene = _bScene; }
    void EatFood(int _iHp);

    void PowerUp(float _fAtkPower, float _fMiningPower, float _fDefPower, int _iMaxHp);

private:
    void Key_Input();
    void Change_State();
    void Move_Frame();
    void Change_Dir();
    void Check_Dir();
    void Set_CurItemInfo();
    void Set_Bmp();
    void Check_Equip();
   
private:
    //리소스
    HDC m_hLeftDC;
    HDC m_hRightDC;

    //플레이어 상태
    OBJSTATE m_eCurState;
    OBJSTATE m_ePreState;

    //플레이어 방향
    DIRECTION m_ePreDir;

    //애니메이션 초기화용 (tFrame 스타트가 0이 아니라 필요)
    int m_iFrameStart;

    //공격시 프레임 초기화용
    int m_iAttackFrameStart;
    int m_iAttackFrameEnd;

    //조건용 bool
    bool m_bUiOpen;
    bool m_bCraftingOpen;
    bool m_bEdit;
    bool m_bAttacking;
    
    //공격 딜레이
    DWORD m_dwAttackTime;
    DWORD m_dwAttackDelay;

    //방향 정하기용 마우스, 공격시 사용할 아이템
    CUi* m_pCursor;
    CUi* m_pHp_Bar;
    CObj* m_pEquipItem;

    CObj* m_pEquipHelm;
    CObj* m_pEquipChest;
    CObj* m_pEquipPants;

    //Hit 처리용
    float m_fHitSpeed;
    DIRECTION m_eHitDir;
    DWORD m_dwHitTime;
    DWORD m_dwHitDelay;

    bool m_bCanHit;
    DWORD m_dwCanHitTime;
    DWORD m_dwCanHitDelay;

    DWORD m_dwRecoveryTime;
    DWORD m_dwRecoveryDelay;

    DWORD m_dwDeadingTime;
    DWORD m_dwDeadingDelay;

    bool m_bCanMining;

    int m_iEquipSlot;

    float m_fBasicAtkPower;
    float m_fBasicDefPower;
    float m_fBasicMiningPower;
    int m_iBasicMaxHp;

    bool m_bBossScene;
};

