#include "pch.h"
#include "CDialogue.h"
#include "CBmpMgr.h"
#include "CButton.h"
#include "CAbstractFactory.h"
#include "CUiMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CInventory.h"

CDialogue::CDialogue() :
	m_iQuestNumber(0),
	m_bQuesting(false),
	m_bQuestClear(false),
	m_hTalkDC(nullptr),
	m_hQuestDC(nullptr),
	m_hClearDC(nullptr),
	m_hFailDC(nullptr)
{
	for (int i = BT_START; i < BT_END; i++)
	{
		m_pButton[i] = nullptr;
	}
}

CDialogue::~CDialogue()
{
	Release();
}

void CDialogue::Initialize()
{
	m_tInfo.fCX = 600.f;
	m_tInfo.fCY = 400.f;

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 300.f;

	m_tBmpInfo.fCX = 600.f;
	m_tBmpInfo.fCY = 400.f;

	m_eCurDialogueState = DIA_MAIN;
	m_ePreDialogueState = DIA_END;

	m_iQuestNumber = 1;
	m_iQuestItemCode = TURF_WALL;
	m_iQuestItemCount = 5;

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Dialogue");
	m_hTalkDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Talk_1");
	m_hQuestDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_1");
	m_hClearDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Clear_1");
	m_hFailDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Fail_1");
	m_hHelpDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Help_1");

	for (int i = BT_START; i < BT_END; i++)
	{
		m_pButton[i] = CUIFactory<CButton>::Create();
		dynamic_cast<CButton*>(m_pButton[i])->Set_Button((BUTTONID)i);
		m_pButton[i]->Update_Rect();
	}

	__super::Update_Rect();

}

void CDialogue::Update()
{

	m_bQuestClear = Check_Quest();
	if (m_bQuestClear)
		m_hHelpDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Help_Clear");


	if (m_bOpen)
	{

		__super::Update_Rect();


		m_bRender = true;
	}
	else
	{
		m_bRender = false;
		return;
	}


	for (int i = BT_START; i < BT_END; i++)
	{
		m_pButton[i]->Update();

		if (dynamic_cast<CButton*>(m_pButton[i])->ButtonClick())
		{
			switch (i)
			{
			case BT_START:
				if(m_bQuest)
				{
					if (m_bQuesting)
					{
						if (m_bQuestClear)
						{
							m_eCurDialogueState = DIA_QUEST_CLEAR;
						}
						else
						{
							m_eCurDialogueState = DIA_QUEST_FAIL;
						}
					}
					else
					{
						m_eCurDialogueState = DIA_TALK;
					}
				}
				break;
			case BT_STOP:
				CUiMgr::Get_Instance()->DialogueClose();
				dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(false);
				m_eCurDialogueState = DIA_MAIN;
				break;
			case BT_ACCEPT:
				m_eCurDialogueState = DIA_QUEST_START;
				m_bQuesting = true;
				break;
			case BT_REFUSE:
				m_eCurDialogueState = DIA_MAIN;
				break;
			}

			break;
		}
	}
}

void CDialogue::Late_Update()
{
	if(m_bOpen)
	{
		Change_State();
	}
}

void CDialogue::Render(HDC _hDC)
{
	if (m_bOpen)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));

		GdiTransparentBlt(_hDC, (int)m_tRect.left + 20, (int)m_tRect.top + 210, 500, 150,
			m_hTextDC, 0, 0, 500, 150, RGB(0, 255, 0));

		for (auto& pButton : m_pButton)
		{
			pButton->Render(_hDC);
		}
	}
	else if(!m_bOpen && m_bQuesting)
	{
		if (CUiMgr::Get_Instance()->Get_Inventory()->front()->IsRender())
		{
			return;
		}

		GdiTransparentBlt(_hDC, 600, 200, 200, 100,
			m_hHelpDC, 0, 0, 200, 100, RGB(0, 255, 0));

		if (!m_bQuestClear)
		{
			HDC hFont = CBmpMgr::Get_Instance()->Find_Image(L"FontNumber");
			int iCount = dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Check_ItemCount(m_iQuestItemCode);

			GdiTransparentBlt(_hDC, 682, 268, 8, 15,
				hFont, iCount * 15, 0, 15, 45, RGB(0, 0, 0));
		}
	}

}

void CDialogue::Release()
{
	for (auto& pButton : m_pButton)
	{
		Safe_Delete<CUi*>(pButton);
	}
	
}

int CDialogue::QuestFrame()
{

	if (m_bQuest)
	{
		if (m_bQuesting)
		{
			if (m_bQuestClear)
			{
				return 2;
			}
			else
			{
				return 1;
			}
		}
		return 0;
	}
	else
		return -1;
}

bool CDialogue::Check_Quest()
{
	return 	dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Check_Crafting(m_iQuestItemCode, m_iQuestItemCount);
}

void CDialogue::Quest_Clear()
{
	m_iQuestNumber++;
	
	m_bQuesting = false;

	if (m_iQuestNumber > 5)
	{
		m_bQuest = false;
		return;
	}

	switch (m_iQuestNumber)
	{
	case 2:
		m_hTalkDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Talk_2");
		m_hQuestDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_2");
		m_hClearDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Clear_2");
		m_hFailDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Fail_2");
		m_hHelpDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Help_2");
		m_iQuestItemCode = SLIME_MEAT;
		m_iQuestItemCount = 5;
		break;
	case 3:		
		m_hTalkDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Talk_3");
		m_hQuestDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_3");
		m_hClearDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Clear_3");
		m_hFailDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Fail_3");
		m_hHelpDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Help_3");
		m_iQuestItemCode = SLIME_CORE;
		m_iQuestItemCount = 1;
		break;
	case 4:
		m_hTalkDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Talk_4");
		m_hQuestDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_4");
		m_hClearDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Clear_4");
		m_hFailDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Fail_4");
		m_hHelpDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Help_4");
		m_iQuestItemCode = IRON_STONE;
		m_iQuestItemCount = 5;
		break;
	case 5:
		m_hTalkDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Talk_5");
		m_hQuestDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_5");
		m_hClearDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Clear_5");
		m_hFailDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Fail_5");
		m_hHelpDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Help_5");
		m_iQuestItemCode = HIVE_CORE;
		m_iQuestItemCount = 1;
		break;
	}
}

void CDialogue::Change_State()
{

	if (m_eCurDialogueState != m_ePreDialogueState)
	{
		m_ePreDialogueState = m_eCurDialogueState;

		for (auto& pButton : m_pButton)
		{
			pButton->Set_Render(false);
		}

		switch(m_eCurDialogueState)
		{
		case DIA_MAIN:
			m_pButton[BT_START]->Set_Render(true);
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = nullptr;
			break;
		case DIA_TALK:
			m_pButton[BT_ACCEPT]->Set_Render(true);
			m_pButton[BT_REFUSE]->Set_Render(true);
			m_hTextDC = m_hTalkDC;
			break;
		case DIA_QUEST_START:
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = m_hQuestDC;
			break;
		case DIA_QUEST_CLEAR:
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = m_hClearDC;
			Quest_Clear();
			break;
		case DIA_QUEST_FAIL:
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = m_hFailDC;
			break;
		}
	}
}

