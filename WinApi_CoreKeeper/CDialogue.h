#pragma once
#include "CUi.h"

class CDialogue :
    public CUi
{
public:
    CDialogue();
    ~CDialogue();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void Set_State(DIALOGUESTATE _eDialogueState) { m_eCurDialogueState = _eDialogueState; }
    int  QuestFrame();
private:
    bool Check_Quest();
    void Quest_Clear();
    void Change_State();

private:
    int m_iQuestNumber;
    int m_iQuestItemCode;
    int m_iQuestItemCount;

    bool m_bQuest;
    bool m_bQuesting;
    bool m_bQuestClear;

    DIALOGUESTATE m_eCurDialogueState;
    DIALOGUESTATE m_ePreDialogueState;

    CUi* m_pButton[BT_END];

    HDC m_hTalkDC;
    HDC m_hQuestDC;
    HDC m_hClearDC;
    HDC m_hFailDC;
    HDC m_hTextDC;
    HDC m_hHelpDC;
};

