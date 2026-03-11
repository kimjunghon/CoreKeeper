#pragma once
#include "Define.h"


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;


	virtual void Hit(float _fAtkDmg, float _fMass, float _fSpeed, DIRECTION _eDir);

public:
	
	//렌더링
	void		Update_Rect();
	void		Check_Render();
	void		Set_Render(bool _bRender) { m_bRender = _bRender; }
	bool		IsRender() { return m_bRender; }
	const BMPINFO& Get_BmpInfo() { return m_tBmpInfo; }

	//슬롯에 그려줄 BMP
	HDC* Get_SlotDC() { return &m_hSlotDC; }

	//Obj 정보
	const RECT& Get_Rect() { return m_tRect; }
	const INFO& Get_Info() { return m_tInfo; }
	const INFO& Get_CollisionInfo() { return m_tCollisionInfo; }
	void		Set_Pos(float _fX, float _fY) { m_tInfo.fX = _fX, m_tInfo.fY = _fY; }
	void		Set_PosX(float _fX) { m_tInfo.fX += _fX; }
	void		Set_PosY(float _fY) { m_tInfo.fY += _fY; }
	void		Set_Dir(DIRECTION _eDir) { m_eDir = _eDir; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	const RENDERID&	Get_RenderID() { return m_eRenderID; }
	bool		Is_Dead() { return m_bDead; }
	void		Set_Speed(float _fSpeed) { m_fSpeed = _fSpeed; }

	//공격
	bool		IsAttack() { return m_bAttack; }
	void		SetAttack(bool _bAttack) { m_bAttack = _bAttack; }
	
	//충돌 처리용
	void		Create_CollisionRect();
	void		Update_CollisionRect();
	const vector<INFO>& Get_VecCollisionInfo() { return m_vecCollisionInfo; }
	float		Get_AtkPower() { return m_fAtkPower; }
	float		Get_Mass() { return m_fMass; }
	float		Get_Speed() { return m_fSpeed; }
	float		Get_MiningPower() { return m_fMiningPower; }
	float		Get_DefPower() { return m_fDefPower; }
	int			Get_Hp() { return m_iHp; }
	int			Get_MaxHp() { return m_iMaxHp; }
	void		Set_AtkPower(float _fAtkPower) { m_fAtkPower = _fAtkPower; }
	OBJID		Get_ObjID() { return m_eObjID; }
	void		Set_Dead() { m_bDead = true; }
protected:
	INFO		m_tInfo;
	BMPINFO		m_tBmpInfo;
	FRAME		m_tFrame;
	RECT		m_tRect;
	DIRECTION	m_eDir;
	OBJID		m_eObjID;
	RENDERID	m_eRenderID;

	//Render DC
	HDC			m_hObjDC;
	HDC			m_hSlotDC;
	
	//Obj Info
	int m_iHp;
	int m_iMaxHp;

	float		m_fAtkPower;
	float		m_fDefPower;
	float		m_fMiningPower; //채굴피해
	float		m_fSpeed;
	
	float		m_fAngle;
	bool		m_bDeading;
	bool		m_bDead;
	bool		m_bRender;

	bool		m_bAttack;

	
	//충돌 처리용
	INFO		m_tCollisionInfo;
	vector<INFO> m_vecCollisionInfo;
	float m_fDistanceX;
	float m_fDistanceY;

	float		m_fMass;
	bool		m_bHit;
};

