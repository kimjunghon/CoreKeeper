#pragma once
#include "Define.h"
#include "CObj.h"

class CObjMgr
{
private:
	CObjMgr();
	CObjMgr(const CObjMgr& rhs) = delete;
	CObjMgr& operator=(CObjMgr& rObjMgr) = delete;
	~CObjMgr();

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;

		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CObjMgr* m_pInstance;

public:
	void	Add_Object(OBJID eID, CObj* pObj);

	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

public:
	CObj*	Get_Player();
	list<CObj*>* Get_MonsterList() { return &m_ObjList[OBJ_MONSTER]; }
	vector<CObj*>* Get_VecWall() { return& m_vecWall; }
	list<CObj*>* Get_ProjectileList() { return &m_ObjList[OBJ_PROJECTILE]; }
	list<CObj*>* Get_ItemList() { return &m_ObjList[OBJ_ITEM]; }
	list<CObj*>* Get_BulidingList() { return &m_ObjList[OBJ_BUILDING]; }
	void	Delete_ID(OBJID eID);	
	void	Change_Wall(POINT _pt, WALLID _eTileID);
	void	Move_Wall_Roof_Frame(int _iIndex);
	void	Save_Player();
	void	Save_Wall();
	void	Save_Monster();
	void	Load_Player();
	void	Load_Wall();
	void	Load_Monster();

public:

private:
	list<CObj*> m_ObjList[OBJ_END];
	vector<CObj*> m_vecWall;

	list<CObj*> m_ObjRenderList[RENDER_END];
	
};

