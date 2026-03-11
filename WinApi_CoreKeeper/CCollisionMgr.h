#pragma once
#include "CObj.h"
#include "CTile.h"
#include "CUi.h"

class CCollisionMgr
{
private:
	CCollisionMgr();
	CCollisionMgr(const CCollisionMgr& rhs) = delete;
	CCollisionMgr& operator=(CCollisionMgr& rCollisionMgr) = delete;
	~CCollisionMgr();

public:
	static CCollisionMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CCollisionMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CCollisionMgr* m_pInstance;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Release();

	void Set_Player(CObj* _pPlayer) { m_pPlayer = _pPlayer; }
	void Set_Mouse(CUi* _pMouse) { m_pMouse = _pMouse; }
	void Set_MonsertList(list<CObj*>* _pMonsterList) { m_pMonsterList = _pMonsterList; }
	void Set_ItemList(list<CObj*>* _pItemList) { m_pItemList = _pItemList; }
	void Set_BulidingList(list<CObj*>* _pBulidingList) { m_pBulidingList = _pBulidingList; }
	void Set_VecTile(vector<CTile*>* _pVecTile) { m_pVecTile = _pVecTile; }
	void Set_VecWall(vector<CObj*>* _pVecWall) { m_pVecWall = _pVecWall; }
	void Set_Inventory(list<CUi*>* _pInventoryList);
	void Set_Crafting(list<CUi*>* _pCraftingList);

	void Push_CollisionSlotList(CUi* _pSlot, SLOTID _eSlotID);

private:
	void	Obj_Tile_Collision_RectEx(CObj* _pObj);
	bool	Check_Tile_Rect(const INFO& _tInfo, CTile* _pTile, float* _pX, float* _pY);
	void	Obj_Wall_Collision_RectEx(CObj* _pObj);
	bool	Check_Wall_Rect(const INFO& _tInfo, CObj* _pWall, float* _pX, float* _pY);
	void	Mouse_Slot_Collision_Rect();

	void	Obj_Idle_Collision_RectEx(CObj* _pDst, CObj* _pSrc);
	void	Obj_Attack_Collision_RectEx(CObj* _pDst, CObj* _pSrc);
	bool	Check_Obj_Rect(const INFO& _tDst, const INFO& _tSrc, float* _pX, float* _pY);

	void	Player_Monster_Collision(CObj* _pMonster);
	void	Player_Wall_Collision(CObj* _pPlayer, vector<CObj*>* _pVecWall);
	bool Find_Target_Wall(const INFO& _tCollisionInfo, const INFO& _tWallCollisionInfo);

private:
	CObj*			m_pPlayer;
	CUi*			m_pMouse;

	list<CObj*>* m_pMonsterList;
	list<CObj*>* m_pProjectileList;
	list<CObj*>* m_pItemList;
	list<CObj*>* m_pBulidingList;

	vector<CTile*>*	m_pVecTile;
	vector<CObj*>*	m_pVecWall;

//	vector<CUi*>* m_pSlotList[SLOT_END];
	vector<CUi*> m_pSlotList[SLOT_END];
};

