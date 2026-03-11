#pragma once
#include "Define.h"

class CShadow
{
public:
	CShadow();
	~CShadow();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

private:
	void Update_Rect();
private:
	INFO m_tInfo;
	INFO m_tBmpInfo;
	RECT m_tRect;

	HDC m_hShadowDC;

};

