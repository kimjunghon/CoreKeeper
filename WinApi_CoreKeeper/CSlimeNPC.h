#pragma once
#include "CObj.h"
class CUi;

class CSlimeNPC :
    public CObj
{
public:
    CSlimeNPC();
    ~CSlimeNPC();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    void Move_Frame();
    bool Check_Dialogue();

private:
    bool m_bUiOpen;

    CUi* m_pDialogue;

    HDC m_hQuestDC;
};

