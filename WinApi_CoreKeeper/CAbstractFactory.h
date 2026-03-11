#pragma once

#include "CObj.h"
#include "CTile.h"
#include "CWall.h"
#include "CUi.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj* Create()
	{
		CObj* pObj = new T;

		pObj->Initialize();

		return pObj;
	}

	static CObj* Create(float fX, float fY, float fAngle = 0.f)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		pObj->Set_Angle(fAngle);

		return pObj;
	}


};

template<typename T>
class CTileFactory
{
public:
	static CTile* Create()
	{
		CTile* pTile = new T;

		pTile->Initialize();

		return pTile;
	}
	static CTile* Create(float fX, float fY)
	{
		CTile* pTile = new T;

		pTile->Initialize();
		pTile->Set_Pos(fX, fY);

		return pTile;
	}
};

template<typename T>
class CUIFactory
{
public:
	static CUi* Create()
	{
		CUi* pUi = new T;

		pUi->Initialize();

		return pUi;
	}
	static CUi* Create(float _fX, float _fY)
	{
		CUi* pUi = new T;

		pUi->Initialize();
		pUi->Set_Pos(_fX, _fY);

		return pUi;
	}
};